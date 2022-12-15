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

#include <string>
#include <iostream>
#include "headers/daterem.hpp"


daterem::Weekly::Weekly()
{
    file.open(DATA_FILE, std::ios::in);
    if (!file.good())
    {
        SetConsoleTextAttribute(hOut, 12);
        std::clog << "Error! Cannot open the data file, create a new reminder first." << std::endl;
        SetConsoleTextAttribute(hOut, 7);
        exit(EXIT_FAILURE);
    }
    short lineNum = objCount * 3 + 1; // information on wich line the object data starts (every object takes 3 lines)
    short actualLine = 1;
    std::string line;
    while(getline(file, line))
    {
        if (actualLine == lineNum) m_wDay = (wDay)std::stoi(line);
        else if (actualLine == lineNum + 1) m_Title = line;
        else if (actualLine == lineNum + 2) m_Description = line;
        actualLine++;
    }
    file.close();
    objCount++;
}


daterem::Weekly::Weekly(std::string d, std::string t, std::string des) :Event(t, des)
{
    bool exitStat = false;
    std::string con;
    
    for (char i : d)
        if (!isdigit(i)) exitStat = true;

    if (std::stoi(d) > 6) exitStat = true;
    else m_wDay = (wDay)std::stoi(d);

    if (exitStat)
    {
        SetConsoleTextAttribute(hOut, 12);
        std::clog << "Error! Wrong date format." << std::endl;
        SetConsoleTextAttribute(hOut, 7);
        exit(EXIT_FAILURE);
    }
    objCount++;
}


daterem::Weekly::~Weekly()
{
    objCount--;
}





std::string daterem::Weekly::GetData() const
{
    static const std::string days[]
    {
        "Sunday",
        "Monday",
        "Tuesday",
        "Wednsday",
        "Thursday",
        "Friday",
        "Saturday"
    };
    return " - "  + days[m_wDay] + " - " + m_Title + " - " + m_Description + '\n';
}


void daterem::Weekly::Save() const
{
    file.open(DATA_FILE, std::ios::out | std::ios::app);
    if (!file.good()){
        file.close();
        AppDataCheckMeta();
        Save();
    }
    else
    {
        file << m_wDay;
        file << '\n' << m_Title;
        file << '\n' << m_Description << '\n';
        file.close();
    }
}


void daterem::Weekly::Check() const
{
    bool checked = false;

    if (m_wDay == ltm->tm_wday) checked = true;

    if (checked)
    {
        /**
         * TODO: Better way to handle anyEvent \/
        */
        Event::anyEvent = true;
        std::cout << m_Title << " " << m_Description << std::endl;
    }
}