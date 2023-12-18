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
#include <iomanip>
#include <string>
#include <filesystem>

#include "headers/daterem.hpp"
#include "headers/Log.hpp"

namespace fs = std::filesystem;
namespace dr = daterem;

const time_t now = time(0);
const tm *ltm = localtime(&now);

extern std::vector < daterem::Event* > s;


void dr::ShowHelp(const char* programArg)
{
    const std::string HELP = 
R"(daterem - create reminders

SYNOPSIS
    daterem [OPTION] [PROPERTIES]

DESCRIPTION
    Mandatory arguments to long options are mandatory for short options too.

    -h, --help
        Prints help.

    -v, --version
        Prints version information.

    -l, --list
        List all scheduled reminders.
    
    -c, --check
        Shows all reminders scheduled for the current day.
    
    -n, --new [date] [title] [description]
        Creates new reminder for given date, date format: DD.MM for every year reminder and DD.MM.YYYY for sigle reminder.
        To create weekly reminder set date to number [0 - 6] days since Sunday.
        When date not set creates everyday reminder.

    -e [number]
        Only for one time reminders

        Converts reminder with indicated number to be every year (use --list to see the numeration).
        When used with -n or --new e.g. (-e --new ...) forces created reminder to be every year, if used like this do not set the number.

    -b [number]
        Only for annual or one time reminders

        Sets two additional reminders 7 days and 2 days before specified date, if turned on turns off (use --list to see the numeration).
        When used with -n or --new, e.g. (-b --new ...) do not set the number.

    --delete [number]
        Deletes reminder with indicated number (use --list to see the numeration).

    -o, --delete-outdated
        Deletes all reminders that expired, applies only to one time reminders.)";

    std::cout << HELP;
}



void dr::ListAllEvents() {
    if (Event::objCount == 0)
    {
        print(L_INFO, "No reminders scheduled yet");
        return;
    }

    // list header
    std::cout << std::left << std::setw(6) << std::setfill(' ') << "No.";
    std::cout << std::left << std::setw(15) << std::setfill(' ') << "Date";
    std::cout << std::left << std::setw(25) << std::setfill(' ') << "Title";
    std::cout << std::left << "Description\n\n";

    // list content
    unsigned int number = 1;
    for (Event* i : s)
    {
        std::cout << std::left << std::setw(6) << number << i->GetData();
        number++;
    }
}



unsigned int dr::CheckEventNr(std::string num)
{
    for (char i : num)
        if (!isdigit(i)) print(L_ERROR, "No such event, or you can not use this option with this event");
    
    unsigned int n = stoi(num);

    if (n > Event::objCount || n == 0) print(L_ERROR, "No such event, or you can not use this option with this event");
    
    return n - 1;
}



void dr::SaveAllEvents()
{
    AppDataCheckMeta();
    std::fstream file;
    file.open(EveryDay::DATA_FILE, std::ios::out | std::ios::trunc);
    if (!file.good()) print(L_ERROR, "Can not save, file inaccesible");
    file.close();

    file.open(Weekly::DATA_FILE, std::ios::out | std::ios::trunc);
    if (!file.good()) print(L_ERROR, "Can not save, file inaccesible");
    file.close();

    file.open(Specified::DATA_FILE, std::ios::out | std::ios::trunc);
    if (!file.good()) print(L_ERROR, "Can not save, file inaccesible");
    file.close();

    for (Event* i : s)
        i->Save();
}



void dr::DeleteEvent(unsigned int n) {
    delete s[n];
    s.erase(s.begin() + n);
}



void dr::GetAllSavedEvents()
{
    AppDataCheckMeta();
    // ! ORDER IS IMPORTANT
    Specified::GetSavedEvents();
    Weekly::GetSavedEvents();
    EveryDay::GetSavedEvents();
}



void dr::CheckEvents()
{
    std::cout << "[" << GetLocalDate() << "]" << "\n\n";
    for (Event* i : s)
        i->Check();

    if (!Event::anyEvent) print(L_NONE, "No reminders scheduled for today");
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
    if (!fs::is_directory(EveryDay::DATA_FILE.parent_path()))
    {
        fs::remove_all(EveryDay::DATA_FILE.parent_path());
        fs::create_directory(EveryDay::DATA_FILE.parent_path());
    }

    if (!fs::is_regular_file(EveryDay::DATA_FILE))
        std::ofstream(EveryDay::DATA_FILE).write(0, 0);

    if (!fs::is_regular_file(Weekly::DATA_FILE))
        std::ofstream(Weekly::DATA_FILE).write(0, 0);

    if (!fs::is_regular_file(Specified::DATA_FILE))
        std::ofstream(Specified::DATA_FILE).write(0, 0);
}