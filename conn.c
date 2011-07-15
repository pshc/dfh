#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <zmq.h>
#include "conn.h"
#include "proto.h"

struct conn {
	void *ctx;
	pthread_key_t key;
	uint16_t screen[80][25];
	int dirty;
};

static int send_msg(struct conn *conn, const uint8_t *buf, size_t len);

void send_blit(struct conn *conn, int tile, int x, int y) {
	conn->screen[x][y] = htons((uint16_t) tile);
	conn->dirty = 1;
}

void send_flip(struct conn *conn) {
	if (!conn->dirty)
		return;
	send_msg(conn, (uint8_t *) conn->screen, sizeof conn->screen);
	conn->dirty = 0;
}

static void sock_dtor(void *sock) {
	zmq_close(sock);
}

struct conn *init_conn(void) {
	void *ctx = zmq_init(1);
	if (!ctx)
		return NULL;
	struct conn *conn = malloc(sizeof *conn);
	if (!conn) {
		zmq_term(ctx);
		return NULL;
	}
	conn->ctx = ctx;
	pthread_key_create(&conn->key, sock_dtor);
	memset(conn->screen, 0, sizeof conn->screen);
	conn->dirty = 1;
	return conn;
}

int conn_poll(struct conn *conn, int ms) {
	void *sock = pthread_getspecific(conn->key);
	if (!sock) {
		usleep(ms * 1000);
		return 0;
	}
	zmq_pollitem_t item = {sock, 0, ZMQ_POLLERR};
	int res = zmq_poll(&item, 1, ms * 1000);
	if (res < 0)
		perror("zmq_poll");
	else if (res > 0)
		fprintf(stderr, "Error on socket?!\n");
	return res;
}

static int send_msg(struct conn *conn, const uint8_t *buf, size_t len) {
	void *sock = pthread_getspecific(conn->key);
	if (!sock) {
		sock = zmq_socket(conn->ctx, ZMQ_PUB);
		if (!sock) {
			perror("zmq_socket");
			return -1;
		}
		if (zmq_connect(sock, OUT_ADDR) != 0) {
			perror("zmq_connect");
			zmq_close(sock);
			return -1;
		}
		if ((errno = pthread_setspecific(conn->key, sock))) {
			perror("pthread_setspecific");
			zmq_close(sock);
			return -1;
		}
	}
	zmq_msg_t msg;
	if (zmq_msg_init_size(&msg, len) != 0) {
		perror("zmq_msg_init_size");
		return -1;
	}
	memcpy(zmq_msg_data(&msg), buf, len);
	if (zmq_send(sock, &msg, 0) < 0) {
		perror("zmq_send");
		zmq_msg_close(&msg);
		return -1;
	}
	zmq_msg_close(&msg);
	return 0;
}
