CFLAGS = -pthread -dynamiclib -arch i386 -Wall $(shell pkg-config SDL_image --cflags)
LIBS = -L/opt/i386/lib -lzmq
LDFLAGS = -headerpad_max_install_names,-undefined,dynamic_lookup,-compatibility_version,1.0,-current_version,1.0,-install_name,/usr/local/lib/libPaulSDL.1.dylib,-arch,i386
SDL = PaulSDL

all: $(SDL) front

$(SDL): conn.c sdl.c
	gcc $(CFLAGS) $(LIBS) -Wl,$(LDFLAGS) -o $@ $^

front: front.c
	gcc -lzmq -o $@ $^

clean:
	rm -f -- $(SDL) test *.sibling *.lreg
