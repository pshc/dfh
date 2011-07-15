
struct conn;

struct conn *init_conn(void);
int conn_poll(struct conn *, int ms);

void send_blit(struct conn *, int tile, int x, int y);
void send_flip(struct conn *conn);
