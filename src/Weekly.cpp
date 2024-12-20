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
#include <sstream>
#include <memory>

#include "daterem.hpp"
#include "Log.hpp"

extern std::vector<std::unique_ptr<daterem::Event>> events;
extern const time_t now;
extern const tm *ltm;


daterem::Weekly::Weekly(const char* day, const char* t, const char* des) : Event(t, des)
{
    bool exitStat = false;
    
    if (!std::isdigit(day[0]) || std::stoi(day) > 6) exitStat = true;
    else _wDay = static_cast<WeekDay>(std::stoi(day));

    if (exitStat) print(L_ERROR, "Wrong date format");
}

daterem::Weekly::Weekly(std::string& day, std::string& t, std::string& des) : Event(t, des)
{
    bool exitStat = false;
    
    if (!std::isdigit(day[0]) || std::stoi(day) > 6) exitStat = true;
    else _wDay = static_cast<WeekDay>(std::stoi(day));

    if (exitStat) print(L_ERROR, "Wrong date format");
}




std::string daterem::Weekly::GetData() const
{
    static const std::string days[]
    {
        "Sunday",
        "Monday",
        "Tuesday",
        "Wednesday",
        "Thursday",
        "Friday",
        "Saturday"
    };
    std::ostringstream oss;
    oss << std::left << std::setw(15) << std::setfill(' ') << days[_wDay]
    << std::setw(25) << _title << _description << std::endl;
    return oss.str();
}


void daterem::Weekly::Save() const
{
    file.open(DATA_FILE, std::ios::out | std::ios::app);
    if (!file.good()){
        file.close();
        print(L_ERROR, "Can not save, file inaccesible");
    }
    else
    {
        file << _wDay;
        file << ';' << _title;
        file << ';' << _description << ";\n";
        file.close();
    }
}


void daterem::Weekly::Check() const
{

    if (_wDay == ltm->tm_wday)
    {
        Event::_anyEvent = true;
        std::cout << std::left << std::setw(25) << std::setfill(' ') << _title << _description << std::endl;
    }
}


void daterem::Weekly::GetSavedEvents()
{
   file.open(DATA_FILE, std::ios::in);
    if (!file.good())
        print(L_ERROR, "Cannot open the data file, create a new reminder first");


    std::string weekDay, title, desc, lineEnd;
    while (std::getline(file, weekDay, ';'))
    {
        std::getline(file, title, ';');
        std::getline(file, desc, ';');
        std::getline(file, lineEnd);
        events.emplace_back(new Weekly{weekDay, title, desc});
    }
    
    file.close();
}