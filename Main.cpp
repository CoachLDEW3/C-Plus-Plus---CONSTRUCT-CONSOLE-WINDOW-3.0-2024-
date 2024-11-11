/**************************************************************************************************************************************************
 Name:		CoachLDEW
 Date:		11.11.2024
 Purpose:	This program allows the user to construct a console window to a specific size based on characters in a row and in a column as well as
            adjust the size of a character in pixels, select a font to use in the console and allows you to add title to the title bar
            Font List:
            Cascadia Code               Cascadia Code SemiLight     Cascadia Mono SemiBold      Lucida Console
            Cascadia Code ExtraLight    Cascadia Mono               Cascadia Mono SemiLight     MS Gothic
            Cascadia Code Light         Cascadia Mono ExtraLight    Consolas                    NSimSun
            Cascadia Code SemiBold      Cascadia Mono Light         Courier New                 SimSun-ExtB
**************************************************************************************************************************************************/
#include <iostream>			//This library contains standard input and output processes
#include <Windows.h>		//This library contains the function to control the cursor position
#include <string>           //many functions that allow you to perform tasks on strings.


using namespace std;


void ConstructConsole(int width, int height, int fontW, int fontH, string fontName, string titleName)
{
    int screenWidth = 0;
    int screenHeight = 0;

    string title;

    //Initializing an object of wstring
    wstring tempTitle = wstring(titleName.begin(), titleName.end());

    //Applying c_str() method on temp
    LPCWSTR wideStringTitleName = tempTitle.c_str();

    //Places a title in the Title Bar;
    //SetConsoleTitle(L"Constuct Console");
    SetConsoleTitle(wideStringTitleName);

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

    if (hOut == INVALID_HANDLE_VALUE)
        cout << "Bad Handle" << endl;

    screenWidth = width;
    screenHeight = height;

    // Change console visual size to a minimum so ScreenBuffer can shrink below the actual visual size
    SMALL_RECT sRECTWindow = { 0, 0, 1, 1 };
    SetConsoleWindowInfo(hOut, TRUE, &sRECTWindow);

    // Set the size of the screen buffer
    COORD coord = { (short)screenWidth, (short)screenHeight };
    if (!SetConsoleScreenBufferSize(hOut, coord))
        cout << "SetConsoleScreenBufferSize" << endl;

    // Assign screen buffer to the console
    if (!SetConsoleActiveScreenBuffer(hOut))
        cout << "SetConsoleActiveScreenBuffer" << endl;

    string font;

    //Initializing an object of wstring
    wstring temp = wstring(fontName.begin(), fontName.end());

    //Applying c_str() method on temp
    LPCWSTR wideStringFontName = temp.c_str();

    // Set the font size now that the screen buffer has been assigned to the console
    CONSOLE_FONT_INFOEX cfi;
    //Change the font
    lstrcpyW(cfi.FaceName, wideStringFontName);
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = fontW;
    cfi.dwFontSize.Y = fontH;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;

    if (!SetCurrentConsoleFontEx(hOut, false, &cfi))
        cout << "SetCurrentConsoleFontEx" << endl;

    //Get screen buffer info and check the maximum allowed window size. Return error if exceeded, so user knows their dimensions/fontsize 
    //are too large
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hOut, &csbi))
        cout << "GetConsoleScreenBufferInfo" << endl;
    if (screenHeight > csbi.dwMaximumWindowSize.Y)
        cout << "Screen Height / Font Height Too Big" << endl;
    if (screenWidth > csbi.dwMaximumWindowSize.X)
        cout << "Screen Width / Font Width Too Big" << endl;

    //Set Physical Console Window Size
    sRECTWindow = { 0, 0, (short)(screenWidth - 1), (short)(screenHeight - 1) };
    if (!SetConsoleWindowInfo(hOut, TRUE, &sRECTWindow))
        cout << "SetConsoleWindowInfo" << endl;

    //Set flags to allow mouse input		
    if (!SetConsoleMode(hIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
        cout << "SetConsoleMode" << endl;

    //Allocate memory for screen buffer
    CHAR_INFO* screenBuffer = new CHAR_INFO[screenWidth * screenHeight];
    memset(screenBuffer, 0, sizeof(CHAR_INFO) * screenWidth * screenHeight);

    //SetConsoleCtrlHandler((PHANDLER_ROUTINE)CloseHandler, TRUE);
    //return 1;
}//End ConstructConsole()


int main()
{
    ConstructConsole(60, 30, 8, 16, "MS Gothic", "Construct Console");

    cout << "This is a test" << endl;

    bool gameOver = false;
    while (!gameOver)
    {
        //Hit the escape key to end the program
        if (GetAsyncKeyState(VK_ESCAPE)) gameOver = true;

    }

    return 0;
}