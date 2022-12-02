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
#include "Event.hpp"

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
        if (currArg == "--help" || currArg == "-h")
        {
            if (argc != 2) dr::ArgErr();

            dr::ShowHelp();
            return EXIT_SUCCESS;
        }

        else if (currArg == "--version" || currArg == "-v")
        {
            if (argc != 2) dr::ArgErr();

            std::cout << "daterem 1.0.0\nCopyright (C) 2022 Paweł Rapacz\nThis program comes with ABSOLUTELY NO WARRANTY;\nThis is free software, and you are welcome to redistribute it\nunder certain conditions.";
            return EXIT_SUCCESS;
        }

        else if (currArg == "--list" || currArg == "-l")
        {
            if (argc != 2) dr::ArgErr();

            dr::GetSavedEvents();
            dr::ListAllEvents();
            return EXIT_SUCCESS;
        }

        else if (currArg == "--check" || currArg == "-c")
        {
            if (argc != 2) dr::ArgErr();

            dr::GetSavedEvents();
            dr::CheckEvents();
            return EXIT_SUCCESS;
        }

        else if (currArg == "--new" || currArg == "-n")
        {
            if (argc < 5 || argc > 7) dr::ArgErr();

            s.push_back(new dr::Event(std::string(argv[i + 1]), argv[i + 2], argv[i + 3]));

            /**
             * TODO: -e -b proper handling
             * ERROR: -e -b don't work at all
            */

            // if (argc > 5)
            // {
            //     if (
            //     std::string(argv[i - 1]) == "-e" ||
            //     std::string(argv[i - 2]) == "-e" ||
            //     std::string(argv[i + 4]) == "-e" ||
            //     std::string(argv[i + 5]) == "-e"
            //     ) 
            //         s.back()->SetToEveryYearEvent();

            //     if (
            //     std::string(argv[i - 1]) == "-b" ||
            //     std::string(argv[i - 2]) == "-b" ||
            //     std::string(argv[i + 4]) == "-b" ||
            //     std::string(argv[i + 5]) == "-b"
            //     ) 
            //         s.back()->SetToRemBefore();
                
            //     dr::ArgErr();
            // }

            s.back()->Save();
            return EXIT_SUCCESS;
        }

        else if (currArg == "--delete")
        {
            if (argc != 3) dr::ArgErr();

            dr::GetSavedEvents();
            dr::DeleteEvent(dr::CheckEventNr(std::stoi(argv[i + 1])));
            dr::SaveAllEvents();
            return EXIT_SUCCESS;
        }

        else if (currArg == "--delete-outdated" || currArg == "-o")
        {
            if (argc != 2) dr::ArgErr();

            dr::GetSavedEvents();
            dr::DeleteOutOfDate();
            dr::SaveAllEvents();
            return EXIT_SUCCESS;
        }

        if (!( 
        argc <= 6 && 
        (currArg == "-e" || currArg == "-b") &&
        (
            ( std::string(argv[i + 1]) == "--new" || std::string(argv[i + 1]) == "-n") ||
            ( std::string(argv[i + 2]) == "--new" || std::string(argv[i + 2]) == "-n")
        )
        )) dr::ArgErr();
    }
}

void dr::ArgErr()
{
    SetConsoleTextAttribute(hOut, 12);
    std::clog << "Error! Incorrect syntax" << std::endl;
    std::clog << "Try 'daterem --help' for more information.";
    exit(EXIT_FAILURE);
}

void dr::ShowHelp()
{
    std::string line;
    std::ifstream help("../src/help.txt");
    if (!help.good()) {
        exit(EXIT_FAILURE);
    }
    while(getline(help, line)){
        std::cout << line << std::endl;
    }
    help.close();
}

void dr::ListAllEvents() {
    for (int i = 0; i < Event::objCount; i++) {
        s[i]->ShowData(i + 1);
    }
}

unsigned short dr::CheckEventNr(int nr) {
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
    rem.open(REMINDERS, std::ios::out | std::ios::trunc);
    if (!rem.good()){
        SetConsoleTextAttribute(hOut, 12);
        std::clog << "Error! Cannot open the save file, check permission settings in the instalation directory.";
        SetConsoleTextAttribute(hOut, 7);
        exit(EXIT_FAILURE);
    }
    rem.close();
    for (int i = 0; i < Event::objCount; i++) {
        s[i]->Save();
    }
}

void dr::DeleteEvent(unsigned short n) {
    delete s[n];
    s.erase(s.begin() + n);
}

void dr::GetSavedEvents() {
    std::string line;
    int numOfLines = 0;
    rem.open(REMINDERS, std::ios::in);
    if (!rem.good()){
        SetConsoleTextAttribute(hOut, 12);
        std::clog << "Error! Cannot open the save file, check permission settings in the instalation directory." << std::endl;
        std::clog << "Also after the instalation there are no reminders, create one.";
        SetConsoleTextAttribute(hOut, 7);
        exit(EXIT_FAILURE);
    }
    while (getline(rem, line)) {
        if (!line.empty()) numOfLines++;
    }
    rem.close();
    // std::cout << numOfLines;

    for (int i = 0; i < (numOfLines / 7); i++) {
        s.push_back(new Event);
    }
}

void dr::CheckEvents()
{
    std::cout << "[" << GetLocalDate() << "]" << std::endl << std::endl;
    bool even = false;
    for (int i = 0; i < Event::objCount; i++)
    {
        s[i]->Check(&even);
    }

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
    {
        if (s[i]->CheckOutOfDate()) iter.push_back(i);
    }
    
    for (int i = 0; i < iter.size(); i++)
    {
        DeleteEvent(iter[i] - i);
    }

    SetConsoleTextAttribute(hOut, 7);
    std::cout << "Deleted: " << iter.size() << " reminders";
    iter.clear();
}