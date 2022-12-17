#include <filesystem>
#include "headers/daterem.hpp"
#include "headers/Log.hpp"

namespace fs = std::filesystem;
namespace dr = daterem;

const time_t now = time(0);
const tm *ltm = localtime(&now);

extern std::vector < daterem::Event* > s;


void dr::ShowHelp(char* programArg)
{
    const auto helpPath{fs::absolute(programArg).parent_path() / "help.txt"};
    std::string line;
    std::ifstream help(helpPath);
    if (!help.good()) exit(EXIT_FAILURE);

    while(getline(help, line))
        std::cout << line << std::endl;
    
    help.close();
}



void dr::ListAllEvents() {
    for (int i = 0; i < Event::objCount; i++) {
        std::cout << i + 1 << s[i]->GetData();
    }
}



unsigned short dr::CheckEventNr(std::string num)
{
    for (char i : num)
        if (!isdigit(i)) print(L_ERROR, "Incorrect syntax\nTry \'daterem --help\' for more information.");
    
    unsigned short n = stoi(num);

    if (n > Event::objCount || n == 0) print(L_ERROR, "No such event");
    
    return n - 1;
}



void dr::SaveAllEvents()
{
    AppDataCheckMeta();
    std::fstream file;
    file.open(EveryDay::DATA_FILE, std::ios::out | std::ios::trunc);
    if (!file.good()) print(L_ERROR, "Data file not found");
    file.close();

    file.open(Weekly::DATA_FILE, std::ios::out | std::ios::trunc);
    if (!file.good()) print(L_ERROR, "Data file not found");
    file.close();

    file.open(Specified::DATA_FILE, std::ios::out | std::ios::trunc);
    if (!file.good()) print(L_ERROR, "Data file not found");
    file.close();

    for (int i = 0; i < Event::objCount; i++)
        s[i]->Save();
}



void dr::DeleteEvent(unsigned short n) {
    delete s[n];
    s.erase(s.begin() + n);
}



void dr::GetAllSavedEvents()
{
    AppDataCheckMeta();
    // ! THIS ORDER IS IMPORTANT
    Specified::GetSavedEvents();
    Weekly::GetSavedEvents();
    EveryDay::GetSavedEvents();
}



void dr::CheckEvents()
{
    std::cout << "[" << GetLocalDate() << "]" << "\n\n";
    for (int i = 0; i < Event::objCount; i++)
        s[i]->Check();


    if (!Event::anyEvent) std::cout << "No reminders scheduled for today";
}



std::string dr::GetLocalDate()
{
    std::string date;
    if (ltm->tm_mday <= 9)
        date = "0" + std::to_string(ltm->tm_mday) + ".";
    else
        date = std::to_string(ltm->tm_mday) + ".";

    if (ltm->tm_mon + 1 <= 9)
        date += "0" + std::to_string(ltm->tm_mon + 1);
    else
        date += std::to_string(ltm->tm_mon + 1);

    date += "." + std::to_string(ltm->tm_year + 1900);

    return date;
}



void dr::AppDataCheckMeta()
{
    if (!fs::is_directory(EveryDay::DATA_FILE.parent_path()))
    {
        fs::remove_all(EveryDay::DATA_FILE.parent_path());
        fs::create_directory(EveryDay::DATA_FILE.parent_path());
    }

    if (!fs::is_regular_file(EveryDay::DATA_FILE))
        std::ofstream(EveryDay::DATA_FILE).write(0, 0);

    if (!fs::is_regular_file(Weekly::DATA_FILE))
        std::ofstream(Weekly::DATA_FILE).write(0, 0);

    if (!fs::is_regular_file(Specified::DATA_FILE))
        std::ofstream(Specified::DATA_FILE).write(0, 0);
}