#include <iostream>
#include <filesystem>
#include <algorithm>
#include <fstream>
#include <string>

#include "console.h"
#include "template_genorator.h"

std::string LOGO = R"(
________/\\\\\\\\\________________________________________/\\\\\\\\\\\\\_______/\\\\\\\\\\\\_
 _____/\\\////////________________________________________\/\\\/////////\\\___/\\\//////////__
  ___/\\\/________________/\\\__________/\\\_______________\/\\\_______\/\\\__/\\\_____________
   __/\\\_________________\/\\\_________\/\\\_______________\/\\\\\\\\\\\\\/__\/\\\____/\\\\\\\_
    _\/\\\______________/\\\\\\\\\\\__/\\\\\\\\\\\___________\/\\\/////////____\/\\\___\/////\\\_
     _\//\\\____________\/////\\\///__\/////\\\///____________\/\\\_____________\/\\\_______\/\\\_
      __\///\\\______________\/\\\_________\/\\\_______________\/\\\_____________\/\\\_______\/\\\_
       ____\////\\\\\\\\\_____\///__________\///________________\/\\\_____________\//\\\\\\\\\\\\/__
        _______\/////////________________________________________\///_______________\////////////____
)";

#define VERSION "0.4.4"

#ifdef DEBUG
    std::string TITLE = "C++ Project Genrator\nDEBUG: v" + 
    std::string(VERSION) + " " + std::string(__TIME__) + " " + std::string(__DATE__);
#else
    std::string TITLE = "C++ Project Genrator\nRELEASE: v" + 
    std::string(VERSION) + " "  + std::string(__TIME__) + " " + std::string(__DATE__);
#endif

int main(int argc, char *argv[])
{
    Console console;

    console.print(LOGO, COLOR::BLUE);
    console.print(TITLE, COLOR::GREEN);

    console.print("Enter project name", COLOR::WHITE);
    std::string name;
    std::getline(std::cin, name);

    std::string projectOptions[2] = { "Executable", "DLL" };
    int result = console.menu(2, projectOptions, "Project Type?");

    std::string versionOptions[6] = { "C++98", "C++03", "C++11", "C++14", "C++17", "C++20" };
    int version = console.menu(6, versionOptions, "C++ Version?");

    console.print("Making project...", COLOR::WHITE);
    MakeProgramFiles(name, result, version);

    return EXIT_SUCCESS;
}