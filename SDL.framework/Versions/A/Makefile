CFLAGS = -pthread -dynamiclib -arch i386 -Wall $(shell pkg-config SDL_image --cflags)
LDFLAGS = -headerpad_max_install_names,-undefined,dynamic_lookup,-compatibility_version,1.0,-current_version,1.0,-install_name,/usr/local/lib/libPaulSDL.1.dylib,-arch,i386
SDL = PaulSDL

all: $(SDL)

$(SDL): sdl.c
	gcc $(CFLAGS) -Wl,$(LDFLAGS) -o $@ $^

clean:
	rm -f -- $(SDL) test *.sibling *.lreg
