#include "Analyzer.h"

void Analyzer::StartAnalyze()
{
    bool MAP_f = false, Messages_f = false;

    cout << "Started analyzing:" << endl;

    if (GetDateFromMessages())
    {
        Messages_f = true;
        if (GetDateFromVucAppModeFile())
        {
            DuplicateReport DupRep;
            DupRep = Analyze_FTCUB4_9641();
            if (DupRep.name != "None")
            {
                CreateDuplicateTable(DupRep);
                ExtractDuplicateIntoTheFile(DupRep);
                return;
            }

            //////////////
            DupRep = Analyze_FTCUB4_9556();
            if (DupRep.name != "None")
            {
                CreateDuplicateTable(DupRep);
                ExtractDuplicateIntoTheFile(DupRep);
                return;
            }
        }
        
    }

    if (GetDateFromMAP())
    {
        MAP_f = true;
        DuplicateReport DupRep;

        DupRep = Analyze_FTCUB4_9700();
        if (DupRep.name != "None")
        {
            CreateDuplicateTable(DupRep);
            ExtractDuplicateIntoTheFile(DupRep);
            return;
        }

        DupRep = Analyze_FTCUB4_9996();
        if (DupRep.name != "None")
        {
            CreateDuplicateTable(DupRep);
            ExtractDuplicateIntoTheFile(DupRep);
            return;
        }

        DupRep = Analyze_FTCUB4_10373();
        if (DupRep.name != "None")
        {
            CreateDuplicateTable(DupRep);
            ExtractDuplicateIntoTheFile(DupRep);
            return;
        }
        
        DupRep = Analyze_FTCUB4_9693();
        if (DupRep.name != "None")
        {
            CreateDuplicateTable(DupRep);
            ExtractDuplicateIntoTheFile(DupRep);
            return;
        }

        DupRep = Analyze_FTCUB4_9997();
        if (DupRep.name != "None")
        {
            CreateDuplicateTable(DupRep);
            ExtractDuplicateIntoTheFile(DupRep);
            return;
        }

    }

    cout << endl << "Result of analyzing:" << endl;
    cout << "Didn`t find similars master tickets in base. Take a look statistic in ALL_FAILS.txt!" << endl << endl;

    //////////////////////////
    ofstream out("ALL_FAILS.txt");
    if (MAP_f)
    {
        WriteToFileAllFails(out);
        WriteToFileTableBitflips(out);
    }
    else
    {
        out << "ALL GOOD!";
    }
    out.close();

    //PrintArrayMessagesSyms();
    //PrintTableBitflips();
}


bool Analyzer::GetDateFromMAP()
{
    ifstream FMap("MAP.txt");
    if (FMap.is_open())
    {
        cout << "Started processing MAP.txt" << endl; 
        string MapLine = "";
        while (getline(FMap, MapLine))
        {
            stringstream S(MapLine);
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
            MapFile[BlockNum] = {Offset, Pages};
        }
        FMap.close();
        cout << "Done processing MAP.txt" << endl;
        return true;
    }
    else
    {
        cout << "MAP.txt not found" << endl;
        return false;
    }
    
}

bool Analyzer::GetDateFromMessages()
{
    if (!system("ParseMessages.exe"))
    {
        cout << "Started processing messages" << endl;
        for (int i = -1; i < 5; ++i)
        {
            ifstream in;
            if (i == -1)
            {
                in.open("messages.txt");    
            }
            else
            {
                in.open("messages" + to_string(i) + ".txt");
            }

            string buff = "";
            while (getline(in, buff))
            {
                stringstream ss(buff);

                string s_name = "";
                string s_description = "";
                string s_count;

                getline(ss, s_name, ';');
                getline(ss, s_description, ';');
                getline(ss, s_count);

                ArrayMessageSyms[i][s_name].description = s_description;
                ArrayMessageSyms[i][s_name].count = atoi(s_count.c_str());
            }
            in.close();

            if (i == -1)
            {
                remove("messages.txt");    
            }
            else
            {
                string buff = "messages" + to_string(i) + ".txt";
                remove(buff.c_str());
            }
        }
        cout << "Done processing messages" << endl;
        return true;
    }

    system("cls");
    cout << "Started analyzing:" << endl;
    cout << "Some Messages not found" << endl;
    return false;
}

bool Analyzer::GetDateFromVucAppModeFile()
{
    ifstream VucLogsFile("Vuc_App_Mode_logs.txt");
    if (VucLogsFile.is_open())
    {
        cout << "Started processing Vuc_App_Mode_logs.txt" << endl;
        string line = "";
        IndexVuc = 0;
        while (getline(VucLogsFile, line))
        {
            ++IndexVuc;
            VucAppModeLogs[IndexVuc] = line;
        }
        cout << "Done processing Vuc_App_Mode_logs.txt" << endl;
        return true;
    }
    cout << "Vuc_App_Mode_logs.txt not found" << endl;
    return false;
}





Analyzer::Analyzer()
{
    BitFlipsCount = {{'1', 0}, {'2', 0}, {'3', 0},
                     {'4', 0}, {'5', 0}, {'6', 0},
                     {'7', 0}, {'8', 0}, {'9', 0},
                     {'e', 0}, {'U', 0}, {'F', 0},
                     {'M', 0}, {'B', 0}, {'X', 0}, 
                     {'*', 0}, {'V', 0}};
                     
    BitFlipsStr = "123456789UFMBVX*e";
    UBiStartBlock = 273;
    UBiEndBlock = 4095;
    BootStartBlock = 122;
    BootEndBlock = 169;
}

void Analyzer::PrintArrayMessagesSyms()
{
    for (const auto& item : ArrayMessageSyms)
    {
        cout << "messages" << item.first << ":" << endl;
        for (const auto& i : item.second)
        {
            cout << i.first << " [\'" << i.second.description << "\', " << i.second.count << "]" << endl;
        }
    }
    
}

void Analyzer::PrintTableBitflips()
{   
    cout << ' ' << setfill('_') << setw(21) << ' ' << endl;
    cout << setw(20) << "|   BitFlips Table   |" << endl;
    cout << '|' << setfill('_') << setw(21) << '|' << endl;
    cout << '|' << setfill(' ') << setw(4) << left << "Type" << '|' << setw(15) << left << "Count" << '|' << endl;
    cout << '|' << setfill('_') << setw(5) << right << '|' << setw(16) << '|' << endl;
    for (const auto& item : BitFlipsCount)
    {
        cout << '|' << setfill(' ') << setw(4) << left << item.first << '|' << setw(15) << left << item.second << '|' << endl;
        cout << '|' << setfill('_') << setw(5) << right << '|' << setw(16) << '|' << endl;
    }
}

void Analyzer::WriteToFileTableBitflips(ofstream& out)
{   
    out << ' ' << setfill('_') << setw(21) << ' ' << endl;
    out << setw(20) << "|   BitFlips Table   |" << endl;
    out << '|' << setfill('_') << setw(21) << '|' << endl;
    out << '|' << setfill(' ') << setw(4) << left << "Type" << '|' << setw(15) << left << "Count" << '|' << endl;
    out << '|' << setfill('_') << setw(5) << right << '|' << setw(16) << '|' << endl;
    for (const auto& item : BitFlipsCount)
    {
        out << '|' << setfill(' ') << setw(4) << left << item.first << '|' << setw(15) << left << item.second << '|' << endl;
        out << '|' << setfill('_') << setw(5) << right << '|' << setw(16) << '|' << endl;
    }
}

DuplicateReport Analyzer::Analyze_FTCUB4_9700()
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

DuplicateReport Analyzer::Analyze_FTCUB4_9641()
{
    if (ArrayMessageSyms[4].count("...Link is down"))
    {
        vector<string> Symptoms;
        Symptoms.push_back("According to Sherlock output message files we have a following trace:");

        for (int i = -1; i < 5; ++i)
        {
            if (ArrayMessageSyms[i].count("...Link is down"))
            {
                string name = "";
                if (i == -1)
                {
                    name = "messages";
                }
                else
                {
                    name = "messages" + to_string(i);
                }
                
                
                Symptoms.push_back("Results from " + name + ": " + "...Dates: " + ArrayMessageSyms[i]["...Dates:"].description);
                Symptoms.push_back(static_cast<string>("...Link is down") + 
                                    " [\'" + ArrayMessageSyms[i]["...Link is down"].description + 
                                    "\', " + to_string(ArrayMessageSyms[i]["...Link is down"].count) + "]");
                Symptoms.push_back(" ");
            }
        }
        Symptoms.push_back("--------------------------------------------------------------------------------------------------------------");


        for (int i = 1; i <= IndexVuc; ++i)
        {
            if (VucAppModeLogs[i].find("FOTA : VuC update process complete !") != string::npos)
            {
                if (i == IndexVuc || VucAppModeLogs[i + 1].find("AT+RESET") == string::npos)
                {
                    Symptoms.push_back("According to VUC logs in the end of file we have following trace:");
                    Symptoms.push_back("    " + VucAppModeLogs[i - 1]);
                    Symptoms.push_back("    " + VucAppModeLogs[i]);
                    Symptoms.push_back("But haven`t command reset \"AT+RESET\"");
                    return {"FTCUB4-9641", "FOTA", Symptoms};
                }
                
            }
        }
    }
    return {"None", "", {}};
}

DuplicateReport Analyzer::Analyze_FTCUB4_9556()
{
    if (ArrayMessageSyms[4].count("...Link is down"))
    {
        vector<string> Symptoms;
        Symptoms.push_back("According to Sherlock output message files we have a following trace:");

        for (int i = -1; i < 5; ++i)
        {
            if (ArrayMessageSyms[i].count("...Link is down"))
            {
                string name = "";
                if (i == -1)
                {
                    name = "messages";
                }
                else
                {
                    name = "messages" + to_string(i);
                }
                
                
                Symptoms.push_back("Results from " + name + ": " + "...Dates: " + ArrayMessageSyms[i]["...Dates:"].description);
                Symptoms.push_back(static_cast<string>("...Link is down") + 
                                    " [\'" + ArrayMessageSyms[i]["...Link is down"].description + 
                                    "\', " + to_string(ArrayMessageSyms[i]["...Link is down"].count) + "]");
                Symptoms.push_back(" ");
            }
        }
        Symptoms.push_back("--------------------------------------------------------------------------------------------------------------");


        for (int i = 1; i <= IndexVuc; ++i)
        {
            if (VucAppModeLogs[i].find("FOTA : VuC update process complete !") != string::npos)
            {
                for (int j = i + 1; j <= IndexVuc; ++j)
                {
                    if (VucAppModeLogs[j].find("ERROR") != string::npos)
                    {
                        Symptoms.push_back("According to VUC logs in the end of file we have following trace:");
                        for (int l = i; l <= j; ++l)
                        {
                            Symptoms.push_back("    " + VucAppModeLogs[l]);
                        }
                        return {"FTCUB4-9556", "FOTA", Symptoms};
                    }
                    
                }
                break;    
            }
        }
    }
    return {"None", "", {}};
}

DuplicateReport Analyzer::Analyze_FTCUB4_9996()
{
    for (const auto& page : MapFile.at("0000").pages)
    {
        if (page == 'e')
        {
            vector<string> Symptoms;
            Symptoms.push_back("The first blocks partition SBL:");
            Symptoms.push_back("0000 " + MapFile.at("0000").offset + ' ' + MapFile.at("0000").pages);
            Symptoms.push_back("0001 " + MapFile.at("0001").offset + ' ' + MapFile.at("0001").pages);

            return {"FTCUB4-9996", "Memory corruption", Symptoms};
        }
    }
    
    return {"None", "", {}};
}

DuplicateReport Analyzer::Analyze_FTCUB4_10373()
{
    for (const auto& page : MapFile.at("0001").pages)
    {
        if (page == 'e')
        {
            vector<string> Symptoms;
            Symptoms.push_back("The first blocks partition SBL:");
            Symptoms.push_back("0000 " + MapFile.at("0000").offset + ' ' + MapFile.at("0000").pages);
            Symptoms.push_back("0001 " + MapFile.at("0001").offset + ' ' + MapFile.at("0001").pages);
            Symptoms.push_back("0002 " + MapFile.at("0002").offset + ' ' + MapFile.at("0002").pages);

            return {"FTCUB4-10373", "Memory corruption", Symptoms};
        }
    }
    
    return {"None", "", {}};
}

DuplicateReport Analyzer::Analyze_FTCUB4_9997()
{
    vector<string> Symptoms;
    Symptoms.push_back("Corruption in the following blocks UBI partition:");

    for (int index = UBiStartBlock; index <= UBiEndBlock; ++index)
    {
        string BlockNum = to_string(index);

        if (BlockNum.size() == 3)
        {
            BlockNum = '0' + BlockNum;
        }

        for (const auto& page : MapFile.at(BlockNum).pages)
        {
            if (page == 'e')
            {
                Symptoms.push_back(BlockNum + " " + MapFile.at(BlockNum).offset + ' ' + MapFile.at(BlockNum).pages);
                break;
            }
        }
    }

    if (Symptoms.size() > 1)
    {
        return {"FTCUB4-9997", "Memory corruption", Symptoms};
    }   
    return {"None", "", {}};
}

DuplicateReport Analyzer::Analyze_FTCUB4_9693()
{
    vector<string> Symptoms;
    Symptoms.push_back("Corruption in the following blocks BOOT partition:");

    for (int index = BootStartBlock; index <= BootEndBlock; ++index)
    {
        string BlockNum = '0' + to_string(index);
        for (const auto& page : MapFile.at(BlockNum).pages)
        {
            if (page == 'e')
            {
                Symptoms.push_back(BlockNum + " " + MapFile.at(BlockNum).offset + ' ' + MapFile.at(BlockNum).pages);
                break;
            }
        }
    }

    if (Symptoms.size() > 1)
    {
        return {"FTCUB4-9693", "Memory corruption", Symptoms};
    }   
    return {"None", "", {}};
}   

void Analyzer::CreateDuplicateTable(const DuplicateReport& ticket)
{
    cout << endl << "Result of analyzing:" << endl;

    string SWver = "-------------";
    if (ArrayMessageSyms.count(-1))
    {
        SWver = ArrayMessageSyms[-1]["...Soc Version"].description;
    }
    
    cout << ' ' << setfill('_') << setw(122) << ' ' << endl;
    cout << '|' << setfill(' ') << setw(10) << left << "SW ver:" << '|' << setw(110) << left << SWver << '|' << endl;
    cout << '|' << setfill('_') << setw(11) << right << '|' << setw(111) << '|' << endl;
    cout << '|' << setfill(' ') << setw(10) << left << "Cause" << '|' << setw(110) << left << ticket.type + " (" + "duplicate of " + ticket.name + " - " +  MainLink + ticket.name + ")" << '|' << endl;
    cout << '|' << setfill('_') << setw(11) << right << '|' << setw(111) << '|' << endl;

    cout << '|' << setfill(' ') << setw(10) << left << "Tech.Det." << '|' << setw(111) << right << '|' << endl;
    for (const auto& sym : ticket.symptoms)
    {
        cout << '|' << setw(11) << right << '|' << setw(110) << left << sym << '|' << endl;
    }
    cout << '|' << setfill('_') << setw(11) << right << '|' << setw(111) << '|' << endl;
}

void Analyzer::WriteToFileAllFails(ofstream& out)
{
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

void Analyzer::ExtractDuplicateIntoTheFile(const DuplicateReport& rep)
{
    ofstream out("Duplicate.txt");

    string SWver = "-------------";
    if (ArrayMessageSyms.count(-1))
    {
        SWver = ArrayMessageSyms[-1]["...Soc Version"].description;
    }
    
    out << "SW ver:" << endl;
    out << SWver << endl << endl;

    out << "Cause:" << endl;
    out << rep.type + " (" + "duplicate of " + rep.name + " - " +  MainLink + rep.name + ")" << endl << endl;

    out << "Technical details:" << endl;
    for (const auto& sym : rep.symptoms)
    {
        out << sym << endl;
    }
    out.close();
}