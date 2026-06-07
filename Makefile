CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -Werror -g
CPPFLAGS := -Iinclude

BUILD_DIR := build
LIB_OBJ := $(BUILD_DIR)/mininpu.o
TOOL := tools/mininpu_run
DEMO := examples/add_relu_demo
TEST := $(BUILD_DIR)/test_mininpu_basic

.PHONY: all test clean

all: $(TOOL) $(DEMO)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(LIB_OBJ): src/mininpu.c include/mininpu.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(TOOL): tools/mininpu_run.c $(LIB_OBJ)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

$(DEMO): examples/add_relu_demo.c $(LIB_OBJ)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

$(TEST): tests/test_mininpu_basic.c $(LIB_OBJ)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

test: $(TEST)
	./$(TEST)

clean:
	rm -rf $(BUILD_DIR) $(TOOL) $(DEMO)
