CXX=g++
CXXFLAGS=-std=c++17
LDLIBS=-lSDL2
# DEPS =
OBJS = chip8.o renderer.o main.o

chipndale: $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDLIBS)

chip8.o: chip8.cpp chip8.hpp
	$(CXX) chip8.cpp -c $(CXXFLAGS)

renderer.o: renderer.cpp renderer.hpp
	$(CXX) renderer.cpp -c $(CXXFLAGS)

main.o: main.cpp chip8.hpp renderer.hpp
	$(CXX) main.cpp -c $(CXXFLAGS)

clean:
	rm $(OBJS) chipndale
