#define HI(t) (((t)>>8) & 0xff)
#define LO(t) ((t) & 0xff)

#define U16(a, i) (((a)[(i)]<<8) | (a)[(i+1)])

#define OUT_ADDR "tcp://127.0.0.1:6767"

enum {
	NONE=0,
	BLIT,
	FLIP,
};


