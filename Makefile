CXX = em++

CXXFLAGS = -g  --preload-file res --use-preload-plugins

OBJS = main.o video.o

TARGET = ufiji.html

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(CXXFLAGS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
	rm *~
