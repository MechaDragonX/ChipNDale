CXX=g++
CXXFLAGS=-std=c++17
LDLIBS=-lSDL2
# DEPS =
OBJS = chip8.o platform.o

chipndale: $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDLIBS)

chip8.o: chip8.cpp chip8.hpp
	$(CXX) -c chip8.cpp $(CXXFLAGS)

platform.o: platform.cpp platform.hpp
	$(CXX) -c platform.cpp $(CXXFLAGS)

clean:
	rm $(OBJS) chipndale
