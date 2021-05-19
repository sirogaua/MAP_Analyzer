#include "Analyzer.h"
#include <windows.h>

int main()
{
    HWND console = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(console, &ConsoleRect); 
    MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 1000, 700, TRUE);

    Analyzer A;
    A.StartAnalyze();
    system("pause");
    return 0;
}