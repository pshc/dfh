CFLAGS = -pthread -dynamiclib -arch i386 -Wall $(shell pkg-config SDL_image --cflags)
LDFLAGS = -headerpad_max_install_names,-undefined,dynamic_lookup,-compatibility_version,1.0,-current_version,1.0,-install_name,/usr/local/lib/libPaulSDL_image.1.dylib,-arch,i386
SDL_image = PaulSDL_image

all: $(SDL_image)

$(SDL_image): sdl_image.c
	gcc $(CFLAGS) -Wl,$(LDFLAGS) -o $@ $^

clean:
	rm -f -- $(SDL_image) *.sibling *.lreg
