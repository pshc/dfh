#include <SDL_video.h>
#include <SDL_image.h>
#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>

#define TRACE do { puts( __func__ ); fflush(stdout); } while (0)

SDL_Surface *IMG_Load(const char *file) {
	TRACE;
	printf("Loading %s\n", file);
	return SDL_CreateRGBSurface(SDL_SWSURFACE, 640, 300, 32, 0, 0, 0, 0);
}
