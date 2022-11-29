#pragma once

#include <string>
#include <fstream>
#include <vector>

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
        ~Event();
        void SetDay();
        void SetMonth();
        void SetYear();
        void DefineDate();
        void ShowData();
        void ShowData(int);
        void ChangeData();
        void Save();
        void SaveBat();
    };

    static std::fstream rem;
    const std::string REMINDERS = "reminders";
    static std::ofstream bat;
    const std::string BAT = "DateRem.bat";

    // UI FUNCTIONS
    void DrawMenu();
    void ListMenu();
    char Input();
    void Pause();

    // OTHER FUNCTIONS
    void GetSavedEvents();
    void SaveAllEvents();
    void ListAllEvents();
    void ExportAllEventsToBat();
    void DeleteEvent(unsigned short);
    unsigned short GetEventNr();
}