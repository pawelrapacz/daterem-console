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
#include <string>
#include <filesystem>
#include <vector>
#include "Event.hpp"

namespace daterem
{
    class Specified : public Event
    {
    private:
        unsigned short m_Day;
        unsigned short m_Month;
        unsigned short m_Year;
        unsigned short m_fRem[3];
        unsigned short m_sRem[3];
        bool m_EveryYearEvent;
        bool m_RemBefore;

    public:
        Specified();
        Specified(std::string, std::string, std::string);
        ~Specified();

    public:
        inline static const std::filesystem::path DATA_FILE{std::filesystem::path(getenv("APPDATA")) / "daterem" / "Specified"};
        inline static std::vector < Specified* > insts; // table of pointers to all class instances

    public:
        std::string GetData() const override;
        void Save() const override;
        void Check() const override;

        std::string GetFormatedDate() const;
        bool CheckOutOfDate();
        void DefineRemBeforeDate();
        void SetEveryYearEvent();
        void SetRemBefore();

        static void GetSavedEvents();
        static void DeleteOutOfDate();
    };
}