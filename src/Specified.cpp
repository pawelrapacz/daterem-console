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
#include <cstring>
#include <sstream>
#include <memory>

#include "daterem.hpp"
#include "Log.hpp"

extern std::vector<std::unique_ptr<daterem::Event>> events;
extern const time_t now;
extern const tm *ltm;

daterem::Specified::Specified(dateint day, dateint month, dateint year, std::string& title, std::string& desc, bool everyYear, bool remBefore)
    : Event(title, desc), _day(day), _month(month), _year(year), _everyYearEvent(everyYear), _remBefore(remBefore)
{
    DefineRemBeforeDate();
    insts.push_back(this);
}

daterem::Specified::Specified(const char* d, const char* t, const char* des) : Event(t, des)
{
    bool exitStat = false;
    std::string con;

    if (!(std::strlen(d) == 5 || std::strlen(d) == 10)) exitStat = true; 
    
    if (!std::isdigit(d[0]) || !std::isdigit(d[1]) || !std::isdigit(d[3]) || !std::isdigit(d[4])) exitStat = true;
    else
    {
        con = std::string(1, d[0]) + d[1];
        _day = std::stoi(con);
        con = std::string(1, d[3]) + d[4];
        _month = std::stoi(con);
        if (_day > 31 || _day < 1 || _month > 12 || _month < 1) exitStat = true;
    }

    if (std::strlen(d) == 5)
    {
        if (!(d[2] == '.')) exitStat = true;
        _year = 0;
        _everyYearEvent = true;
    }
    else
    {
        if (d[2] != '.' || d[5] != '.') exitStat = true;
        if (!std::isdigit(d[6]) || !std::isdigit(d[7]) || !std::isdigit(d[8]) || !std::isdigit(d[9])) exitStat = true;
        else
        {
            _everyYearEvent = false;
            con = std::string(1, d[6]) + d[7] + d[8] + d[9];
            _year = std::stoi(con);
            // the year can't be less than current year and grater than current year + 100
            if (_year > ltm->tm_year + 2000 || _year < ltm->tm_year + 1900) exitStat = true;
        }
    }

    if (exitStat) print(L_ERROR, "Wrong date format");

    DefineRemBeforeDate();
}

daterem::Specified::Specified(std::string& d, std::string& t, std::string& des) : Event(t, des)
{
    bool exitStat = false;
    std::string con;

    if (!(d.length() == 5 || d.length() == 10)) exitStat = true; 
    
    if (!std::isdigit(d[0]) || !std::isdigit(d[1]) || !std::isdigit(d[3]) || !std::isdigit(d[4])) exitStat = true;
    else
    {
        con = d.substr(0,2);
        _day = std::stoi(con);
        con = d.substr(3, 2);
        _month = std::stoi(con);
        if (_day > 31 || _day < 1 || _month > 12 || _month < 1) exitStat = true;
    }

    if (d.length() == 5)
    {
        if (!(d[2] == '.')) exitStat = true;
        _year = 0;
        _everyYearEvent = true;
    }
    else
    {
        if (d[2] != '.' || d[5] != '.') exitStat = true;
        if (!std::isdigit(d[6]) || !std::isdigit(d[7]) || !std::isdigit(d[8]) || !std::isdigit(d[9])) exitStat = true;
        else
        {
            _everyYearEvent = false;
            con = d.substr(6,4);
            _year = std::stoi(con);
            // the year can't be less than current year and grater than current year + 100
            if (_year > ltm->tm_year + 2000 || _year < ltm->tm_year + 1900) exitStat = true;
        }
    }

    if (exitStat) print(L_ERROR, "Wrong date format");

    DefineRemBeforeDate();
}




std::string daterem::Specified::GetData() const
{
    std::ostringstream oss;
    oss << std::left << std::setw(15) << std::setfill(' ') << GetFormatedDate()
    << std::setw(25) << _title << _description << std::endl;
    return oss.str();
}

void daterem::Specified::Save() const
{
    file.open(DATA_FILE, std::ios::out | std::ios::app);
    if (!file.good()){
        file.close();
        print(L_ERROR, "Can not save, file inaccesible");
    }
    else
    {
        file << _day
        << ';' << _month
        << ';' << _year
        << ';' << _title
        << ';' << _description
        << ';' << _everyYearEvent
        << ';' << _remBefore << ";\n";
        file.close();
    }
}


void daterem::Specified::Check() const
{
    bool today = false;
    bool before = false;


    if (_everyYearEvent)
    {
        if (_day == ltm->tm_mday && _month == ltm->tm_mon + 1)
            today = true;
    }
    else
    {
        if (_day == ltm->tm_mday && _month == ltm->tm_mon + 1 && _year == ltm->tm_year + 1900)
            today = true;
    }

    if (_remBefore)
    {
        if (_everyYearEvent)
        {
            if (_fRem[0] == ltm->tm_mday && _fRem[1] == ltm->tm_mon + 1)
                before = true;
            else if (_sRem[0] == ltm->tm_mday && _sRem[1] == ltm->tm_mon + 1)
                before = true;
        }
        else
        {
            if (_fRem[0] == ltm->tm_mday && _fRem[1] == ltm->tm_mon + 1 && _fRem[2] == ltm->tm_year + 1900)
                before = true;
            else if (_sRem[0] == ltm->tm_mday && _sRem[1] == ltm->tm_mon + 1 && _sRem[2] == ltm->tm_year + 1900)
                before = true;
        }
    }


    if (today)
    {
        Event::_anyEvent = true;
        std::cout << std::left << std::setw(25) << std::setfill(' ') << _title << _description << std::endl;
    }

    if (before)
    {
        Event::_anyEvent = true;
        std::cout << std::left << std::setw(25) << std::setfill(' ') << _title << _description 
        << " is scheduled for " << GetFormatedDate().erase(0, 1) << std::endl;
    }
}





void daterem::Specified::DefineRemBeforeDate()
{
    if (_remBefore)
    {
    // defining firstRemDate and second one
    static const unsigned short remDaysF = 7, remDaysS = 2; // days to back reminders the date
    unsigned short y; // stands for year (for determining dates in leap years)
    _everyYearEvent ? y = ltm->tm_year + 1900 : y = _year;

    // defining remDaysS
    // day

    if (_day > remDaysS) {
        _sRem[0] = _day - remDaysS;
        _sRem[1] = _month;
        if (!_everyYearEvent) _sRem[2] = _year;
    }
    else {
        switch (_month) {
            case 1:     [[fallthrough]];
            case 2:     [[fallthrough]];
            case 4:     [[fallthrough]];
            case 6:     [[fallthrough]];
            case 8:     [[fallthrough]];
            case 9:     [[fallthrough]];
            case 11:
                _sRem[0] = 31 - abs(_day - remDaysS);
                break;
            case 5:     [[fallthrough]];
            case 7:     [[fallthrough]];
            case 10:    [[fallthrough]];
            case 12:
                _sRem[0] = 30 - abs(_day - remDaysS);
                break;
            case 3:
                if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0)
                    _sRem[0] = 29 - abs(_day - remDaysS);
                else
                    _sRem[0] = 28 - abs(_day - remDaysS);
                break;
        }

        // month and year
        
        if (_month == 1) {
            _sRem[1] = 12;
            if (!_everyYearEvent)
                _sRem[2] = _year - 1;
        }
        else {
            _sRem[1] = _month - 1;
            if (!_everyYearEvent)
                _sRem[2] = _year;
        }
    }

    // defining remDaysF
    // day

    if (_day > remDaysF) {
        _fRem[0] = _day - remDaysF;
        _fRem[1] = _month;
        if (!_everyYearEvent) _fRem[2] = _year;
    }
    else {
        switch (_month) {
            case 1:     [[fallthrough]];
            case 2:     [[fallthrough]];
            case 4:     [[fallthrough]];
            case 6:     [[fallthrough]];
            case 8:     [[fallthrough]];
            case 9:     [[fallthrough]];
            case 11:
                _fRem[0] = 31 - abs(_day - remDaysF);
                break;
            case 5:     [[fallthrough]];
            case 7:     [[fallthrough]];
            case 10:    [[fallthrough]];
            case 12:
                _fRem[0] = 30 - abs(_day - remDaysF);
                break;
            case 3:
                    if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0)
                        _fRem[0] = 29 - abs(_day - remDaysF);
                    else
                        _fRem[0] = 28 - abs(_day - remDaysF);
                break;
        }

        // month and year

        if (_month == 1) {
            _fRem[1] = 12;
            if (!_everyYearEvent)
                _fRem[2] = _year - 1;
        }
        else {
            _fRem[1] = _month - 1;
            if (!_everyYearEvent)
                _fRem[2] = _year;
        }
    }
    }
}


bool daterem::Specified::CheckOutOfDate() const
{
    bool outDated = false;

    if (!_everyYearEvent)
    {
        if (_year < ltm->tm_year + 1900) outDated = true;
        else if (_month < ltm->tm_mon + 1 && _year == ltm->tm_year + 1900) outDated = true;
        else if (_day < ltm->tm_mday && _month == ltm->tm_mon + 1 && _year == ltm->tm_year + 1900) outDated = true;
    }
    if (outDated)
        std::cout << GetFormatedDate() << " - " << _title << " - " << _description << std::endl;

    return outDated;
}


std::string daterem::Specified::GetFormatedDate() const
{
    std::string date;
    if (_remBefore) date += "*";

    if (_day <= 9) date += "0" + std::to_string(_day) + ".";
    else date += std::to_string(_day) + ".";

    if (_month <= 9) date += "0" + std::to_string(_month);
    else date += std::to_string(_month);

    if (!_everyYearEvent) date += "." + std::to_string(_year);

    return date;
}


void daterem::Specified::SetEveryYearEvent()
{
   _everyYearEvent = true;
}


void daterem::Specified::SetRemBefore()
{
    _remBefore = !_remBefore;
}





void daterem::Specified::GetSavedEvents()
{
    file.open(DATA_FILE, std::ios::in);
    if (!file.good())
        print(L_ERROR, "Cannot open the data file, create a new reminder first");

    std::string day, month, year, title, desc, everyYear, remBefore, lineEnd;
    while (std::getline(file, day, ';'))
    {
        std::getline(file, month, ';');
        std::getline(file, year, ';');
        std::getline(file, title, ';');
        std::getline(file, desc, ';');
        std::getline(file, everyYear, ';');
        std::getline(file, remBefore, ';');
        std::getline(file, lineEnd);
        events.emplace_back(new Specified{
            static_cast<dateint>(std::stoi(day)),
            static_cast<dateint>(std::stoi(month)),
            static_cast<dateint>(std:: stoi(year)),
            title,
            desc,
            static_cast<bool>(std::stoi(everyYear)),
            static_cast<bool>(std::stoi(remBefore))
        });
    }
    file.close();
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