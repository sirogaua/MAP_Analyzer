#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int main()
{
    vector<int> nums_blocks_with_problems;
    ifstream mem("MAP.txt");

    if (!mem.is_open())
    {
        cout << "No file with name \"MAP.txt\"" << endl;
        system("pause");
        return 0;
    }
    

    string str = "";
    while (getline(mem, str))
    {
        stringstream s(str);
        int block_num;
        string offset = "";
        string pages = "";

        s >> block_num >> offset >> pages;

        if(pages.find('e') != string::npos || pages.find('X') != string::npos)
        {
            nums_blocks_with_problems.push_back(block_num);
        }

        //cout << block_num << " " << offset << " " << pages << endl;
    }
    mem.close();

    if (nums_blocks_with_problems.empty())
    {
        cout << "ALL GOOD, none problems!" << endl;
    }
    else
    {
        cout << "Numbers of blocks where was found problem: " << endl;
        for (const auto& item : nums_blocks_with_problems)
        {
            cout << item << endl;
        }
    }
    system("pause");
    return 0;
}