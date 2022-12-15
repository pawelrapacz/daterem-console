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
#include <string>

#include "headers/daterem.hpp"

extern std::vector < daterem::Event* > s;

daterem::Specified::Specified()
{
    file.open(DATA_FILE, std::ios::in);
    if (!file.good()){
        SetConsoleTextAttribute(hOut, 12);
        std::clog << "Error! Cannot open the data file, create a new reminder first." << std::endl;
        SetConsoleTextAttribute(hOut, 7);
        exit(EXIT_FAILURE);
    }
    short lineNum = objCount * 7 + 1; // information on wich line the object data starts (every object takes 7 lines)
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
    objCount++;

    /**
     * ! insts desn't work -> no idea why
    */
    insts.push_back(this);
}


daterem::Specified::Specified(std::string d, std::string t, std::string des) :Event(t, des)
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

    if (exitStat) {
        SetConsoleTextAttribute(hOut, 12);
        std::clog << "Error! Wrong date format." << std::endl;
        SetConsoleTextAttribute(hOut, 7);
        exit(EXIT_FAILURE);
    }

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
    return " - " + GetFormatedDate() + " - " + m_Title + " - " + m_Description + '\n';
}


void daterem::Specified::Save() const
{
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
        std::cout << m_Title << " " << m_Description << std::endl;
    }

    if (before)
    {
        Event::anyEvent = true;
        std::cout << "In day "+ GetFormatedDate() + " " + m_Title + " - " + m_Description + " is scheduled" << std::endl;
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


bool daterem::Specified::CheckOutOfDate()
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
    if (m_Day <= 9) date = "0" + std::to_string(m_Day) + ".";
    else date= std::to_string(m_Day) + ".";

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


void daterem::Specified::DeleteOutOfDate()
{
    std::vector < unsigned short > iter;
    iter.reserve(10);

    for (int i = 0; i < Event::objCount; i++)
        if (insts[i]->CheckOutOfDate()) iter.push_back(i);
    
    /**
     * ! insts desn't work -> no idea why
     * ! DeleteEvent() deletes wrong/random Events
    */

    for (int i = 0; i < iter.size(); i++)
        daterem::DeleteEvent(iter[i] - i);


    std::cout << "Deleted: " << iter.size() << " reminders";
    iter.clear();
}