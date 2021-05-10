#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <iomanip>

using namespace std;

struct DuplicateReport
{
    string name;
    string type;
    vector<string> symptoms;
};

struct MAP_struct
{
    string offset = "";
    string pages = "";
};

class Analyzer
{
    string MainLink = "https://jira-iic.zone2.agileci.conti.de/browse/";
public:
    DuplicateReport Analyze_FTCUB4_9700(map<string, MAP_struct>& MapFile)
    {
        short MbLocation = 0;
        if (MapFile.at("0010").pages[0] == 'm' && MapFile.at("0010").pages[1] == 'd')
        {
            MbLocation = 1;
            for (const auto& Ch : MapFile.at("0000").pages)
            {
                if (Ch != '.')
                {
                    return {"None", "", {}};
                }
            }
        }
        else
        {
            if (MapFile.at("0016").pages[0] == 'm' && MapFile.at("0016").pages[1] == 'd')
            {
                MbLocation = 2;
                for (const auto& Ch : MapFile.at("0008").pages)
                {
                    if (Ch != '.')
                    {
                        return {"None", "", {}};
                    }
                }
            }
            
        }

        vector<string> Symptoms;

        Symptoms.push_back("The first blocks partition SBL:");
        if (MbLocation == 1)
        {
            Symptoms.push_back("0000 " + MapFile.at("0000").offset + ' ' + MapFile.at("0000").pages);
            Symptoms.push_back("0001 " + MapFile.at("0001").offset + ' ' + MapFile.at("0001").pages);
        }
        else
        {
            Symptoms.push_back("0008 " + MapFile.at("0008").offset + ' ' + MapFile.at("0008").pages);
            Symptoms.push_back("0009 " + MapFile.at("0009").offset + ' ' + MapFile.at("0009").pages);
        }

        return {"FTCUB4-9700", "Memory corruption", Symptoms};
    }
    
    void CreateDuplicateTable(const DuplicateReport& ticket)
    {
        cout << ' ' << setfill('_') << setw(122) << ' ' << endl;
        cout << '|' << setfill(' ') << setw(10) << left << "Cause" << '|' << setw(110) << left << ticket.type + " (" + "duplicate of " + ticket.name + " - " +  MainLink + ticket.name << '|' << endl;
        cout << '|' << setfill('_') << setw(11) << right << '|' << setw(111) << '|' << endl;

        cout << '|' << setfill(' ') << setw(10) << left << "Tech.Det." << '|' << setw(111) << right << '|' << endl;
        for (const auto& sym : ticket.symptoms)
        {
            cout << '|' << setw(11) << right << '|' << setw(110) << left << sym << '|' << endl;
        }
        cout << '|' << setfill('_') << setw(11) << right << '|' << setw(111) << '|' << endl;
    }

    void WriteToFileAllFails(const map<char, set<string>>& BitFlipsNumBlocks, const map<string, MAP_struct>& MapFile)
    {
        ofstream out("ALL_FAILS.txt");
        for (const auto& bitf : BitFlipsNumBlocks)
        {
            out << "All \'" << bitf.first << "\' Bitflips:" << endl;
            for (const auto& block : bitf.second)
            {
                out << block << " " << MapFile.at(block).offset << " " << MapFile.at(block).pages << endl;
            }
            out << endl;
        }
    }
};




void PrintTableBitflips(const map<char, int>& bit_flips)
{   
    cout << ' ' << setfill('_') << setw(21) << ' ' << endl;
    cout << setw(20) << "|   BitFlips Table   |" << endl;
    cout << '|' << setfill('_') << setw(21) << '|' << endl;
    cout << '|' << setfill(' ') << setw(4) << left << "Type" << '|' << setw(15) << left << "Count" << '|' << endl;
    cout << '|' << setfill('_') << setw(5) << right << '|' << setw(16) << '|' << endl;
    for (const auto& item : bit_flips)
    {
        cout << '|' << setfill(' ') << setw(4) << left << item.first << '|' << setw(15) << left << item.second << '|' << endl;
        cout << '|' << setfill('_') << setw(5) << right << '|' << setw(16) << '|' << endl;
    }
}

void FindMain(const vector<string>& NumbersBlocksWithProblems, const map<char, int>& BitFlipsCount)  
{
    cout << endl << "Statistic" << endl << endl;
    cout << "--------------------------------------------" << endl;
    cout << "Numbers blocks where was found problem: " << endl;
    if (NumbersBlocksWithProblems.empty())
    {
        cout << "None block with the problems!" << endl;
    }
    else
    {
        for (const auto& Item : NumbersBlocksWithProblems)
        {
            cout << Item << endl;
        }
    }
    cout << "--------------------------------------------" << endl;
    PrintTableBitflips(BitFlipsCount);
    cout << endl;
}

int main()
{
    vector<string> NumbersBlocksWithProblems;
    map<char, int> BitFlipsCount = {{'1', 0}, {'2', 0}, {'3', 0},
                                    {'4', 0}, {'5', 0}, {'6', 0},
                                    {'7', 0}, {'8', 0}, {'9', 0},
                                    {'e', 0}, {'U', 0}, {'F', 0},
                                    {'M', 0}, {'B', 0}, {'X', 0}, 
                                    {'*', 0}, {'V', 0}};
    map<string, MAP_struct> MapFile;
    string BitFlipsStr = "123456789UFMBVX*e";
    map<char, set<string>> BitFlipsNumBlocks;

    short Op;
    

    ifstream Mem("ALL_map.txt");

    if (!Mem.is_open())
    {
        cout << "No file with name \"ALL_map.txt\"" << endl;
        system("pause");
        return 0;
    }


    cout << "MENU" << endl << endl;
    cout << "1. To find count BitFlips and numbers of blocks where can be problems - press 1" << endl;
    cout << "2. Analyzation whose duplicate can be this ticket - press 2" << endl;
    cout << "Your choose: ";
    cin >> Op;
    system("cls");

    string Str = "";
    while (getline(Mem, Str))
    {
        stringstream S(Str);
        string BlockNum = "";
        string Offset = "";
        string Pages = "";

        S >> BlockNum >> Offset >> Pages;

        for(const auto& ch : Pages)
        {
            if (BitFlipsStr.find(ch) != string::npos)
            {
                ++BitFlipsCount[ch];
                BitFlipsNumBlocks[ch].insert(BlockNum);
            }
        }

        if(Pages.find('e') != string::npos || Pages.find('X') != string::npos)
        {
            NumbersBlocksWithProblems.push_back(BlockNum);
        }

        if (Op == 2)
        {
            MapFile[BlockNum] = {Offset, Pages};
        }
    }
    Mem.close();

    if (Op == 1)
    {
        FindMain(NumbersBlocksWithProblems, BitFlipsCount);
    }
    else
    {
        Analyzer a;
        DuplicateReport buff;

        buff = a.Analyze_FTCUB4_9700(MapFile);
        if (buff.name != "None")
        {
            a.CreateDuplicateTable(buff);
        }
        else
        {
            cout << "Analyzer didn`t find similar tickets, it is master ticket maybe. Watch ALL_FAILS.txt for all posible fails!" << endl;
            a.WriteToFileAllFails(BitFlipsNumBlocks, MapFile);
        }
        
        //FindMain(NumbersBlocksWithProblems, BitFlipsCount);
    }

    system("pause");
    return 0;
}