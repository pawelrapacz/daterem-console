#pragma once

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
        static unsigned short objCount;
        Event();
        Event(bool);
        Event(std::string, std::string, std::string);
        ~Event();

        void CheckDate(std::string);

        void SetDay();
        void SetMonth();
        void SetYear();
        void DefineDate();
        void ShowData(int);
        void Save();
    };

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
    void SaveEvent();
    void ShowHelp();
}