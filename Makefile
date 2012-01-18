CXX=g++
LD=g++

#DEBUG_FLAGS=-ggdb

CXXFLAGS=-fPIC -Wall -pedantic $(INCDIR) $(DEBUG_FLAGS) \
         -Wpointer-arith -Wcast-qual \
         -Wcast-align -Wsign-compare -Wfloat-equal \
         -D_GNU_SOURCE -DLINUX -pedantic

LDFLAGS=-lboost_program_options

LDCMD=$(LD) $(LDFLAGS)
CXXCMD=$(CXX) -c $(CXXFLAGS)

.PHONY: clean all test
all: check-line-length

OBJS=line-length.o

# generate dependency targets
DEPS = $(OBJS:%.o=%.depends)

check-line-length: $(OBJS)
	$(LDCMD) -o $@ $(OBJS)

clean:
	rm -rf *.o check-line-length *.orig


# pattern rules for object files
%.o: %.cpp
	$(CXXCMD) $< -o $@

# pattern rules for generating dependency files
%.depends: %.cpp
	$(CXX) -MM $(CXXFLAGS) $< > $@

%.depends: %.c
	$(CC) -MM $(CFLAGS) $< > $@



