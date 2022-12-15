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


#pragma once

#include "Event.hpp"
#include "Specified.hpp"
#include "Weekly.hpp"
#include "EveryDay.hpp"



#include <windows.h>
#include <string>
#include <fstream>
#include <filesystem>

namespace daterem
{
    const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    static const time_t now = time(0);
    static const tm *ltm = localtime(&now);

    // FILES
    const std::filesystem::path REMINDERS{std::filesystem::path(getenv("APPDATA")) / "daterem" / "reminders"};
    static std::fstream rem;

    // OTHER FUNCTIONS
    void GetSavedEvents();
    void SaveAllEvents();
    void ListAllEvents();
    void DeleteOutOfDate();
    void DeleteEvent(unsigned short);
    unsigned short CheckEventNr(std::string);
    void ShowHelp(char*);
    void CheckEvents();
    std::string GetLocalDate();
    void ArgErr();
    void AppDataCheckMeta(); // Checks whether users data exists, if not creates it
}