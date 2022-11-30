#include <iostream>
#include <vector>
#include <windows.h>
#include <string>
#include <fstream>
#include <ctime>
#include "Event.hpp"

namespace dr = date_rem;

std::vector < dr::Event* > s;

int main(int argc, char *argv[])
{
    SetConsoleCP( 1250 );
    SetConsoleOutputCP( 1250 );
    setlocale( LC_ALL, "1250" );

    for (int i = 1; i < argc; i++)
    {
        if (std::string(argv[i]) == "--help" || std::string(argv[i]) == "-h")
        {
            dr::ShowHelp();
            return EXIT_SUCCESS;
        }

        else if (std::string(argv[i]) == "--version" || std::string(argv[i]) == "-v")
        {
            std::cout << "daterem 1.0";
            return EXIT_SUCCESS;
        }

        else if (std::string(argv[i]) == "--list" || std::string(argv[i]) == "-l")
        {
            dr::GetSavedEvents();
            dr::ListAllEvents();
            return EXIT_SUCCESS;
        }

        else if (std::string(argv[i]) == "--check" || std::string(argv[i]) == "-c")
        {
            dr::GetSavedEvents();
            dr::CheckEvents();
            return EXIT_SUCCESS;
        }

        else if (std::string(argv[i]) == "--new" || std::string(argv[i]) == "-n")
        {
            s.push_back(new dr::Event(std::string(argv[i + 1]), argv[i + 2], argv[i + 3]));
            if (argc > 5 && (std::string(argv[i - 1]) == "-e" || std::string(argv[i + 4]) == "-e")) s.back()->SetToEveryYearEvent();
            s.back()->Save();
            return EXIT_SUCCESS;
        }

        else if (std::string(argv[i]) == "--delete")
        {
            dr::GetSavedEvents();
            dr::DeleteEvent(dr::CheckEventNr(std::stoi(argv[i + 1])));
            dr::SaveAllEvents();
            return EXIT_SUCCESS;
        }

        else
        {
            if (!(std::string(argv[i]) == "-e" && (std::string(argv[i + 1]) == "--new" || std::string(argv[i + 1]) == "-n")))
            {
                SetConsoleTextAttribute(dr::hOut, 12);
                std::clog << "Error! No such argument" << std::endl;
                SetConsoleTextAttribute(dr::hOut, 7);
                dr::ShowHelp();
                return EXIT_FAILURE;
            }
        }
    }
    return EXIT_SUCCESS;
}

void dr::ShowHelp()
{
    std::string line;
    std::ifstream help("help.txt");
    if (!help.good()) {
        exit(EXIT_FAILURE);
    }
    while(getline(help, line)){
        std::cout << line << std::endl;
    }
    help.close();
}

void dr::ListAllEvents() {
    for (int i = 0; i < dr::Event::objCount; i++) {
        s[i]->ShowData(i + 1);
    }
}

unsigned short dr::CheckEventNr(int nr) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (nr > Event::objCount || nr == 0) {
        SetConsoleTextAttribute(hOut, 12);
        std::clog << "Error! No such event" << std::endl;
        exit(EXIT_FAILURE);
    }
    else {
        return nr - 1;
    }
}

void dr::SaveAllEvents() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    rem.open(REMINDERS, std::ios::out | std::ios::trunc);
    if (!rem.good()){
        SetConsoleTextAttribute(hOut, 12);
        std::clog << "Error! Cannot open the save file, check permission settings in the instalation directory.";
        SetConsoleTextAttribute(hOut, 7);
        return;
    }
    rem.close();
    for (int i = 0; i < dr::Event::objCount; i++) {
        s[i]->Save();
    }
}

void dr::DeleteEvent(unsigned short n) {
    delete s[n];
    s.erase(s.begin() + n);
}

void dr::GetSavedEvents() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::string line;
    int numOfLines = 0;
    rem.open(REMINDERS, std::ios::in);
    if (!rem.good()){
        SetConsoleTextAttribute(hOut, 12);
        std::clog << "Error! Cannot open the save file, check permission settings in the instalation directory." << std::endl;
        std::clog << "Also after the instalation there are no reminders, create one.";
        SetConsoleTextAttribute(hOut, 7);
        return;
    }
    while (getline(rem, line)) {
        if (!line.empty()) numOfLines++;
    }
    rem.close();
    // std::cout << numOfLines;

    for (int i = 0; i < (numOfLines / 6); i++) {
        s.push_back(new dr::Event);
    }
}

void dr::CheckEvents()
{
    std::cout << "[" << dr::GetLocalDate() << "]" << std::endl << std::endl;
    bool even = false;
    for (int i = 0; i < dr::Event::objCount; i++)
    {
        s[i]->Check(&even);
    }

    if (!dr::Event::anyEvent)
        std::cout << "No reminders scheduled for today";
}

std::string dr::GetLocalDate()
{
    std::string date;
    if (dr::ltm->tm_mday <= 9)
        date = "0" + std::to_string(dr::ltm->tm_mday) + ".";
    else
        date = std::to_string(dr::ltm->tm_mday) + ".";

    if (dr::ltm->tm_mon + 1 <= 9)
        date += "0" + std::to_string(dr::ltm->tm_mon + 1);
    else
        date += std::to_string(dr::ltm->tm_mon + 1);

    date += "." + std::to_string(dr::ltm->tm_year + 1900);

    return date;
}