# Makefile tests, C+Perl
#
# Copyright (c) 2013 Hugo de Freitas Siqueira Sadok Menna Barreto <sadok.com.br>
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.


CXX=g++
LD=g++
CXXFLAGS=$(shell perl -MExtUtils::Embed -e ccopts) -Dusethreads -Duseithreads -Wall -Wno-write-strings #-g
LDFLAGS=$(shell perl -MExtUtils::Embed -e ldopts) -Dusethreads -Duseithreads #-g

RM=rm -vf

LIB=CPlusPerl.a
HEADER=CPlusPerl.h

EXP=example.cpp
EXP_OBJS=$(subst .cpp,.o,$(EXP))
EXP_EXEC= example1


.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

all: $(LIB) $(HEADER) $(EXP_EXEC)
	@echo "=================================================================================================="
	@echo "=== Go to the docs directory to execute the example1, the source code is also available there. ==="

$(EXP_EXEC): $(EXP_OBJS)
	$(LD) -o $(EXP_EXEC) $(EXP_OBJS) $(LIB) $(LDFLAGS)

$(LIB):
	+$(MAKE) -C ../source
	cp ../source/$(LIB) .

$(HEADER): $(LIB)
	cp ../source/$(HEADER) .

clean:
	$(RM) $(EXP_OBJS) $(EXP_EXEC) $(LIB) $(HEADER)
