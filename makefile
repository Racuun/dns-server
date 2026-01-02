BUILD_DIR = build

.PHONY: all clean project build run test


clean:
	rm -rf $(BUILD_DIR)/*

all: project
	$(MAKE) -C $(BUILD_DIR)

project:
	@mkdir -p $(BUILD_DIR)
	@cd build && cmake ..

build-lib:
	$(MAKE) -C $(BUILD_DIR) dnslib


build: project
	$(MAKE) -C $(BUILD_DIR) dns-server

run: build
	./$(BUILD_DIR)/dns-server

sudo-run: build
	sudo ./$(BUILD_DIR)/dns-server

test: project
	$(MAKE) -C $(BUILD_DIR) dns_tests
	cd $(BUILD_DIR) && ctest --output-on-failure

.PHONY: test
test:
	$ cd build && ctest --output-on-failure
