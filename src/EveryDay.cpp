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

#include "headers/daterem.hpp"
#include "headers/Log.hpp"

extern std::vector < daterem::Event* > s;
extern const time_t now;
extern const tm *ltm;

daterem::EveryDay::EveryDay()
{
    file.open(DATA_FILE, std::ios::in);
    if (!file.good()) print(L_ERROR, "Cannot open the data file, create a new reminder first");

    short lineNum = objCount * LINES_PER_OBJ + 1; // information on wich line the object data starts (every object takes 2 lines)
    short actualLine = 1;
    std::string line;
    while(getline(file, line))
    {
        if (actualLine == lineNum) m_Title = line;
        else if (actualLine == lineNum + 1) m_Description = line;
        actualLine++;
    }
    file.close();
    objCount++;
}


daterem::EveryDay::EveryDay(std::string t, std::string des) : Event(t, des)
{
    objCount++;
}


daterem::EveryDay::~EveryDay()
{
    objCount--;
}





std::string daterem::EveryDay::GetData() const
{
    std::ostringstream oss;
    oss << std::left << std::setw(15) << std::setfill(' ') << "Everyday";
    oss << std::setw(25) << m_Title << m_Description << std::endl;
    return oss.str();
}


void daterem::EveryDay::Save() const
{
    file.open(DATA_FILE, std::ios::out | std::ios::app);
    if (!file.good()){
        file.close();
        AppDataCheckMeta();
        Save();
    }
    else
    {
        file << m_Title;
        file << '\n' << m_Description << '\n';
        file.close();
    }
}


void daterem::EveryDay::Check() const
{
    Event::anyEvent = true;
    std::cout << std::left << std::setw(25) << std::setfill(' ') << m_Title << m_Description << std::endl;
}





void daterem::EveryDay::GetSavedEvents()
{
    std::string line;
    unsigned int numOfLines{};
    file.open(DATA_FILE, std::ios::in);
    if (!file.good()) print(L_ERROR, "Cannot open the data file, create a new reminder first");

    while (getline(file, line))
        if (!line.empty()) numOfLines++;
    
    file.close();
    for (unsigned int i = 0; i < (numOfLines / LINES_PER_OBJ); i++)
        s.push_back(new EveryDay);
}