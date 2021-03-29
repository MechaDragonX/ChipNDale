CXX = g++
CXXFLAGS = -std=c++20

LDLIBS :=
ifeq ($(OS),Windows_NT)
	LDLIBS += -lmingw32 -lSDL2main -lSDL2
else
	LDLIBS += -lSDL2
endif

OBJS = obj/chip8.o obj/renderer.o obj/main.o

TARGET :=
ifeq ($(OS),Windows_NT)
	TARGET += chipndale.exe
else
	TARGET += chipndale
endif

OBJDIR = obj
BINDIR = bin

$(TARGET): $(OBJS)
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
	rm $(OBJS) $(BINDIR)/$(TARGET)
