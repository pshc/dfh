#include <SDL_video.h>
#include <SDL_image.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include "sdl.h"

#define TRACE do { puts( __func__ ); fflush(stdout); } while (0)

static int find_number(const char **str) {
	const char *pos = *str;
	int number = -1;
	while (1) {
		char c = *pos;
		if (!c)
			break;
		if (c >= '0' && c <= '9') {
			unsigned int candidate;
			if (sscanf(pos, "%d", &candidate) == 1)
				if (candidate <= INT_MAX)
					number = (int) candidate;
			do {
				c = *++pos;
			} while (c >= '0' && c <= '9');
			break;
		}
		pos++;
	}
	*str = pos;
	return number;
}

SDL_Surface *IMG_Load(const char *file) {
	TRACE;
	printf("Loading %s\n", file);
	const char *pos = file;
	int width = find_number(&pos);
	int height = find_number(&pos);
	if (width < 100 || width > 1024)
		width = 640;
	if (height < 100 || height > 1024)
		height = 300;
	SDL_Surface *s = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0, 0, 0, 0);
	if (s)
		s->hwdata->filename = strdup(file);
	return s;
}
