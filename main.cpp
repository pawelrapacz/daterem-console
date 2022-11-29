#include <iostream>
#include <vector>
#include <windows.h>
#include <limits>
#include <string>
#include "Event.hpp"

namespace dr = date_rem;

std::vector < dr::Event* > s;

int main(int argc, char *argv[])
{
    SetConsoleCP( 1250 );
    SetConsoleOutputCP( 1250 );
    setlocale( LC_ALL, "1250" );

    for (int i = 1; i < argc; i++)
    {
        if (std::string(argv[i]) == "--list" || std::string(argv[i]) == "-l")
        {
            dr::GetSavedEvents();
            dr::ListAllEvents();
            return EXIT_SUCCESS;
        }
        else if (std::string(argv[i]) == "--new" || std::string(argv[i]) == "-n")
        {
            s.push_back(new dr::Event(std::string(argv[i + 1]), argv[i + 2], argv[i + 3]));
            s.back()->Save();
            return EXIT_SUCCESS;
        }
        else if (std::string(argv[i]) == "-e")
        {
            return EXIT_SUCCESS;
        }
        else {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

void dr::ListAllEvents() {
    for (int i = 0; i < dr::Event::objCount; i++) {
        s[i]->ShowData(i + 1);
    }
}


void dr::GetSavedEvents() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::string line;
    int numOfLines = 0;
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
    while (getline(rem, line)) {
        if (!line.empty()) numOfLines++;
    }
    rem.close();
    // std::cout << numOfLines;

    for (int i = 0; i < (numOfLines / 6); i++) {
        s.push_back(new dr::Event);
    }
}