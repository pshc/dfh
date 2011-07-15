all:
	make server &
	make client

client:
	coffee -o www/ -cw client.coffee

server: build
	coffee -o build/ -cw server.coffee

build:
	mkdir build

run:
	node build/server.js

clean:
	rm -rf -- www/client.js build

.PHONY: client server run clean
