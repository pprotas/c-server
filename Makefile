all: bin/hello_world

bin/hello_world: src/hello_world.c
	clang $< -o $@

bin:
	mkdir -p bin

clean:
	rm -rf bin

.PHONY: all clean
