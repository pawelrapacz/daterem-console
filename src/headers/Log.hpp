#pragma once

enum Level : unsigned short
{
    L_NONE,
    L_INFO,
    L_WARN,
    L_ERROR
};

class LOG final
{
private:
    LOG();

public:
    LOG(const LOG&) = delete;
    ~LOG();
    static LOG Log;

    template < typename T > inline void print(Level level, T message)
    {
        #include <iostream>
        #include <windows.h>
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
        #include <iostream>
        #include <windows.h>
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
};

LOG LOG::Log;

#define Log LOG::Log