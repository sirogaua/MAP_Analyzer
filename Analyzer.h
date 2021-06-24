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
    map<char, set<string>> BitFlipsNumBlocks; // <bitflip_type, [array_num_blocks]>
    map<string, MAP_struct> MapFile; // <block_num, {offset, pages_line}>
public:
    void StartAnalyze();
    bool GetDateFromMAP();
    bool GetDateFromMessages();
    bool GetDateFromVucAppModeFile();
    void PrintTableBitflips();
    void WriteToFileTableBitflips(ofstream& out);
private:
    //MAP constants
    int UBiStartBlock;
    int UBiEndBlock;
    int BootStartBlock;
    int BootEndBlock;


    string MainLink = "https://jira-iic.zone2.agileci.conti.de/browse/";
    map<int, map<string, MessageSyms>> ArrayMessageSyms; // <message_num, <name_sym, {description, count}>>
    map<int, string> VucAppModeLogs; // <line_num, line_text>
    int IndexVuc;
public:
    Analyzer();

    void PrintArrayMessagesSyms();
    void ExtractDuplicateIntoTheFile(const DuplicateReport& rep);

    DuplicateReport Analyze_FTCUB4_9700(); // erased first block SBL partition
    DuplicateReport Analyze_FTCUB4_9641(); // Fota update complite, but after not seen comand reset
    DuplicateReport Analyze_FTCUB4_9556(); // Fota update complite, but after ERROR
    DuplicateReport Analyze_FTCUB4_9996(); // UECC errors in 0000 block SBL partition
    DuplicateReport Analyze_FTCUB4_10373(); // UECC errors in 0001 block SBL partition
    DuplicateReport Analyze_FTCUB4_9997(); // UECC errors in some blocks UBI partition
    DuplicateReport Analyze_FTCUB4_9693(); // UECC errors in some blocks BOOT patition
    
    void CreateDuplicateTable(const DuplicateReport& ticket);

    void WriteToFileAllFails(ofstream& out);
};