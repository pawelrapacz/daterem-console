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
        std::clog << "Error! Cannot open the save file, check permission settings in the instalation directory." << std::endl;
        std::clog << "Also after the instalation there are no reminders, create one.";
        SetConsoleTextAttribute(hOut, 7);
        exit(EXIT_FAILURE);
    }
    short lineNum = objCount * 6 + 1;
    short actualLine = 1;
    std::string line;
    while(getline(rem, line)) {
        if (actualLine == lineNum) day = stoi(line);
        else if (actualLine == lineNum + 1) month = stoi(line);
        else if (actualLine == lineNum + 2) year = stoi(line);
        else if (actualLine == lineNum + 3) title = line;
        else if (actualLine == lineNum + 4) description = line;
        else if (actualLine == lineNum + 5){
            everyYearEvent = stoi(line);
            break;
        }
        actualLine++;
    }
    rem.close();
    DefineDate();
    objCount++;
}

dr::Event::Event(std::string d, std::string t, std::string des)
{
    std::string con;
    con = d.substr(0,2);
    day = std::stoi(con);
    con = d.substr(3, 2);
    month = std::stoi(con);

    if (d.length() > 6)
    {
        con = d.substr(6,4);
        year = std::stoi(con);
        everyYearEvent = false;
    }
    else {
        year = 0;
        everyYearEvent = true;
    }

    CheckDateFormat(d);

    title = t;
    description = des;
    DefineDate();
    objCount++;
}

dr::Event::~Event()
{
    cout << "\nUsuni�to obiekt";
    objCount--;
}


// METHODS

void dr::Event::Check(bool e)
{
    bool today = false;
    if (everyYearEvent)
    {
        if (day == ltm->tm_mday && month == ltm->tm_mon + 1)
        {
            today = true;
            anyEvent = true;
        }
    }
    else
    {
        if (day == ltm->tm_mday && month == ltm->tm_mon + 1 && year == ltm->tm_year + 1900)
        {
            today = true;
            anyEvent = true;
        }
    }

    if (today)
    {
        if (e)
        { 
            SetConsoleTextAttribute( hOut, 6);
            e = false;
        }
        else
        {
            SetConsoleTextAttribute( hOut, 7);
            e = true;
        }
        cout << title << " " << description << endl;
    }
}

void dr::Event::ShowData(int i) {
    if (i % 2 == 0) {
        SetConsoleTextAttribute( hOut, 6);
        cout << i << " - " << fullDate << " - " << title << " - " << description << endl;
    }
    else {
        SetConsoleTextAttribute( hOut, 7);
        cout << i << " - " << fullDate << " - " << title << " - " << description << endl;
    }
}

void dr::Event::CheckDateFormat(std::string d)
{
    bool exitStat = false;

    if (!(d.length() == 5 || d.length() == 10)) exitStat = true;

    if (!isdigit(d[0]) || !isdigit(d[1]) || !isdigit(d[3]) || !isdigit(d[4])) exitStat = true;

    if (day > 31 || day < 1 || month > 12 || month < 1) exitStat = true;

    if (!everyYearEvent) {
        //if (year > 2100 || year < 2022) exitStat = true;
        if (!isdigit(d[6]) || !isdigit(d[7]) || !isdigit(d[8]) || !isdigit(d[9])) exitStat = true;
    }

    if (exitStat) {
        SetConsoleTextAttribute(hOut, 12);
        std::clog << "Error! Wrong date format." << endl;
        exit(EXIT_FAILURE);
    }
}

void dr::Event::SetToEveryYearEvent(){
    everyYearEvent = true;
}

bool dr::Event::CheckOutOfDate()
{
    bool outDated = false;

    if (!everyYearEvent)
    {
        if (year < ltm->tm_year + 1900) outDated = true;
        if (month < ltm->tm_mon + 1 && year == ltm->tm_year + 1900) outDated = true;
        if (day < ltm->tm_mday && month == ltm->tm_mon + 1 && year == ltm->tm_year + 1900) outDated = true;
    }
    if (outDated)
    {
        cout << fullDate << " - " << title << " - " << description << endl;
    }

    return outDated;
}

void dr::Event::DefineDate() {
    // defining fullDate
    if (day <= 9)
        fullDate = "0" + std::to_string(day) + ".";
    else
        fullDate = std::to_string(day) + ".";


    if (month <= 9)
        fullDate += "0" + std::to_string(month);
    else
        fullDate += std::to_string(month);


    if (!everyYearEvent)
        fullDate += "." + std::to_string(year);

    // defining firstRemDate and second one
    const unsigned short remDaysF = 7, remDaysS = 2; // days to back reminders the date

    // defining remDaysS

    if (day > remDaysS) {
        if (day <= 9)
        secRemDate = "0" + std::to_string(day - remDaysS) + ".";
        else
            secRemDate = std::to_string(day - remDaysS) + ".";


        if (month <= 9)
            secRemDate += "0" + std::to_string(month);
        else
            secRemDate += std::to_string(month);


        if (!everyYearEvent)
            secRemDate += "." + std::to_string(year);
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
                secRemDate = std::to_string(31 - abs(day - remDaysS)) + ".";
                break;
            case 5:
            case 7:
            case 10:
            case 12:
                secRemDate = std::to_string(30 - abs(day - remDaysS)) + ".";
                break;
            case 3:
                if (!everyYearEvent) {
                    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
                        secRemDate = std::to_string(29 - abs(day - remDaysS)) + ".";
                    else
                        secRemDate = std::to_string(28 - abs(day - remDaysS)) + ".";
                }
                break;
        }
        if (month == 1)
            secRemDate += "12";
            if (!everyYearEvent)
                secRemDate += "." + std::to_string(year - 1);
        else {
            if (month - 1 <= 9)
            secRemDate += "0" + std::to_string(month - 1);
            else
            secRemDate += std::to_string(month - 1);
            if (!everyYearEvent)
                secRemDate += "." + std::to_string(year);
        }
    }

    // defining remDaysF

    if (day > remDaysF) {
        if (day <= 9)
        firstRemDate = "0" + std::to_string(day - remDaysF) + ".";
        else
            firstRemDate = std::to_string(day - remDaysF) + ".";


        if (month <= 9)
            firstRemDate += "0" + std::to_string(month);
        else
            firstRemDate += std::to_string(month);


        if (!everyYearEvent)
            firstRemDate += "." + std::to_string(year);
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
                firstRemDate = std::to_string(31 - abs(day - remDaysF)) + ".";
                break;
            case 5:
            case 7:
            case 10:
            case 12:
                firstRemDate = std::to_string(30 - abs(day - remDaysF)) + ".";
                break;
            case 3:
                if (!everyYearEvent) {
                    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
                        firstRemDate = std::to_string(29 - abs(day - remDaysF)) + ".";
                    else
                        firstRemDate = std::to_string(28 - abs(day - remDaysF)) + ".";
                }
                break;
        }
        if (month == 1)
            firstRemDate += "12";
            if (!everyYearEvent)
                firstRemDate += "." + std::to_string(year - 1);
        else {
            if (month - 1 <= 9)
            firstRemDate += "0" + std::to_string(month - 1);
            else
            firstRemDate += std::to_string(month - 1);
            if (!everyYearEvent)
                firstRemDate += "." + std::to_string(year);
        }
    }


    // defining remDescription
    remDescription = "In day" + fullDate + " zaplanowane jest: " + description;
}

void dr::Event::Save() {
    rem.open(REMINDERS, std::ios::out | std::ios::app);
    if (!rem.good()){
        SetConsoleTextAttribute(hOut, 12);
        std::clog << "Error! Cannot open the save file, check permission settings in the instalation directory.";
        SetConsoleTextAttribute(hOut, 7);
        exit(EXIT_FAILURE);
    }
    rem << day;
    rem << endl << month;
    rem << endl << year;
    rem << endl << title;
    rem << endl << description;
    rem << endl << everyYearEvent << endl;
    rem.close();
}