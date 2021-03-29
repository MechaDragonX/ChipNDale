CXX=g++
CXXFLAGS=-std=c++17
LDLIBS=-lSDL2
OBJS=obj/chip8.o obj/renderer.o obj/main.o
BIN=chipndale
OBJDIR=obj
BINDIR=bin

chipndale: $(OBJS)
	$(CXX) -o $(BINDIR)/$@ $^ $(CXXFLAGS) $(LDLIBS)

obj/chip8.o: chip8.cpp chip8.hpp
	$(CXX) -c chip8.cpp -o $(OBJDIR)/chip8.o $(CXXFLAGS) 

obj/renderer.o: renderer.cpp renderer.hpp
	$(CXX) -c renderer.cpp -o $(OBJDIR)/renderer.o $(CXXFLAGS)

obj/main.o: main.cpp chip8.hpp renderer.hpp
	$(CXX) -c main.cpp -o $(OBJDIR)/main.o $(CXXFLAGS)

setup:
	mkdir -p $(OBJDIR) $(BINDIR)

clean:
	rm $(OBJS) $(BINDIR)/$(BIN)
