/**
 * daterem - a program that helps you keep track your plans and reminds
 * you your daily things.
 * Copyright (C) 2022  Paweł Rapacz
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


#include <iostream>
#include <vector>
#include <windows.h>
#include <string>
#include <fstream>
#include <ctime>
#include <filesystem>
#include "Event.hpp"

namespace fs = std::filesystem;
namespace dr = date_rem;

std::vector < dr::Event* > s;

int main(int argc, char *argv[])
{
    SetConsoleCP( 65001 );
    SetConsoleOutputCP( 65001 );
    setlocale( LC_ALL, "65001" );

    if (argc == 1)
    {
        dr::GetSavedEvents();
        dr::CheckEvents();
        return EXIT_SUCCESS;
    }

    for (int i = 1; i < argc; i++)
    {
        std::string currArg = argv[i];
        if ((currArg == "--help" || currArg == "-h") && argc == 2)
        {
            dr::ShowHelp();
            return EXIT_SUCCESS;
        }

        else if ((currArg == "--version" || currArg == "-v") && argc == 2)
        {
            std::cout << "daterem 1.0.0\nCopyright (C) 2022 Paweł Rapacz\nThis program comes with ABSOLUTELY NO WARRANTY;\nThis is free software, and you are welcome to redistribute it\nunder certain conditions.";
            return EXIT_SUCCESS;
        }

        else if ((currArg == "--list" || currArg == "-l") && argc == 2)
        {
            dr::GetSavedEvents();
            dr::ListAllEvents();
            return EXIT_SUCCESS;
        }

        else if ((currArg == "--check" || currArg == "-c") && argc == 2)
        {
            dr::GetSavedEvents();
            dr::CheckEvents();
            return EXIT_SUCCESS;
        }

        else if (currArg == "-e" && argc == 3)
        {
            dr::GetSavedEvents();
            s[ dr::CheckEventNr(argv[i + 1]) ]->SetToEveryYearEvent();
            dr::SaveAllEvents();
            return EXIT_SUCCESS;
        }

        else if (currArg == "-b" && argc == 3)
        {
            dr::GetSavedEvents();
            s[ dr::CheckEventNr(argv[i + 1]) ]->SetToRemBefore();
            dr::SaveAllEvents();
            return EXIT_SUCCESS;
        }

        else if ((currArg == "--new" || currArg == "-n") && (argc >= 5 || argc <= 7))
        {
            s.push_back(new dr::Event(std::string(argv[i + 1]), argv[i + 2], argv[i + 3]));

            if (argc > 5)
            {
                for (int j = 1; j < argc; j++)
                {
                    if (std::string(argv[j]) == "-e") s.back()->SetToEveryYearEvent();
                    else if (std::string(argv[j]) == "-b") s.back()->SetToRemBefore();
                    else if (!(argv[j] == argv[i] || argv[j] == argv[i + 1] || argv[j] == argv[i + 2] || argv[j] == argv[i + 3])) dr::ArgErr();
                }
            }

            s.back()->Save();
            std::cout << "Successfully created new reimnder.";
            return EXIT_SUCCESS;
        }

        else if (currArg == "--delete" && argc == 3)
        {
            dr::GetSavedEvents();
            dr::DeleteEvent( dr::CheckEventNr(argv[i + 1]) );
            dr::SaveAllEvents();
            return EXIT_SUCCESS;
        }

        else if ((currArg == "--delete-outdated" || currArg == "-o") && argc == 2)
        {
            dr::GetSavedEvents();
            dr::DeleteOutOfDate();
            dr::SaveAllEvents();
            return EXIT_SUCCESS;
        }

        else if (!( 
        argc > 5 && 
        (currArg == "-e" || currArg == "-b") &&
        (
            std::string(argv[i + 1]) == "--new" ||
            std::string(argv[i + 2]) == "--new" ||
            std::string(argv[i + 1]) == "-n" ||
            std::string(argv[i + 2]) == "-n"
        ))) 
            dr::ArgErr();
    }
}

void dr::ArgErr()
{
    SetConsoleTextAttribute(hOut, 12);
    std::clog << "Error! Incorrect syntax" << std::endl;
    std::clog << "Try \'daterem --help\' for more information.";
    exit(EXIT_FAILURE);
}

void dr::ShowHelp()
{
    std::string line;
    std::ifstream help("help.txt");
    if (!help.good()) exit(EXIT_FAILURE);

    while(getline(help, line))
        std::cout << line << std::endl;
    
    help.close();
}

void dr::ListAllEvents() {
    for (int i = 0; i < Event::objCount; i++) {
        s[i]->ShowData(i + 1);
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

    for (int i = 0; i < (numOfLines / 7); i++)
        s.push_back(new Event);
}

void dr::CheckEvents()
{
    std::cout << "[" << GetLocalDate() << "]" << std::endl << std::endl;
    bool even = false;
    for (int i = 0; i < Event::objCount; i++)
        s[i]->Check(&even);


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

void dr::DeleteOutOfDate()
{
    std::vector < unsigned short > iter;
    iter.reserve(10);

    for (int i = 0; i < Event::objCount; i++)
        if (s[i]->CheckOutOfDate()) iter.push_back(i);
    
    for (int i = 0; i < iter.size(); i++)
        DeleteEvent(iter[i] - i);


    SetConsoleTextAttribute(hOut, 7);
    std::cout << "Deleted: " << iter.size() << " reminders";
    iter.clear();
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