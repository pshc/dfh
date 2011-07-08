CFLAGS = -pthread -dynamiclib -arch i386 -Wall
LDFLAGS = -headerpad_max_install_names,-undefined,dynamic_lookup,-compatibility_version,1.0,-current_version,1.0,-install_name,/usr/local/lib/libfmodex.dylib,-arch,i386
FMOD = libfmodex.dylib

all: $(FMOD)

$(FMOD): fmod.c
	gcc $(CFLAGS) -Wl,$(LDFLAGS) -o $@ $^

clean:
	rm -f -- $(FMOD) *.sibling *.lreg
