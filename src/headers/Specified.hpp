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

namespace daterem
{
    class Specified : public Event
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
         * remBefore - if true the reminder is additionally displayed two times before specified date,
         * else the reminder displays only in the specified date [defaulut: false]
         * (SetToRemBefore() sets the value to true);
        */
        unsigned short day;
        unsigned short month;
        unsigned short year;
        unsigned short fRem[3];
        unsigned short sRem[3];
        bool everyYearEvent;
        bool remBefore;

    public:
        static bool anyEvent;
        static unsigned short objCount;

    public:
        Specified();
        Specified(std::string, std::string, std::string);
        ~Specified();

    public:
        void ShowData(int) override;
        void Save() override;
        void Check() override;


        bool CheckOutOfDate();
        void DefineDate();
        void SetToEveryYearEvent();
        void SetToRemBefore();
    };
}