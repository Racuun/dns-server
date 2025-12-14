
clean:
	rm -rf build/*

project:
	$ mkdir build || true
	$ cd build && cmake ..

.PHONY: build
build:
	make project
	$ cd build && make

run:
	$ cd build && ./dns-server

.PHONY: test
test:
	$ cd build && ctest --output-on-failure