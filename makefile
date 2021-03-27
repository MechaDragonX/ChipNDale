CC=g++
# CFLAGS=
# DEPS =
OBJS = chip8.o

chipndale: $(OBJS)
	$(CC) -o $@ $^

chip8.o: chip.8.cpp chip8.hpp
	$(CC) -c chip8.cpp

clean:
	rm $(OBJS) chipndale
