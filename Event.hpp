#pragma once

#include <windows.h>
#include <string>
#include <fstream>

namespace date_rem
{
    class Event
    {
    private:
        unsigned short day;
        unsigned short month;
        unsigned short year;
        std::string title;
        std::string description, remDescription;
        std::string fullDate;
        std::string firstRemDate, secRemDate;
        bool everyYearEvent;
    public:
        static bool anyEvent;
        static unsigned short objCount;
        Event();
        Event(std::string, std::string, std::string);
        ~Event();

        void CheckDate(std::string);

        void DefineDate();
        void ShowData(int);
        void Save();
        void Check(bool);
        void SetToEveryYearEvent();
    };

    static HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    static const time_t now = time(0);
    static const tm *ltm = localtime(&now);

    static std::fstream rem;
    const std::string REMINDERS = "reminders";
    static std::ofstream bat;
    const std::string BAT = "DateRem.bat";

    // OTHER FUNCTIONS
    void GetSavedEvents();
    void SaveAllEvents();
    void ListAllEvents();
    void DeleteEvent(unsigned short);
    unsigned short CheckEventNr(int);
    void ShowHelp();
    void CheckEvents();
    std::string GetLocalDate();
}