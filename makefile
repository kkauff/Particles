INCFLAGS  = -I /Users/eitak/Dropbox/School/graphics/vecmath
INCFLAGS += -I /opt/local/include/


LINKFLAGS = -L/opt/local/lib -lglut -lGL -lGLU
LINKFLAGS += -L/Users/eitak/Dropbox/School/graphics/vecmath -lvecmath

#CFLAGS    = -O2 -Wall -ansi #optimized compiler commands
CFLAGS    = -g -Wall -Wextra -Wno-unused -ansi -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC #debugging compiler commands
CC        = g++ 
SRCS      = integrator.cpp particleSystem.cpp simpleSystem.cpp camera.cpp main.cpp 
OBJS      = $(SRCS:.cpp=.o)
PROG      = a3

all: $(SRCS) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LINKFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

depend:
	makedepend $(INCFLAGS) -Y $(SRCS)

clean:
	rm $(OBJS) $(PROG)