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

enum Level : unsigned short
{
    L_NONE,
    L_INFO,
    L_WARN,
    L_ERROR
};

#include <iostream>
#include <windows.h>

template<typename T> inline void print(Level level, T message)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (level)
    {
    case L_NONE:
        std::cout << message;
        break;
    case L_INFO:
        std::cout << "[INFO]: " << message;
        break;

    case L_WARN:
        SetConsoleTextAttribute(hConsole, 12);
        std::clog << "[WARNING]: " << message;
        SetConsoleTextAttribute(hConsole, 7);
        break;

    case L_ERROR:
        SetConsoleTextAttribute(hConsole, 12);
        std::clog << "[ERROR]: " << message;
        SetConsoleTextAttribute(hConsole, 7);
        exit(EXIT_FAILURE);
        break;
    }
}

template < typename T > inline void println(Level level, T message)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (level)
    {
    case L_NONE:
        std::cout << message << '\n';
        break;
    case L_INFO:
        std::cout << "[INFO]: " << message << '\n';
        break;

    case L_WARN:
        SetConsoleTextAttribute(hConsole, 12);
        std::clog << "[WARNING]: " << message << '\n';
        SetConsoleTextAttribute(hConsole, 7);
        break;

    case L_ERROR:
        SetConsoleTextAttribute(hConsole, 12);
        std::clog << "[ERROR]: " << message << '\n';
        SetConsoleTextAttribute(hConsole, 7);
        exit(EXIT_FAILURE);
        break;
    }
}