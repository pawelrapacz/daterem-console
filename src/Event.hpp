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
        inline static std::fstream file;
        
        std::string _title; // max 20 characters
        std::string _description;
        Event(std::string&, std::string&);
        Event(const char*, const char*);
        inline static bool _anyEvent = false;
        inline static std::size_t _objCount{};

    public:
        virtual ~Event();

    public:
        static const std::filesystem::path DATA_FILE;
        inline static const bool& anyEvent = _anyEvent;
        inline static std::size_t& objCount = _objCount;

    public: 
        virtual std::string GetData() const = 0;
        virtual void Save() const = 0;
        virtual void Check() const = 0;
    }; 
}
