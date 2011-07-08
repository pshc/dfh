#include <SDL.h>
#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>

#define TRACE do { puts( __func__ ); fflush(stdout); } while (0)
