#include <iostream>
#include <filesystem>
#include <algorithm>
#include <fstream>
#include <string>

#include "logo.h"
#include "console.h"
#include "template_makefile_genorator.h"

void MakeProgramFiles(std::string name, int type, int version)
{
    std::string project_path = "./" + name;
    std::filesystem::create_directory(project_path);
    std::filesystem::create_directory(project_path + "/src");
    std::filesystem::create_directory(project_path + "/lib");
    std::filesystem::create_directory(project_path + "/include");

    if (type == 0)
    {
        std::ofstream ofs_main(project_path + "/src/main.cpp");
        ofs_main << 
R"(#include <iostream>

int main(int argc, char *argv[])
{
    std::cout << "it works" << std::endl;
    return 0;
})";
        ofs_main.close();
    }
    else if (type == 1)
    {
        std::string nameupper = name;
        std::transform(nameupper.begin(), nameupper.end(), nameupper.begin(), ::toupper);
        std::ofstream ofs_main(project_path + "/src/" + name + ".h");
        ofs_main << 
R"(#ifndef )" + nameupper + R"(_HPP
#define )" + nameupper + R"(_HPP

#ifdef __cplusplus
    extern "C" {
#endif

#ifdef BUILD_DLL
    #define )" + nameupper + R"( __declspec(dllexport)
#else
    #define )" + nameupper + R"( __declspec(dllimport)
#endif

// functions here

#ifdef __cplusplus
    }
#endif

// or here

#endif)";

        ofs_main.close();

        std::ofstream ofs_main1(project_path + "/src/" + name +".cpp");
        ofs_main1 << 
R"(#include ")" + name + R"(.h"
)";
        ofs_main1.close();
    }


    std::ofstream ofs_make(project_path + "/makefile");

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

    if (type == 0)
        ofs_make << basicMakefile(name, versionName);
    else if (type == 1)
        ofs_make << dllMakefile(name, versionName);

    ofs_make.close();
}

int main(int argc, char *argv[])
{
    Console console;

    console.print(CPPG::logo, COLOR::BLUE);
    console.print(CPPG::title, COLOR::GREEN);

    console.print("Enter project name", COLOR::WHITE);
    std::string name;
    std::getline(std::cin, name);

    std::string projectOptions[2] = { "Executable", "DLL" };
    int result = console.menu(2, projectOptions, "Project Type?");

    std::string versionOptions[6] = { "C++98", "C++03", "C++11", "C++14", "C++17", "C++20" };
    int version = console.menu(6, versionOptions, "C++ Version?");

    switch (result)
    {
    case 0:
        console.print("Making Executable project...", COLOR::WHITE);
        MakeProgramFiles(name, result, version);
        break;
    case 1:
        console.print("Making DLL project...", COLOR::WHITE);
        MakeProgramFiles(name, result, version);
        break;
    
    default:
        break;
    }

    return EXIT_SUCCESS;
}