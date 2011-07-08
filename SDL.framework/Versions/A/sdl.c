#include <SDL.h>
#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define TRACE do { puts( __func__ ); fflush(stdout); } while (0)

static struct timeval start_time;

int SDL_Init(Uint32 flags) {
	TRACE;
	gettimeofday(&start_time, NULL);
	return 0;
}

int SDL_InitSubSystem(Uint32 flags) {
	TRACE;
	return 0;
}

/* GL */

static int gl_attributes[17] = {0};

int SDL_GL_SetAttribute(SDL_GLattr attr, int value) {
	TRACE;
	assert(attr < (sizeof gl_attributes / sizeof gl_attributes[0]));
	gl_attributes[attr] = value;
	return 0;
}

int SDL_GL_GetAttribute(SDL_GLattr attr, int *value) {
	TRACE;
	assert(attr < (sizeof gl_attributes / sizeof gl_attributes[0]));
	*value = gl_attributes[attr];
	return 0;
}

/* IO */

int SDL_EnableKeyRepeat(int delay, int interval) {
	TRACE;
	return 0;
}

int SDL_EnableUNICODE(int enable) {
	TRACE;
	static int mode = 0;
	if (enable == -1)
		return mode;
	int old = mode;
	mode = enable;
	return old;
}

int SDL_ShowCursor(int toggle) {
	TRACE;
	static int current = SDL_ENABLE;
	if (toggle != SDL_QUERY)
		current = toggle;
	return current;
}

Uint32 SDL_GetTicks(void) {
	//TRACE;
	struct timeval now;
	gettimeofday(&now, NULL);
	return (now.tv_sec - start_time.tv_sec) * 1000 + (now.tv_usec - start_time.tv_usec) / 1000;
}

int SDL_PollEvent(SDL_Event *event) {
	TRACE;
	/*
	if (anevent) {
		if (event) {
			pop(anevent);
			event->type = SDL_KEYDOWN;
		}
		return 1;
	}
	*/
	return 0;
}

Uint8 SDL_GetAppState(void) {
	//TRACE;
	//return SDL_APPMOUSEFOCUS | SDL_APPINPUTFOCUS | SDL_APPACTIVE;
	return SDL_APPINPUTFOCUS | SDL_APPACTIVE;
}

Uint8 SDL_GetMouseState(int *x, int *y) {
	TRACE;
	if (x)
		*x = 0;
	if (y)
		*y = 0;
	return 0;
	//return SDL_BUTTON(1);
}

int SDL_NumJoysticks(void) {
	//TRACE;
	return 0;
}

void SDL_Delay(Uint32 ms) {
	usleep(ms);
}

/* WINDOW */

void SDL_WM_SetCaption(const char *title, const char *icon) {
	TRACE;
	printf("Window title: %s\n", title);
	SDL_WM_SetIcon(NULL, NULL);
}

static SDL_Surface *cur_icon = NULL;

void SDL_WM_SetIcon(SDL_Surface *icon, Uint8 *mask) {
	TRACE;
	if (cur_icon != icon) {
		if (cur_icon)
			SDL_FreeSurface(cur_icon);
		cur_icon = icon;
		if (cur_icon)
			cur_icon->refcount++;
	}
}


const SDL_VideoInfo *SDL_GetVideoInfo(void) {
	TRACE;
	static SDL_VideoInfo video_info;
	static int video_setup = 0;
	if (!video_setup) {
		memset(&video_info, 0, sizeof video_info);
		video_info.video_mem = 128 * 1024;
		video_info.current_w = 1024;
		video_info.current_h = 768;
		video_setup = 1;
	}
	return &video_info;
}

static SDL_Surface *video_surface = NULL;

SDL_Surface *SDL_SetVideoMode(int width, int height, int bpp, Uint32 flags) {
	TRACE;
	if (!video_surface)
		video_surface = SDL_CreateRGBSurface(flags, width, height, bpp, 0, 0, 0, 0);
	return video_surface;
}

SDL_Surface *SDL_GetVideoSurface(void) {
	TRACE;
	/* Should handle a null video surface... */
	return video_surface;
}

int SDL_Flip(SDL_Surface *screen) {
	TRACE;
	return 0;
}

/* PIXELS */

SDL_Surface *SDL_CreateRGBSurface(Uint32 flags, int width, int height, int depth, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask) {
	//TRACE;
	SDL_PixelFormat *f = calloc(1, sizeof *f);
	if (!f)
		return NULL;
	f->palette = NULL;
	f->Rmask = Rmask;
	f->Gmask = Gmask;
	f->Bmask = Bmask;
	f->Amask = Amask;
	f->BitsPerPixel = depth;
	f->BytesPerPixel = depth/8;

	SDL_Surface *s = calloc(1, sizeof *s);
	if (!s) {
		free(f);
		return NULL;
	}
	s->flags = flags;
	s->format = f;
	s->w = width;
	s->h = height;
	s->pitch = f->BytesPerPixel * width;
	s->refcount = 1;

	s->pixels = malloc(width * height * f->BytesPerPixel);
	if (!s->pixels) {
		free(f);
		free(s);
		return NULL;
	}

	static int ctr = 0;
	s->offset = ++ctr;
	printf("Made surface %d\n", ctr);
	return s;
}

Uint32 SDL_MapRGB(const SDL_PixelFormat * const fmt, const Uint8 r, const Uint8 g, const Uint8 b) {
	//TRACE;
	return 0xffffffff;
}

int SDL_SetColorKey(SDL_Surface *surface, Uint32 flag, Uint32 key) {
	TRACE;
	surface->format->colorkey = key;
	return 0;
}

SDL_Surface *SDL_ConvertSurface(SDL_Surface *src, SDL_PixelFormat *fmt, Uint32 flags) {
	TRACE;
	assert(fmt->BitsPerPixel && fmt->BitsPerPixel % 8 == 0);
	SDL_Surface *dest = SDL_CreateRGBSurface(flags, src->w, src->h, fmt->BitsPerPixel, 0, 0, 0, 0);
	if (!dest)
		return NULL;
	memmove(dest->format, fmt, sizeof *fmt);
	return dest;
}

void SDL_FreeSurface(SDL_Surface *surface) {
	//TRACE;
	if (!surface)
		return;
	if (0 == --surface->refcount) {
		printf("Free surface %d\n", surface->offset);
		free(surface->format);
		free(surface->pixels);
		free(surface);
	}
}

int SDL_SetAlpha(SDL_Surface *surface, Uint32 flag, Uint8 alpha) {
	//TRACE;
	surface->format->alpha = alpha;
	return 0;
}

int SDL_UpperBlit(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect) {
	//TRACE;
	return 0;
}

void SDL_GetRGBA(Uint32 pixel, const SDL_PixelFormat *fmt, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a) {
	*r = *g = *b = *a = 0xff;
}

SDL_Surface *SDL_DisplayFormat(SDL_Surface *surface) {
	TRACE;
	return SDL_ConvertSurface(surface, surface->format, surface->flags);
}

int SDL_FillRect(SDL_Surface *dst, SDL_Rect *dstrect, Uint32 color) {
	TRACE;
	return 0;
}

int SDL_LockSurface(SDL_Surface *surface) {
	//TRACE;
	return 0;
}

void SDL_UnlockSurface(SDL_Surface *surface) {
	//TRACE;
}

/* THREADING */

Uint32 SDL_ThreadID(void) {
	TRACE;
	assert(sizeof(pthread_t) == sizeof(Uint32));
	return (Uint32) pthread_self();
}

struct bounce {
	int (*fn)(void *);
	void *data;
};

static void *boing(void *context) {
	struct bounce *bounce = context;
	int (*fn)(void *) = bounce->fn;
	void *data = bounce->data;
	free(bounce);
	printf("Boing!\n");
	int result = fn(data);
	printf("Thump.\n");
	return (void *) result;
}

SDL_Thread *SDL_CreateThread(int (*fn)(void *), void *data) {
	TRACE;
	pthread_t thread;
	struct bounce *bounce = malloc(sizeof *bounce);
	bounce->fn = fn;
	bounce->data = data;
	if (pthread_create(&thread, NULL, boing, bounce) != 0) {
		fprintf(stderr, "Thread creation failed.\n");
		free(bounce);
		return NULL;
	}
	assert(sizeof thread == sizeof(SDL_Thread *));
	return (SDL_Thread *) thread;
}

struct SDL_semaphore {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	Uint32 val;
};

SDL_sem *SDL_CreateSemaphore(Uint32 initial_value) {
	TRACE;
	SDL_sem *sem = malloc(sizeof *sem);
	if (pthread_mutex_init(&sem->mutex, NULL) != 0) {
		fprintf(stderr, "Mutex creation failed.\n");
		free(sem);
		return NULL;
	}
	if (pthread_cond_init(&sem->cond, NULL) != 0) {
		fprintf(stderr, "Condvar creation failed.\n");
		pthread_mutex_destroy(&sem->mutex);
		free(sem);
		return NULL;
	}
	sem->val = initial_value;
	return sem;
}

int SDL_SemWait(SDL_sem *sem) {
	TRACE;
	int res = pthread_mutex_lock(&sem->mutex);
	if (res != 0) {
		if (res == EDEADLK)
			fprintf(stderr, "Would deadlock (wait).\n");
		else
			fprintf(stderr, "Bad semaphore (wait).\n");
		return -1;
	}

	while (sem->val == 0) {
		if (pthread_cond_wait(&sem->cond, &sem->mutex) != 0) {
			fprintf(stderr, "Bad cond for waiting\n");
			pthread_mutex_unlock(&sem->mutex);
			return -1;
		}
	}
	sem->val--;
	pthread_mutex_unlock(&sem->mutex);
	return 0;
}

int SDL_SemPost(SDL_sem *sem) {
	TRACE;
	int res = pthread_mutex_lock(&sem->mutex);
	if (res != 0) {
		if (res == EDEADLK)
			fprintf(stderr, "Would deadlock (post).\n");
		else
			fprintf(stderr, "Bad semaphore (post).\n");
		return -1;
	}
	sem->val++;
	pthread_cond_signal(&sem->cond);
	pthread_mutex_unlock(&sem->mutex);
	return 0;
}

int SDL_SemTryWait(SDL_sem *sem) {
	TRACE;
	int res = pthread_mutex_trylock(&sem->mutex);
	if (res != 0) {
		if (res == EBUSY)
			return SDL_MUTEX_TIMEDOUT;
		fprintf(stderr, "Bad semaphore (trywait).\n");
		return -1;
	}
	if (sem->val == 0) {
		pthread_mutex_unlock(&sem->mutex);
		return SDL_MUTEX_TIMEDOUT;
	}
	sem->val--;
	pthread_mutex_unlock(&sem->mutex);
	return 0;
}
