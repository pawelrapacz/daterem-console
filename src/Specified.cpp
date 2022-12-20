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

daterem::Specified::Specified()
{
    file.open(DATA_FILE, std::ios::in);
    if (!file.good()) print(L_ERROR, "Cannot open the data file, create a new reminder first");

    short lineNum = objCount * LINES_PER_OBJ + 1; // information on wich line the object data starts (every object takes 7 lines)
    short actualLine = 1;
    std::string line;
    while(getline(file, line)) {
        if (actualLine == lineNum) m_Day = std::stoi(line);
        else if (actualLine == lineNum + 1) m_Month = std::stoi(line);
        else if (actualLine == lineNum + 2) m_Year = std::stoi(line);
        else if (actualLine == lineNum + 3) m_Title = line;
        else if (actualLine == lineNum + 4) m_Description = line;
        else if (actualLine == lineNum + 5) m_EveryYearEvent = std::stoi(line);
        else if (actualLine == lineNum + 6) m_RemBefore = std::stoi(line);
        actualLine++;
    }
    file.close();
    DefineRemBeforeDate();
    insts.push_back(this);
    objCount++;
}


daterem::Specified::Specified(std::string d, std::string t, std::string des) : Event(t, des)
{
    bool exitStat = false;
    std::string con;

    if (!(d.length() == 5 || d.length() == 10)) exitStat = true; 
    
    if (!isdigit(d[0]) || !isdigit(d[1]) || !isdigit(d[3]) || !isdigit(d[4])) exitStat = true;
    else
    {
        con = d.substr(0,2);
        m_Day = std::stoi(con);
        con = d.substr(3, 2);
        m_Month = std::stoi(con);
        if (m_Day > 31 || m_Day < 1 || m_Month > 12 || m_Month < 1) exitStat = true;
    }

    if (d.length() == 5)
    {
        if (!(d[2] == '.')) exitStat = true;
        m_Year = 0;
        m_EveryYearEvent = true;
    }
    else
    {
        if (d[2] != '.' || d[5] != '.') exitStat = true;
        if (!isdigit(d[6]) || !isdigit(d[7]) || !isdigit(d[8]) || !isdigit(d[9])) exitStat = true;
        else
        {
            m_EveryYearEvent = false;
            con = d.substr(6,4);
            m_Year = std::stoi(con);
            // the year can't be less than current year and grater than current year + 100
            if (m_Year > ltm->tm_year + 2000 || m_Year < ltm->tm_year + 1900) exitStat = true;
        }
    }

    if (exitStat) print(L_ERROR, "Wrong date format");

    m_RemBefore = false;
    DefineRemBeforeDate();
    objCount++;
}


daterem::Specified::~Specified()
{
    objCount--;
}





std::string daterem::Specified::GetData() const
{
    std::ostringstream oss;
    oss << std::left << std::setw(15) << std::setfill(' ') << GetFormatedDate()
    << std::setw(25) << m_Title << m_Description << std::endl;
    return oss.str();
}

void daterem::Specified::Save() const
{
    file.open(DATA_FILE, std::ios::out | std::ios::app);
    if (!file.good()){
        file.close();
        AppDataCheckMeta();
        Save();
    }
    else
    {
        file << m_Day;
        file << '\n' << m_Month;
        file << '\n' << m_Year;
        file << '\n' << m_Title;
        file << '\n' << m_Description;
        file << '\n' << m_EveryYearEvent;
        file << '\n' << m_RemBefore << '\n';
        file.close();
    }
}


void daterem::Specified::Check() const
{
    bool today = false;
    bool before = false;


    if (m_EveryYearEvent)
    {
        if (m_Day == ltm->tm_mday && m_Month == ltm->tm_mon + 1)
            today = true;
    }
    else
    {
        if (m_Day == ltm->tm_mday && m_Month == ltm->tm_mon + 1 && m_Year == ltm->tm_year + 1900)
            today = true;
    }

    if (m_RemBefore)
    {
        if (m_EveryYearEvent)
        {
            if (m_fRem[0] == ltm->tm_mday && m_fRem[1] == ltm->tm_mon + 1)
                before = true;
            else if (m_sRem[0] == ltm->tm_mday && m_sRem[1] == ltm->tm_mon + 1)
                before = true;
        }
        else
        {
            if (m_fRem[0] == ltm->tm_mday && m_fRem[1] == ltm->tm_mon + 1 && m_fRem[2] == ltm->tm_year + 1900)
                before = true;
            else if (m_sRem[0] == ltm->tm_mday && m_sRem[1] == ltm->tm_mon + 1 && m_sRem[2] == ltm->tm_year + 1900)
                before = true;
        }
    }


    if (today)
    {
        Event::anyEvent = true;
        std::cout << std::left << std::setw(25) << std::setfill(' ') << m_Title << m_Description << std::endl;
    }

    if (before)
    {
        Event::anyEvent = true;
        std::cout << std::left << std::setw(25) << std::setfill(' ') << m_Title << m_Description 
        << " is scheduled for " << GetFormatedDate().erase(0, 1) << std::endl;
    }
}





void daterem::Specified::DefineRemBeforeDate()
{
    if (m_RemBefore)
    {
    // defining firstRemDate and second one
    static const unsigned short remDaysF = 7, remDaysS = 2; // days to back reminders the date
    unsigned short y; // stands for year (for determining dates in leap years)
    m_EveryYearEvent ? y = ltm->tm_year + 1900 : y = m_Year;

    // defining remDaysS
    // day

    if (m_Day > remDaysS) {
        m_sRem[0] = m_Day - remDaysS;
        m_sRem[1] = m_Month;
        if (!m_EveryYearEvent) m_sRem[2] = m_Year;
    }
    else {
        switch (m_Month) {
            case 1:
            case 2:
            case 4:
            case 6:
            case 8:
            case 9:
            case 11:
                m_sRem[0] = 31 - abs(m_Day - remDaysS);
                break;
            case 5:
            case 7:
            case 10:
            case 12:
                m_sRem[0] = 30 - abs(m_Day - remDaysS);
                break;
            case 3:
                if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0)
                    m_sRem[0] = 29 - abs(m_Day - remDaysS);
                else
                    m_sRem[0] = 28 - abs(m_Day - remDaysS);
                break;
        }

        // month and year
        
        if (m_Month == 1) {
            m_sRem[1] = 12;
            if (!m_EveryYearEvent)
                m_sRem[2] = m_Year - 1;
        }
        else {
            m_sRem[1] = m_Month - 1;
            if (!m_EveryYearEvent)
                m_sRem[2] = m_Year;
        }
    }

    // defining remDaysF
    // day

    if (m_Day > remDaysF) {
        m_fRem[0] = m_Day - remDaysF;
        m_fRem[1] = m_Month;
        if (!m_EveryYearEvent) m_fRem[2] = m_Year;
    }
    else {
        switch (m_Month) {
            case 1:
            case 2:
            case 4:
            case 6:
            case 8:
            case 9:
            case 11:
                m_fRem[0] = 31 - abs(m_Day - remDaysF);
                break;
            case 5:
            case 7:
            case 10:
            case 12:
                m_fRem[0] = 30 - abs(m_Day - remDaysF);
                break;
            case 3:
                    if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0)
                        m_fRem[0] = 29 - abs(m_Day - remDaysF);
                    else
                        m_fRem[0] = 28 - abs(m_Day - remDaysF);
                break;
        }

        // month and year

        if (m_Month == 1) {
            m_fRem[1] = 12;
            if (!m_EveryYearEvent)
                m_fRem[2] = m_Year - 1;
        }
        else {
            m_fRem[1] = m_Month - 1;
            if (!m_EveryYearEvent)
                m_fRem[2] = m_Year;
        }
    }
    }
}


bool daterem::Specified::CheckOutOfDate() const
{
    bool outDated = false;

    if (!m_EveryYearEvent)
    {
        if (m_Year < ltm->tm_year + 1900) outDated = true;
        else if (m_Month < ltm->tm_mon + 1 && m_Year == ltm->tm_year + 1900) outDated = true;
        else if (m_Day < ltm->tm_mday && m_Month == ltm->tm_mon + 1 && m_Year == ltm->tm_year + 1900) outDated = true;
    }
    if (outDated)
        std::cout << GetFormatedDate() << " - " << m_Title << " - " << m_Description << std::endl;

    return outDated;
}


std::string daterem::Specified::GetFormatedDate() const
{
    std::string date;
    if (m_RemBefore) date += "*";

    if (m_Day <= 9) date = "0" + std::to_string(m_Day) + ".";
    else date += std::to_string(m_Day) + ".";

    if (m_Month <= 9) date += "0" + std::to_string(m_Month);
    else date += std::to_string(m_Month);

    if (!m_EveryYearEvent) date += "." + std::to_string(m_Year);

    return date;
}


void daterem::Specified::SetEveryYearEvent()
{
    m_EveryYearEvent = m_EveryYearEvent ? false : true;
}


void daterem::Specified::SetRemBefore()
{
    m_RemBefore = m_RemBefore ? false : true;
}





void daterem::Specified::GetSavedEvents()
{
    std::string line;
    unsigned int numOfLines{};
    file.open(DATA_FILE, std::ios::in);
    if (!file.good()) print(L_ERROR, "Cannot open the data file, create a new reminder first");

    while (getline(file, line))
        if (!line.empty()) numOfLines++;
    
    file.close();
    for (unsigned int i = 0; i < (numOfLines / LINES_PER_OBJ); i++)
        s.push_back(new Specified);
}


void daterem::Specified::SaveEvents()
{
    file.open(DATA_FILE, std::ios::out | std::ios::trunc);
    if (!file.good()) print(L_ERROR, "Data file not found");
    file.close();

   for (Specified* i : insts)
        i->Save();
}


void daterem::Specified::DeleteOutOfDate()
{
    std::vector < unsigned int > iter;
    iter.reserve(10);

    for (unsigned int i = 0; i < objCount; i++)
        if ( insts[i]->CheckOutOfDate() ) iter.push_back(i);
    

    for (unsigned int i = 0; i < (unsigned int)iter.size(); i++)
    {
        daterem::DeleteEvent(iter[i] - i);
        insts.erase( insts.begin() + ( iter[i] - i ) );
    }

    std::cout << "Deleted: " << iter.size() << " reminders";
    iter.clear();
}