CXX = g++

SDL_LIB = -L/usr/lib -lSDL -lSDL_gfx
SDL_INCLUDE = -I/usr/local/include `sdl-config --cflags --libs`

CXXFLAGS = -std=c++0x -O2 -g -fmessage-length=0 $(SDL_INCLUDE)

OBJS = main.o video.o
LIBS = $(SDL_LIB)

TARGET = plat

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
