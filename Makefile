CC = gcc
CFLAGS = -pthread -dynamiclib -arch i386 -Wall $(shell pkg-config SDL_image --cflags)
LIBS = -L/opt/i386/lib -lzmq
LDFLAGS = -headerpad_max_install_names,-undefined,dynamic_lookup,-compatibility_version,1.0,-current_version,1.0,-arch,i386

SDL = SDL.framework/Versions/A/PaulSDL
SDL_INSTALL = -install_name,/usr/local/lib/libPaulSDL.1.dylib

SDL_IMAGE = SDL_image.framework/Versions/A/PaulSDL_image
SDL_IMAGE_INSTALL = -install_name,/usr/local/lib/libPaulSDL_image.1.dylib

all: $(SDL) $(SDL_IMAGE)

$(SDL): conn.c sdl.c SDL.framework
	$(CC) $(CFLAGS) $(LIBS) -Wl,$(LDFLAGS),$(SDL_INSTALL) -o $@ conn.c sdl.c

$(SDL_IMAGE): image.c SDL_image.framework
	$(CC) $(CFLAGS) $(LIBS) -Wl,$(LDFLAGS),$(SDL_IMAGE_INSTALL) -o $@ image.c

SDL.framework:
	mkdir -p SDL.framework/Versions/A/
SDL_image.framework:
	mkdir -p SDL_image.framework/Versions/A/

clean:
	rm -rf -- SDL.framework SDL_image.framework
