# Makefile example, C+Perl

CXX=g++
LD=g++
CXXFLAGS=$(shell perl -MExtUtils::Embed -e ccopts) -Dusethreads -Duseithreads -Wall -Wno-write-strings #-g
LDFLAGS=$(shell perl -MExtUtils::Embed -e ldopts) -Dusethreads -Duseithreads #-g

RM=rm -vf

LIB=CPlusPerl.a
HEADER=CPlusPerl.h

##### EDIT THE FOLLOWING LINES #####
SRCS=example.cpp #### <--- Edit this line to list your cpp files (separate using space)
EXEC= example #### <--- Edit this line to have your final executable name
####################################

OBJS=$(subst .cpp,.o,$(SRCS))


.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

all: $(LIB) $(HEADER) $(EXEC)
	@echo "Done"

$(EXEC): $(OBJS)
	$(LD) -o $(EXEC) $(OBJS) $(LIB) $(LDFLAGS)

$(LIB):
	$(error Missing Library File: $(LIB))

$(HEADER): $(LIB)
	$(error Missing Header File: $(HEADER))

clean:
	$(RM) $(OBJS) $(EXEC) $(LIB) $(HEADER)
