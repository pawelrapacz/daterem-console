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
#include <fstream>
#include <filesystem>

namespace daterem
{
    class Event
    {
    protected:
        static std::fstream file;
        
        std::string m_Title;
        std::string m_Description;
        Event();
        Event(std::string, std::string);

    public:
        virtual ~Event();

    public:
        inline static unsigned int objCount{};
        inline static const std::filesystem::path DATA_FILE{NULL};
        inline static bool anyEvent = false;

    public: 
        virtual std::string GetData() const = 0;
        virtual void Save() const = 0;
        virtual void Check() const = 0;
    }; 
}