TARGET = prog
LIBS = -pthread
CPP = g++
CPPFLAGS = -std=c++11 -pthread
#-g -Wall

.PHONY: default all clean

INCLUDEPATH += ../include
INCLUDEPATHARG=$(foreach d, $(INCLUDEPATH), -I $d)

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.h)

#the -c flag means "compile only"
%.o: %.cpp $(HEADERS)
	$(CPP) $(CPPFLAGS) $(INCLUDEPATHARG) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

#the -o flag means link
$(TARGET): $(OBJECTS)
	$(CPP) $(OBJECTS) $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)