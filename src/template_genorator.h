#define TEMPLATE_GENORATOR_H
#ifdef TEMPLATE_GENORATOR_H

#include <string>

std::string emptyMainFile()
{
	return 
R"(#include <iostream>

int main(int argc, char *argv[])
{
    std::cout << "it works" << std::endl;
    return 0;
})";
}

std::string emptyDllHeadderFile(std::string name)
{
	return
R"(#ifndef )" + name + R"(_HPP
#define )" + name + R"(_HPP

#ifdef __cplusplus
    extern "C" {
#endif

#ifdef BUILD_DLL
    #define )" + name + R"( __declspec(dllexport)
#else
    #define )" + name + R"( __declspec(dllimport)
#endif

// functions here

#ifdef __cplusplus
    }
#endif

// or here

#endif)";
}

std::string emptyDllMainFile(std::string name)
{
	return
R"(#include ")" + name + R"(.h"
)";
}

std::string basicMakefile(std::string name, std::string version)
{
    return
R"(CXX = g++
BUILD ?= DEBUG
MAIN = bin/)" + name + R"(.exe
CFLAGS = )" + version + R"(
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

std::string dllMakefile(std::string name, std::string version)
{
    return
R"(CXX = g++
BUILD ?= DEBUG
MAIN = bin/)" + name + R"(.dll
OTHER = bin/)" + name + R"(dll.lib
CFLAGS = )" + version + R"( -shared
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

void MakeProgramFiles(std::string name, int type, int version)
{
    std::string project_path = "./" + name;
    std::filesystem::create_directory(project_path);
    std::filesystem::create_directory(project_path + "/src");
    std::filesystem::create_directory(project_path + "/lib");
    std::filesystem::create_directory(project_path + "/include");

    if (type == 0)
    {
        std::ofstream mainFile(project_path + "/src/main.cpp");
        mainFile << emptyMainFile();
        mainFile.close();
    }
    else if (type == 1)
    {
        std::string nameupper = name;
        std::transform(nameupper.begin(), nameupper.end(), nameupper.begin(), ::toupper);

        std::ofstream headderDllFile(project_path + "/src/" + name + ".h");
        headderDllFile << emptyDllHeadderFile(nameupper);
        headderDllFile.close();

        std::ofstream mainDllFile(project_path + "/src/" + name +".cpp");
        mainDllFile << emptyDllMainFile(name);
        mainDllFile.close();
    }


    std::string versionName;

    switch (version)
    {
    case 0:
        versionName = "-std=c++98";
        break;
    case 1:
        versionName = "-std=c++03";
        break;
    case 2:
        versionName = "-std=c++11";
        break;
    case 3:
        versionName = "-std=c++14";
        break;
    case 4:
        versionName = "-std=c++17";
        break;
    case 5:
        versionName = "-std=c++20";
        break;
    }

    std::ofstream makeFile(project_path + "/makefile");

    if (type == 0)
        makeFile << basicMakefile(name, versionName);
    else if (type == 1)
        makeFile << dllMakefile(name, versionName);

    makeFile.close();
}

#endif