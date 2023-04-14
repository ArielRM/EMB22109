INCLUDE_SYSTEMC=C:/systemc-2.3.3/source/src
LIB_SYSTEMC=C:/systemc-2.3.3/eclipse/src

CXX=g++
CPPFLAGS=-std=c++14 -I$(INCLUDE_SYSTEMC) -O0 -g3 -Wall -fmessage-length=0 -MMD
LDFLAGS=-L$(LIB_SYSTEMC)
LDLIBS=-lsystemc

SRCS=main.cc draw.cc
OBJS=build/$(subst .cc,.o,$(SRCS))

.PHONY: run all

all: build/neander.exe

build/neander.exe: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

run: build/neander.exe
	build/neander.exe

build/%.o: %.cc build
	$(CXX) $(CPPFLAGS) -c -o $@ $<

build:
	mkdir build

-include build/main.d
-include build/draw.d