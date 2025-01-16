#pragma once
#define TEMPLATE_MAKEFILE_GENORATOR_H
#ifdef TEMPLATE_MAKEFILE_GENORATOR_H

#include <string>

std::string basicMakefile(std::string name)
{
    return R"(CXX = g++
BUILD ?= DEBUG
MAIN = bin/)" + name + R"(.exe
CFLAGS = -std=c++17
LIBS =
DEFS = 

SRCS = $(wildcard src/*.cpp)
SLIBS = $(wildcard lib/*.a)
INCDIR = ./include
SLIBDIR = ./lib
OBJDIR = ./obj
BINDIR = ./bin

OBJS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(notdir $(SRCS)))

ifeq ($(BUILD),DEBUG)
	CFLAGS += -g -Wall -DDEBUG
else ifeq ($(BUILD),RELEASE)
	CFLAGS += -O2 -DNDEBUG
endif

all: $(MAIN)

$(MAIN): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CFLAGS) -o $@ $^ $(addprefix -I, $(INCDIR)) $(addprefix -L, $(SLIBDIR)) $(SLIBS) $(addprefix -D, $(DEFS)) $(LIBS)

$(OBJDIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) -c -o $@ $< $(addprefix -D, $(DEFS)) $(addprefix -I, $(INCDIR))

clean:
	rm -rf $(OBJDIR) $(MAIN)

run:
	$(MAIN)

debug: 
	@$(MAKE) BUILD=DEBUG

release:
	@$(MAKE) BUILD=RELEASE)";
}

std::string dllMakefile(std::string name)
{
    return R"(CXX = g++
BUILD ?= DEBUG
MAIN = bin/)" + name + R"(.dll
OTHER = bin/)" + name + R"(dll.lib
CFLAGS = -std=c++17 -shared
LIBS =
DEFS = BUILD_DLL

SRCS = $(wildcard src/*.cpp)
SLIBS = $(wildcard lib/*.a)
INCDIR = ./include
SLIBDIR = ./lib
OBJDIR = ./obj
BINDIR = ./bin

OBJS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(notdir $(SRCS)))

ifeq ($(BUILD),DEBUG)
	CFLAGS += -g -Wall -DDEBUG
else ifeq ($(BUILD),RELEASE)
	CFLAGS += -O2 -DNDEBUG
endif

all: $(MAIN)

$(MAIN): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CFLAGS) -Wl,--out-implib,$(OTHER) -o $@ $^ $(addprefix -I, $(INCDIR)) $(addprefix -L, $(SLIBDIR)) $(SLIBS) $(addprefix -D, $(DEFS)) $(LIBS)

$(OBJDIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) -c -o $@ $< $(addprefix -D, $(DEFS)) $(addprefix -I, $(INCDIR))

clean:
	rm -rf $(OBJDIR) $(MAIN)

debug: 
	@$(MAKE) BUILD=DEBUG

release:
	@$(MAKE) BUILD=RELEASE)";
}

#endif