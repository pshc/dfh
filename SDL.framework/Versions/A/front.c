#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <zmq.h>
#include "proto.h"

static void *ctx, *in_sock;

#define CHECK_LEN(x) do { if (len != (x)) { printf("Bad length: %u\n", (unsigned int) len); return; } } while (0)

static void on_recv(uint8_t *buf, size_t len) {
	if (!len) {
		printf("Got empty message.\n");
		return;
	}
	switch (buf[0]) {
	case BLIT: {
		CHECK_LEN(5);
		int tile = U16(buf, 1), x = buf[3], y = buf[4];
		printf("%03d @ %02d,%02d\n", tile, x, y);
	}
		break;

	case FLIP:
		CHECK_LEN(1);
		break;

	default:
		printf("Unrecognized message type 0x%02x", buf[0]);
	}
}

static int poll(void) {
	zmq_pollitem_t items[] = {
		{in_sock, 0, ZMQ_POLLIN | ZMQ_POLLERR},
	};
	while (1) {
		int r = zmq_poll(items, sizeof items / sizeof items[0], -1);
		assert(r != 0);
		if (r < 0) {
			perror("zmq_poll");
			break;
		}
		if (items[0].revents & ZMQ_POLLIN) {
			zmq_msg_t msg;
			if (zmq_recv(in_sock, &msg, ZMQ_NOBLOCK) != 0) {
				assert(errno != EAGAIN);
				perror("zmq_recv");
				return 0;
			}
			on_recv(zmq_msg_data(&msg), zmq_msg_size(&msg));
			zmq_msg_close(&msg);
		}
		if (items[0].revents & ZMQ_POLLERR) {
			fprintf(stderr, "poll err?!\n");
			return 0;
		}
	}
	return 1;
}

int main(void) {
	ctx = zmq_init(1);
	if (!ctx) {
		perror("zmq_init");
		return 1;
	}
	in_sock = zmq_socket(ctx, ZMQ_SUB);
	if (!in_sock) {
		perror("zmq_socket");
		return 1;
	}
	if (zmq_bind(in_sock, OUT_ADDR) != 0) {
		perror("zmq_bind");
		return 1;
	}
	zmq_setsockopt(in_sock, ZMQ_SUBSCRIBE, "", 0);
	int res = poll();
	zmq_close(in_sock);
	zmq_term(ctx);
	return res ? 0 : 1;
}
