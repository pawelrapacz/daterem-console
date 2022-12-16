#include <iostream>
#include <filesystem>
#include "headers/daterem.hpp"

namespace fs = std::filesystem;
namespace dr = daterem;

extern std::vector < daterem::Event* > s;

void dr::ArgErr()
{
    SetConsoleTextAttribute(hOut, 12);
    std::clog << "Error! Incorrect syntax" << std::endl;
    std::clog << "Try \'daterem --help\' for more information.";
    SetConsoleTextAttribute(hOut, 7);
    exit(EXIT_FAILURE);
}



void dr::ShowHelp(char* programArg)
{
    const auto helpPath{fs::absolute(programArg).parent_path() / "help.txt"};
    std::string line;
    std::ifstream help(helpPath);
    if (!help.good()) exit(EXIT_FAILURE);

    while(getline(help, line))
        std::cout << line << std::endl;
    
    help.close();
}



void dr::ListAllEvents() {
    for (int i = 0; i < Event::objCount; i++) {
        std::cout << i << s[i]->GetData();
    }
}



unsigned short dr::CheckEventNr(std::string num)
{
    for (char i : num)
        if (!isdigit(i)) ArgErr();
    
    unsigned short n = stoi(num);

    if (n > Event::objCount || n == 0) {
        SetConsoleTextAttribute(hOut, 12);
        std::clog << "Error! No such event" << std::endl;
        SetConsoleTextAttribute(hOut, 7);
        exit(EXIT_FAILURE);
    }
    else return n - 1;
}



void dr::SaveAllEvents()
{
    AppDataCheckMeta();
    std::fstream file;
    file.open(EveryDay::DATA_FILE, std::ios::out | std::ios::trunc);
    if (!file.good()){
        SetConsoleTextAttribute(hOut, 12);
        std::clog << "Error! Data file not found.";
        SetConsoleTextAttribute(hOut, 7);
        exit(EXIT_FAILURE);
    }
    file.close();

    file.open(Weekly::DATA_FILE, std::ios::out | std::ios::trunc);
    if (!file.good()){
        SetConsoleTextAttribute(hOut, 12);
        std::clog << "Error! Data file not found.";
        SetConsoleTextAttribute(hOut, 7);
        exit(EXIT_FAILURE);
    }
    file.close();

    file.open(Specified::DATA_FILE, std::ios::out | std::ios::trunc);
    if (!file.good()){
        SetConsoleTextAttribute(hOut, 12);
        std::clog << "Error! Data file not found.";
        SetConsoleTextAttribute(hOut, 7);
        exit(EXIT_FAILURE);
    }
    file.close();

    for (int i = 0; i < Event::objCount; i++)
        s[i]->Save();
}



void dr::DeleteEvent(unsigned short n) {
    delete s[n];
    s.erase(s.begin() + n);
}



void dr::GetAllSavedEvents()
{
    EveryDay::GetSavedEvents();
    Weekly::GetSavedEvents();
    Specified::GetSavedEvents();
}



void dr::CheckEvents()
{
    std::cout << "[" << GetLocalDate() << "]" << std::endl << std::endl;
    for (int i = 0; i < Event::objCount; i++)
        s[i]->Check();


    if (!Event::anyEvent) std::cout << "No reminders scheduled for today";
}



std::string dr::GetLocalDate()
{
    std::string date;
    if (ltm->tm_mday <= 9)
        date = "0" + std::to_string(ltm->tm_mday) + ".";
    else
        date = std::to_string(ltm->tm_mday) + ".";

    if (ltm->tm_mon + 1 <= 9)
        date += "0" + std::to_string(ltm->tm_mon + 1);
    else
        date += std::to_string(ltm->tm_mon + 1);

    date += "." + std::to_string(ltm->tm_year + 1900);

    return date;
}



void dr::AppDataCheckMeta()
{
    if (!fs::is_directory(EveryDay::DATA_FILE.parent_path()) || !fs::is_regular_file(EveryDay::DATA_FILE))
    {
        fs::remove_all(EveryDay::DATA_FILE.parent_path());
        fs::create_directory(EveryDay::DATA_FILE.parent_path());
        std::ofstream(EveryDay::DATA_FILE).write(0, 0);
    }

    if (!fs::is_directory(Weekly::DATA_FILE.parent_path()) || !fs::is_regular_file(Weekly::DATA_FILE))
    {
        fs::remove_all(Weekly::DATA_FILE.parent_path());
        fs::create_directory(Weekly::DATA_FILE.parent_path());
        std::ofstream(Weekly::DATA_FILE).write(0, 0);
    }

    if (!fs::is_directory(Specified::DATA_FILE.parent_path()) || !fs::is_regular_file(Specified::DATA_FILE))
    {
        fs::remove_all(Specified::DATA_FILE.parent_path());
        fs::create_directory(Specified::DATA_FILE.parent_path());
        std::ofstream(Specified::DATA_FILE).write(0, 0);
    }
}