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

#include <windows.h>
#include <string>
#include <fstream>

namespace date_rem
{
    class Event
    {
    private:
        /**
         * day, month, year - reminder date;
         * 
         * fRem[3] - first reminder date [0]day [1]month [2]year;
         * 
         * sRem[3] - second reminder date;
         * 
         * fullDate - represents date in DD.MM.YYYY or DD.MM format
         * 
         * everyYearEvent - if true the reminder is set for every year with no year specified,
         * else the reminder is single event with year precised (SetToEveryYearEvent() method changes the value
         * to true even if the year is specified -> [-e] option) [defaulut: based on the date];
         * 
         * remBefore - if true the reminder is displayed two times before specified date,
         * else the reminder displays only in the specified date [defaulut: false]
         * (SetToRemBefore() sets the value to true);
        */
        unsigned short day;
        unsigned short month;
        unsigned short year;
        unsigned short fRem[3];
        unsigned short sRem[3];
        std::string title;
        std::string description;
        std::string fullDate;
        bool everyYearEvent;
        bool remBefore;
    public:
        static bool anyEvent;
        static unsigned short objCount;
        Event();
        Event(std::string, std::string, std::string);
        ~Event();

        void CheckDateFormat(std::string);
        bool CheckOutOfDate();
        void DefineDate();
        void ShowData(int);
        void Save();
        void Check(bool);
        void SetToEveryYearEvent();
        void SetToRemBefore();
    };

    static HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    static const time_t now = time(0);
    static const tm *ltm = localtime(&now);

    static std::fstream rem;
    const std::string REMINDERS = "../src/reminders";

    // OTHER FUNCTIONS
    void GetSavedEvents();
    void SaveAllEvents();
    void ListAllEvents();
    void DeleteOutOfDate();
    void DeleteEvent(unsigned short);
    unsigned short CheckEventNr(int);
    void ShowHelp();
    void CheckEvents();
    std::string GetLocalDate();
    void ArgErr();
}