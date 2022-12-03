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
#include <fstream>
#include <string>
#include <windows.h>
#include <cmath>
#include <ctime>
#include "Event.hpp"

namespace dr = date_rem;
using std::cout, std::endl;

unsigned short dr::Event::objCount = 0;
bool dr::Event::anyEvent = false;

// CONSTRUCTORS / DESTRUCTOR

dr::Event::Event()
{
    rem.open(REMINDERS, std::ios::in);
    if (!rem.good()){
        SetConsoleTextAttribute(hOut, 12);
        std::clog << "Error! Cannot open the data file, create a new reminder first." << endl;
        SetConsoleTextAttribute(hOut, 7);
        exit(EXIT_FAILURE);
    }
    short lineNum = objCount * 7 + 1; // information on wich line the object data starts (every object takes 7 lines)
    short actualLine = 1;
    std::string line;
    while(getline(rem, line)) {
        if (actualLine == lineNum) day = stoi(line);
        else if (actualLine == lineNum + 1) month = stoi(line);
        else if (actualLine == lineNum + 2) year = stoi(line);
        else if (actualLine == lineNum + 3) title = line;
        else if (actualLine == lineNum + 4) description = line;
        else if (actualLine == lineNum + 5) everyYearEvent = stoi(line);
        else if (actualLine == lineNum + 6) remBefore = stoi(line);
        actualLine++;
    }
    rem.close();
    DefineDate();
    objCount++;
}

dr::Event::Event(std::string d, std::string t, std::string des)
{
    bool exitStat = false;
    std::string con;

    if (!(d.length() == 5 || d.length() == 10)) exitStat = true; 
    
    if (!isdigit(d[0]) || !isdigit(d[1]) || !isdigit(d[3]) || !isdigit(d[4])) exitStat = true;
    else
    {
        con = d.substr(0,2);
        day = std::stoi(con);
        con = d.substr(3, 2);
        month = std::stoi(con);
        if (day > 31 || day < 1 || month > 12 || month < 1) exitStat = true;
    }

    if (d.length() == 5)
    {
        if (!(d[2] == '.')) exitStat = true;
        year = 0;
        everyYearEvent = true;
    }
    else
    {
        if (d[2] != '.' || d[5] != '.') exitStat = true;
        if (!isdigit(d[6]) || !isdigit(d[7]) || !isdigit(d[8]) || !isdigit(d[9])) exitStat = true;
        else
        {
            everyYearEvent = false;
            con = d.substr(6,4);
            year = std::stoi(con);
            // the year can't be less than current year and grater than current year + 100
            if (year > ltm->tm_year + 2000 || year < ltm->tm_year + 1900) exitStat = true;
        }
    }

    if (exitStat) {
        SetConsoleTextAttribute(hOut, 12);
        std::clog << "Error! Wrong date format." << endl;
        exit(EXIT_FAILURE);
    }

    title = t;
    description = des;
    remBefore = false;
    DefineDate();
    objCount++;
}

dr::Event::~Event()
{
    // cout << "\nObject has been deleted";
    objCount--;
}


// METHODS

void dr::Event::Check()
{
    bool today = false;
    bool before = false;
    if (everyYearEvent)
    {
        if (day == ltm->tm_mday && month == ltm->tm_mon + 1)
            today = true;
    }
    else
    {
        if (day == ltm->tm_mday && month == ltm->tm_mon + 1 && year == ltm->tm_year + 1900)
            today = true;
    }

    if (remBefore)
    {
        if (everyYearEvent)
        {
            if (fRem[0] == ltm->tm_mday && fRem[1] == ltm->tm_mon + 1)
                before = true;
            else if (sRem[0] == ltm->tm_mday && sRem[1] == ltm->tm_mon + 1)
                before = true;
        }
        else
        {
            if (fRem[0] == ltm->tm_mday && fRem[1] == ltm->tm_mon + 1 && fRem[2] == ltm->tm_year + 1900)
                before = true;
            else if (sRem[0] == ltm->tm_mday && sRem[1] == ltm->tm_mon + 1 && sRem[2] == ltm->tm_year + 1900)
                before = true;
        }
    }


    if (today)
    {
        anyEvent = true;
        cout << title << " " << description << endl;
    }

    if (before)
    {
        anyEvent = true;
        cout << "In day " + fullDate + " " + title + " - " + description + " is scheduled" << endl;
    }
}

void dr::Event::ShowData(int i) {
    if (i % 2 == 0) SetConsoleTextAttribute( hOut, 6);
    else SetConsoleTextAttribute( hOut, 7);
    cout << i << " - " << fullDate << " - " << title << " - " << description << endl;
}

void dr::Event::SetToEveryYearEvent(){
    year = 0;
    everyYearEvent = true;
}

void dr::Event::SetToRemBefore() {
    remBefore = true;
}

bool dr::Event::CheckOutOfDate()
{
    bool outDated = false;

    if (!everyYearEvent)
    {
        if (year < ltm->tm_year + 1900) outDated = true;
        else if (month < ltm->tm_mon + 1 && year == ltm->tm_year + 1900) outDated = true;
        else if (day < ltm->tm_mday && month == ltm->tm_mon + 1 && year == ltm->tm_year + 1900) outDated = true;
    }
    if (outDated)
        cout << fullDate << " - " << title << " - " << description << endl;

    return outDated;
}

void dr::Event::DefineDate() {
    // defining fullDate
    if (day <= 9) fullDate = "0" + std::to_string(day) + ".";
    else fullDate = std::to_string(day) + ".";

    if (month <= 9) fullDate += "0" + std::to_string(month);
    else fullDate += std::to_string(month);

    if (!everyYearEvent) fullDate += "." + std::to_string(year);



    // defining firstRemDate and second one
    const unsigned short remDaysF = 7, remDaysS = 2; // days to back reminders the date
    unsigned short y; // stands for year (for determining dates in leap years)
    everyYearEvent ? y = ltm->tm_year + 1900 : y = year;

    // defining remDaysS
    // day

    if (day > remDaysS) {
        sRem[0] = day - remDaysS;
        sRem[1] = month;
        if (!everyYearEvent) sRem[2] = year;
    }
    else {
        switch (month) {
            case 1:
            case 2:
            case 4:
            case 6:
            case 8:
            case 9:
            case 11:
                sRem[0] = 31 - abs(day - remDaysS);
                break;
            case 5:
            case 7:
            case 10:
            case 12:
                sRem[0] = 30 - abs(day - remDaysS);
                break;
            case 3:
                if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0)
                    sRem[0] = 29 - abs(day - remDaysS);
                else
                    sRem[0] = 28 - abs(day - remDaysS);
                break;
        }

        // month and year
        
        if (month == 1) {
            sRem[1] = 12;
            if (!everyYearEvent)
                sRem[2] = year - 1;
        }
        else {
            sRem[1] = month - 1;
            if (!everyYearEvent)
                sRem[2] = year;
        }
    }

    // defining remDaysF
    // day

    if (day > remDaysF) {
        fRem[0] = day - remDaysF;
        fRem[1] = month;
        if (!everyYearEvent) fRem[2] = year;
    }
    else {
        switch (month) {
            case 1:
            case 2:
            case 4:
            case 6:
            case 8:
            case 9:
            case 11:
                fRem[0] = 31 - abs(day - remDaysF);
                break;
            case 5:
            case 7:
            case 10:
            case 12:
                fRem[0] = 30 - abs(day - remDaysF);
                break;
            case 3:
                    if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0)
                        fRem[0] = 29 - abs(day - remDaysF);
                    else
                        fRem[0] = 28 - abs(day - remDaysF);
                break;
        }

        // month and year

        if (month == 1) {
            fRem[1] = 12;
            if (!everyYearEvent)
                fRem[2] = year - 1;
        }
        else {
            fRem[1] = month - 1;
            if (!everyYearEvent)
                fRem[2] = year;
        }
    }
}

void dr::Event::Save() {
    rem.open(REMINDERS, std::ios::out | std::ios::app);
    if (!rem.good()){
        rem.close();
        AppDataCheckMeta();
        Save();
    }
    else
    {
        rem << day;
        rem << endl << month;
        rem << endl << year;
        rem << endl << title;
        rem << endl << description;
        rem << endl << everyYearEvent;
        rem << endl << remBefore << endl;
        rem.close();
    }
}