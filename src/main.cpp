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
#include <typeinfo>
#include <windows.h>
#include "headers/daterem.hpp"
#include "headers/LOG.hpp"

namespace dr = daterem;

std::vector < dr::Event* > s;

int main(int argc, char *argv[])
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

    for (int i = 1; i < argc; i++)
    {
        std::string currArg = argv[i];
        if ((currArg == "--help" || currArg == "-h") && argc == 2)
        {
            dr::ShowHelp( argv[0] );
            return EXIT_SUCCESS;
        }

        else if ((currArg == "--version" || currArg == "-v") && argc == 2)
        {
            Log.print(L_NONE, "daterem 2.0.0\nCopyright (C) 2022 Paweł Rapacz\nThis program comes with ABSOLUTELY NO WARRANTY;\nThis is free software, and you are welcome to redistribute it under certain conditions.");
            return EXIT_SUCCESS;
        }

        else if ((currArg == "--list" || currArg == "-l") && argc == 2)
        {
            dr::GetAllSavedEvents();
            dr::ListAllEvents();
            return EXIT_SUCCESS;
        }

        else if ((currArg == "--check" || currArg == "-c") && argc == 2)
        {
            dr::GetAllSavedEvents();
            dr::CheckEvents();
            return EXIT_SUCCESS;
        }

        else if (currArg == "-e" && argc == 3)
        {
            dr::Specified::GetSavedEvents();

            int eventNumber = dr::CheckEventNr( argv[i + 1] );
            if (typeid( s[eventNumber] ) == typeid(dr::Specified))
            {
                dr::Specified::insts[eventNumber]->SetEveryYearEvent();
            }
            else Log.print(L_ERROR, "Ony reminders with specified date can be modified with this option");

            dr::SaveAllEvents();
            return EXIT_SUCCESS;
        }

        else if (currArg == "-b" && argc == 3)
        {
            dr::Specified::GetSavedEvents();

            int eventNumber = dr::CheckEventNr( argv[i + 1] );
            if (typeid( s[eventNumber] ) == typeid(dr::Specified))
            {
                dr::Specified::insts[eventNumber]->SetRemBefore();
            }
            else Log.print(L_ERROR, "Ony reminders with specified date can be modified with this option");

            dr::SaveAllEvents();
            return EXIT_SUCCESS;
        }

        else if ((currArg == "--new" || currArg == "-n") && argc == 4)
        {
            dr::EveryDay rem(std::string(argv[i + 1]), argv[i + 2]);

            rem.Save();
            Log.print(L_NONE, "Successfully created new reimnder");
            return EXIT_SUCCESS;
        }

        else if ((currArg == "--new" || currArg == "-n") && (argc >= 5 && argc <= 7))
        {


            if (argc == 5 && dr::Weekly::CheckIfWDay(argv[i + 1]))
            {
                dr::Weekly rem(std::string(argv[i + 1]), argv[i + 2], argv[i + 3]);
                rem.Save();
            }
            else
            {
                dr::Specified rem(std::string(argv[i + 1]), argv[i + 2], argv[i + 3]);

                if (argc > 5)
                {
                    for (int j = 1; j < argc; j++)
                    {
                        if (std::string(argv[j]) == "-e") rem.SetEveryYearEvent();
                        else if (std::string(argv[j]) == "-b") rem.SetRemBefore();
                        else if (!(argv[j] == argv[i] || argv[j] == argv[i + 1] || argv[j] == argv[i + 2] || argv[j] == argv[i + 3])) dr::ArgErr();
                    }
                }
                rem.Save();
            }

            Log.print(L_NONE, "Successfully created new reimnder");
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
            dr::Specified::GetSavedEvents();
            dr::Specified::DeleteOutOfDate();
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
            Log.print(L_ERROR, "Incorrect syntax\nTry \'daterem --help\' for more information.");
    }
}