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
#include <filesystem>
#include "Event.hpp"

namespace daterem
{
    class Weekly : public Event
    {
    private:
        inline static const std::string days[]
        {
            "Sunday",
            "Monday",
            "Tuesday",
            "Wednsday",
            "Thursday",
            "Friday",
            "Saturday"
        };

        enum wDay : unsigned short
        {
            SUNDAY,
            MONDAY,
            TUESDAY,
            WEDNSDAY,
            THURSDAY,
            FRIDAY,
            SATURDAY
        };

        wDay m_wDay;

    public:
        Weekly();
        Weekly(std::string, std::string, std::string);
        ~Weekly();

    public:
        inline static unsigned int objCount{};
        inline static const std::filesystem::path DATA_FILE{std::filesystem::path(getenv("APPDATA")) / "daterem" / "Weekly"};
        inline static const unsigned short LINES_PER_OBJ{3};

    public:
        std::string GetData() const override;
        void Save() const override;
        void Check() const override;

        static void GetSavedEvents();
        static bool CheckIfWDay(std::string);
    };
}