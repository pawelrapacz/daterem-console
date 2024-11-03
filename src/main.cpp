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


#include <vector>
#include <windows.h>
#include <cstring>
#include <memory>

#include "headers/daterem.hpp"
#include "headers/Log.hpp"

namespace dr = daterem;

std::vector<std::unique_ptr<dr::Event>> events;

int main(const int argc, const char* argv[])
{
    SetConsoleCP( 65001 );
    SetConsoleOutputCP( 65001 );
    setlocale( LC_ALL, "65001" );

    if (argc == 1)
    {
        dr::GetAllSavedEvents();
        dr::CheckEvents();
        return EXIT_SUCCESS;
    }

    std::string currArg;
    for (int i = 1; i < argc; i++)
    {
        currArg = argv[i];
        if ((currArg == "--help" || currArg == "-h") && argc == 2)
        {
            dr::ShowHelp( argv[0] );
            return EXIT_SUCCESS;
        }

        else if ((currArg == "--version" || currArg == "-v") && argc == 2)
        {
            print(L_NONE, 
R"(daterem 2.0.1
Copyright (C) 2022 Paweł Rapacz
This program comes with ABSOLUTELY NO WARRANTY;
This is free software, and you are welcome to redistribute it under certain conditions.)");
            return EXIT_SUCCESS;
        }

        else if ((currArg == "--list" || currArg == "-l") && argc == 2)
        {
            dr::GetAllSavedEvents();
            dr::ListAllEvents();
            return EXIT_SUCCESS;
        }

        else if (((currArg == "--check" || currArg == "-c") && argc == 2) || argc == 1)
        {
            dr::GetAllSavedEvents();
            dr::CheckEvents();
            return EXIT_SUCCESS;
        }

        else if (currArg == "-e" && argc == 3)
        {
            // Loading only specified events
            dr::Specified::GetSavedEvents();
            dr::Specified::insts[dr::CheckEventNr(argv[i + 1])]->SetEveryYearEvent();
            dr::Specified::SaveEvents();
            print(L_NONE, "Successfully changed reimnder");
            return EXIT_SUCCESS;
        }

        else if (currArg == "-b" && argc == 3)
        {
            // Loading only specified events
            dr::Specified::GetSavedEvents();
            dr::Specified::insts[dr::CheckEventNr(argv[i + 1])]->SetRemBefore();
            dr::Specified::SaveEvents();
            print(L_NONE, "Successfully changed reimnder");
            return EXIT_SUCCESS;
        }

        else if ((currArg == "--new" || currArg == "-n") && argc == 4)
        {
            dr::EveryDay rem(argv[i + 1], argv[i + 2]);
            rem.Save();
            print(L_NONE, "Successfully created new reimnder");
            return EXIT_SUCCESS;
        }

        else if ((currArg == "--new" || currArg == "-n") && (argc >= 5 && argc <= 7))
        {
            if (argc == 5 && strlen(argv[i + 1]) == 1)
            {
                dr::Weekly rem(argv[2], argv[i + 2], argv[i + 3]);
                rem.Save();
            }
            else
            {
                dr::Specified rem(argv[i + 1], argv[i + 2], argv[i + 3]);

                if (argc > 5)
                {
                    for (int j = 1; j < argc; j++)
                    {
                        if (strcmp(argv[j], "-e") == 0) rem.SetEveryYearEvent();
                        else if (strcmp(argv[j], "-b") == 0) rem.SetRemBefore();
                        else if (!(argv[j] == argv[i] || argv[j] == argv[i + 1] || argv[j] == argv[i + 2] || argv[j] == argv[i + 3]))
                            print(L_ERROR, "Incorrect syntax\nTry \'daterem --help\' for more information.");
                    }
                }
                rem.Save();
            }

            print(L_NONE, "Successfully created new reimnder");
            return EXIT_SUCCESS;
        }

        else if (currArg == "--delete" && argc == 3)
        {
            dr::GetAllSavedEvents();
            dr::DeleteEvent( dr::CheckEventNr(argv[i + 1]) );
            dr::SaveAllEvents();
            return EXIT_SUCCESS;
        }

        else if ((currArg == "--delete-outdated" || currArg == "-o") && argc == 2)
        {
            // Loading only specified events
            dr::Specified::GetSavedEvents();
            dr::Specified::DeleteOutOfDate();
            dr::Specified::SaveEvents();
            return EXIT_SUCCESS;
        }

        else if (!( 
        argc > 5 && 
        (currArg == "-e" || currArg == "-b") &&
        (
            strcmp(argv[i + 1], "--new") == 0 ||
            strcmp(argv[i + 2], "--new") == 0 ||
            strcmp(argv[i + 1], "-n") == 0 ||
            strcmp(argv[i + 2], "-n") == 0
        ))) 
            print(L_ERROR, "Incorrect syntax\nTry \'daterem --help\' for more information.");
    }
}