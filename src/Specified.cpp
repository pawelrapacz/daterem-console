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

#include <string>

#include "headers/Specified.hpp"


daterem::Specified::Specified(/* args */)
{
}

daterem::Specified::~Specified()
{
}





std::string daterem::Specified::GetData() const
{
    std::string date;
    if (m_Day <= 9) date = "0" + std::to_string(m_Day) + ".";
    else date= std::to_string(m_Day) + ".";

    if (m_Month <= 9) date += "0" + std::to_string(m_Month);
    else date += std::to_string(m_Month);

    if (!m_EveryYearEvent) date += "." + std::to_string(m_Year);

    return " - " + date + " - " + m_Title + " - " + m_Description + '\n';
}


void daterem::Specified::Save() const
{
}


void daterem::Specified::Check() const
{
}