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
    for (int i = 0; i < num.length(); i++)
        if (!isdigit(num[i])) dr::ArgErr();
    
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
    rem.open(REMINDERS, std::ios::out | std::ios::trunc);
    if (!rem.good()){
        SetConsoleTextAttribute(hOut, 12);
        std::clog << "Error! Data file not found.";
        SetConsoleTextAttribute(hOut, 7);
        exit(EXIT_FAILURE);
    }
    rem.close();

    for (int i = 0; i < Event::objCount; i++)
        s[i]->Save();
}



void dr::DeleteEvent(unsigned short n) {
    delete s[n];
    s.erase(s.begin() + n);
}

/**
 * ! need fix \/
*/

void dr::GetSavedEvents()
{
    std::string line;
    int numOfLines = 0;
    rem.open(REMINDERS, std::ios::in);
    if (!rem.good())
    {
        SetConsoleTextAttribute(hOut, 12);
        std::clog << "Error! Cannot open the data file, create a new reminder first." << std::endl;
        SetConsoleTextAttribute(hOut, 7);
        exit(EXIT_FAILURE);
    }

    while (getline(rem, line))
        if (!line.empty()) numOfLines++;
    
    rem.close();
    // std::cout << numOfLines;

    // for (int i = 0; i < (numOfLines / 7); i++)
    //     s.push_back(new Event);
}



void dr::CheckEvents()
{
    std::cout << "[" << GetLocalDate() << "]" << std::endl << std::endl;
    for (int i = 0; i < Event::objCount; i++)
        s[i]->Check();


    if (!Event::anyEvent)
        std::cout << "No reminders scheduled for today";
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
    if (!fs::is_directory(REMINDERS.parent_path()) || !fs::is_regular_file(REMINDERS))
    {
        fs::remove_all(REMINDERS.parent_path());
        fs::create_directory(REMINDERS.parent_path());
        std::ofstream(REMINDERS).write(0, 0);
    }
}