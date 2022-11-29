#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <cmath>
#include <limits>
#include "Event.hpp"

namespace dr = date_rem;
using std::cout, std::endl;

unsigned short dr::Event::objCount = 0;

// CONSTRUCTORS / DESTRUCTOR

dr::Event::Event()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    rem.open(REMINDERS, std::ios::in);
    if (!rem.good()){
        SetConsoleTextAttribute(hOut, 12);
        std::clog << "B��d dodawania pliku!";
        std::clog << "\nPrawdopodobne problemy:\n\t- plik nie istnieje lub zosta� usuni�ty\n\t- program nie posiada uprawnie� do pliku";
        std::clog << "\n\nSpr�buj przywr�ci� plik lub nada� odpowiednie uprawnienia dost�pu.";
        std::clog << "\n*Je�li program zosta� uruchomiony po raz pierwszy nale�y kontynuowa� normalnie, plik zostanie utworzony automatycznie.";
        SetConsoleTextAttribute(hOut, 7);
        //Pause();
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

dr::Event::Event(bool eYE)
{
    system("cls");
    cout << "Podaj dzie�: "; SetDay();
    cout << "\n\nPodaj miesi�c: "; SetMonth();
    if (!eYE) {
        cout << "\n\nPodaj rok: "; SetYear();
    }
    else year = 0;
    std::cin.ignore();
    cout << "\n\nPodaj tytu�: "; std::getline(std::cin, title);
    cout << "\n\nPodaj opis: "; std::getline(std::cin, description);
    everyYearEvent = eYE;
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

    CheckDate(d);

    title = t;
    description = des;
    DefineDate();
    objCount++;
}

dr::Event::~Event()
{
    // cout << "\n\nUsuni�to obiekt";
    objCount--;
}


// METHODS

void dr::Event::CheckDate(std::string d)
{
    bool exitStat = false;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (day > 31 || day < 1) {
        SetConsoleTextAttribute(hOut, 12);
        std::clog << "Error! Wrong day format" << endl;
        exitStat= true;
    }
    if (month > 12 || month < 1) {
        SetConsoleTextAttribute(hOut, 12);
        std::clog << "Error! Wrong month format" << endl;
        exitStat = true;
    }
    if (!everyYearEvent)
    {
        if (year > 2100 || year < 2022) {
            SetConsoleTextAttribute(hOut, 12);
            std::clog << "Error! Wrong year format" << endl;
            exitStat = true;
        }
    }

    if (exitStat)
        exit(0);
}

void dr::Event::SetDay() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cin >> day;
    if (std::cin.fail() || day > 31 || day < 1) {
        SetConsoleTextAttribute(hOut, 12);
        cout << endl << "B��d wprowadzonych danych! Wprowad� je ponownie" << endl;
        cout << ">";
        SetConsoleTextAttribute(hOut, 7);
        std::cin.clear();
        std::cin.ignore( std::numeric_limits < std::streamsize >::max(), '\n' ); // works, no error
        SetDay();
    }
}

void dr::Event::SetMonth() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cin >> month;
    if (std::cin.fail() || month > 12 || month < 1) {
        SetConsoleTextAttribute(hOut, 12);
        cout << endl << "B��d wprowadzonych danych! Wprowad� je ponownie" << endl;
        cout << ">";
        SetConsoleTextAttribute(hOut, 7);
        std::cin.clear();
        std::cin.ignore( std::numeric_limits < std::streamsize >::max(), '\n' ); // works, no error
        SetMonth();
    }
}

void dr::Event::SetYear() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cin >> year;
    if (std::cin.fail()|| year > 2100 || year < 2022) {
        SetConsoleTextAttribute(hOut, 12);
        cout << endl << "B��d wprowadzonych danych! Wprowad� je ponownie" << endl;
        cout << ">";
        SetConsoleTextAttribute(hOut, 7);
        std::cin.clear();
        std::cin.ignore( std::numeric_limits < std::streamsize >::max(), '\n' ); // works, no error
        SetYear();
    }
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
    remDescription = "W dniu " + fullDate + " zaplanowane jest: " + description;
}

void dr::Event::ShowData(int i) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (i % 2 == 0) {
        SetConsoleTextAttribute( hOut, 6);
        cout << i << " - " << fullDate << " - " << title << " " << description << endl;
    }
    else {
        SetConsoleTextAttribute( hOut, 7);
        cout << i << " - " << fullDate << " - " << title << " " << description << endl;
    }
}

void dr::Event::Save() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    rem.open(REMINDERS, std::ios::out | std::ios::app);
    if (!rem.good()){
        SetConsoleTextAttribute(hOut, 12);
        std::clog << "B��d dodawania pliku!";
        std::clog << "\nPrawdopodobne problemy:\n\t- plik nie istnieje lub zosta� usuni�ty\n\t- program nie posiada uprawnie� do pliku";
        std::clog << "\n\nSpr�buj przywr�ci� plik lub nada� odpowiednie uprawnienia dost�pu.";
        SetConsoleTextAttribute(hOut, 7);
        //Pause();
    }
    rem << day;
    rem << endl << month;
    rem << endl << year;
    rem << endl << title;
    rem << endl << description;
    rem << endl << everyYearEvent << endl;
    rem.close();
}