#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <iomanip>

using namespace std;

struct MessageSyms
{
    string description;
    int count;
};


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
    //
    map<char, int> BitFlipsCount; // <bitflip_type, count_in_MAP_file>
    string BitFlipsStr; // types bitflip
    map<char, set<string>> BitFlipsNumBlocks; // <bitflip_type, [full_line]>
    map<string, MAP_struct> MapFile; // <block_num, {offset, pages_line}>
public:
    void StartAnalyze();
    bool GetDateFromMAP();
    bool GetDateFromMessages();
    bool GetDateFromVucAppModeFile();
    void PrintTableBitflips();
    void WriteToFileTableBitflips(ofstream& out);
private:
    //

    string MainLink = "https://jira-iic.zone2.agileci.conti.de/browse/";
    map<int, map<string, MessageSyms>> ArrayMessageSyms; // <message_num, <name_sym, {description, count}>>
    map<int, string> VucAppModeLogs; // <line_num, line_text>
    int IndexVuc;
public:
    Analyzer();

    void PrintArrayMessagesSyms();
    void ExtractDuplicateIntoTheFile(const DuplicateReport& rep);

    DuplicateReport Analyze_FTCUB4_9700();
    DuplicateReport Analyze_FTCUB4_9641();
    DuplicateReport Analyze_FTCUB4_9556();
    
    void CreateDuplicateTable(const DuplicateReport& ticket);

    void WriteToFileAllFails(ofstream& out);
};