#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <iomanip>

using namespace std;

struct MAP_struct
{
    string offset = "";
    string pages = "";
};

class Analyzer
{
public:
    bool Analyze_FTCUB4_9700(map<int, MAP_struct>& MapFile)
    {
        short MbLocation = 0;
        if (MapFile.at(10).pages[0] == 'm' && MapFile.at(10).pages[1] == 'd')
        {
            MbLocation = 1;
            for (const auto& Ch : MapFile.at(0).pages)
            {
                if (Ch != '.')
                {
                    return false;
                }
            }
        }
        else
        {
            if (MapFile.at(16).pages[0] == 'm' && MapFile.at(16).pages[1] == 'd')
            {
                MbLocation = 2;
                for (const auto& Ch : MapFile.at(8).pages)
                {
                    if (Ch != '.')
                    {
                        return false;
                    }
                }
            }
            
        }

        cout << "This ticket is duplicate of FTCUB4-9700, because first line SBL partition erased!" << endl;
        cout << "The first blocks partition SBL: " << endl;
        if (MbLocation == 1)
        {
            cout << setw(4) << setfill('0') << 0 << " " <<  MapFile.at(0).offset << " " << MapFile.at(0).pages << endl;
            cout << setw(4) << setfill('0') << 1 << " " <<  MapFile.at(1).offset << " " << MapFile.at(1).pages << endl;
        }
        else
        {
            cout << setw(4) << setfill('0') << 8 << " " <<  MapFile.at(8).offset << " " << MapFile.at(8).pages << endl;
            cout << setw(4) << setfill('0') << 9 << " " <<  MapFile.at(9).offset << " " << MapFile.at(9).pages << endl;
        }

        return true;
    }
};




void PrintTableBitflips(const map<int, int>& bit_flips)
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

void FindMain(const vector<int>& NumbersBlocksWithProblems, const map<int, int>& BitFlips)  
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
    PrintTableBitflips(BitFlips);
    cout << endl;
}

int main()
{
    vector<int> NumbersBlocksWithProblems;
    map<int, int> BitFlips = {{1, 0}, {2, 0}, {3, 0},
                                {4, 0}, {5, 0}, {6, 0},
                                {7, 0}, {8, 0}, {9, 0}};
    map<int, MAP_struct> MapFile;

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
        int BlockNum;
        string Offset = "";
        string Pages = "";

        S >> BlockNum >> Offset >> Pages;

        for(const auto& ch : Pages)
        {
            switch (ch)
            {
            case '1':
                ++BitFlips[1];
                break;
            case '2':
                ++BitFlips[2];
                break;
            case '3':
                ++BitFlips[3];
                break;
            case '4':
                ++BitFlips[4];
                break;
            case '5':
                ++BitFlips[5];
                break;
            case '6':
                ++BitFlips[6];
                break;
            case '7':
                ++BitFlips[7];
                break;
            case '8':
                ++BitFlips[8];
                break;
            case '9':
                ++BitFlips[9];
                break;
            default:
                break;
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
        FindMain(NumbersBlocksWithProblems, BitFlips);
    }
    else
    {
        Analyzer a;
        if (!a.Analyze_FTCUB4_9700(MapFile))
        {
            cout << "Analyzer didn`t find similar tickets, it is master ticket maybe" << endl;
        }
        FindMain(NumbersBlocksWithProblems, BitFlips);
    }

    system("pause");
    return 0;
}