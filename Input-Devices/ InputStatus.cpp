//    A demonstrating program that displays the mouse positione, window class
//     of the active window, GRB value of the pixel pointed to by the mouse
//     and a list of all pressed virtual keys.
//
//    Copyright (C) 2016  Meena Erian
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#define _WIN32_WINNT 0x0600
#include <windows.h>
#include <string.h>
#include <iostream>
using namespace std;

HANDLE soh = GetStdHandle(STD_OUTPUT_HANDLE);

typedef unsigned long (WINAPI * PGetPixel)(HDC hdc, int x, int y);
typedef unsigned long (WINAPI * PSetPixel)(HDC hdc, int x, int y, unsigned long color);
PGetPixel pGetPixel = (PGetPixel)GetProcAddress(GetModuleHandle("Gdi32.dll"), "GetPixel");
PSetPixel pSetPixel = (PSetPixel)GetProcAddress(GetModuleHandle("Gdi32.dll"), "SetPixel");

template<typename t>
bool operator==(const t& o1,const t& o2)
{
    return !memcmp(&o1,&o2, sizeof(t));
}

template<typename t>
bool operator!=(const t& o1,const t& o2)
{
    return memcmp(&o1,&o2, sizeof(t));
}

bool SetCCP(int colom, int row)
{
    COORD c;
    c.X=colom;
    c.Y=row;
    return SetConsoleCursorPosition(soh,c);
}

bool SetWindowSize(int x, int y)
{
    SMALL_RECT nw;
    nw.Top=0;
    nw.Bottom=y-1;
    nw.Right=x-1;
    nw.Left =0;
    return SetConsoleWindowInfo(soh,1,&nw);
}

bool cursor(DWORD dwSize, BOOL bVisible)
{
    _CONSOLE_CURSOR_INFO cci;
    cci.dwSize=dwSize;
    cci.bVisible=bVisible;
    return SetConsoleCursorInfo(soh,&cci);
}

void color(char BackGround, char ForeGround)
{
    WORD c=(ForeGround+(BackGround*16));
    SetConsoleTextAttribute(soh,c);
}

const string VirtualKeys[256]=
{
"", //0x00
"Left mouse button", //0x01
"Right mouse button", //0x02
"Control-break processing", //0x03
"Middle mouse button", //0x04
"X1 mouse button", //0x05
"X2 mouse button", //0x06
"Undefined", //0x07
"BACKSPACE key", //0x08
"TAB key", //0x09
"Reserved", //0x0A
"Reserved", //0x0B
"CLEAR key", //0x0C
"ENTER key", //0x0D
"Undefined", //0x0E
"Undefined", //0x0F
"SHIFT key", //0x10
"CTRL key", //0x11
"ALT key", //0x12
"PAUSE key", //0x13
"CAPS LOCK key", //0x14
"IME Hangul mode or IME Kana mode", //0x15
"Undefined", //0x16
"IME Junja mode", //0x17
"IME final mode", //0x18
"IME Hanja/Kanji mode", //0x19
"Undefined", //0x1A
"ESC key", //0x1B
"IME convert", //0x1C
"IME nonconvert", //0x1D
"IME accept", //0x1E
"IME mode change request", //0x1F
"SPACEBAR", //0x20
"PAGE UP key", //0x21
"PAGE DOWN key", //0x22
"END key", //0x23
"HOME key", //0x24
"LEFT ARROW key", //0x25
"UP ARROW key", //0x26
"RIGHT ARROW key", //0x27
"DOWN ARROW key", //0x28
"SELECT key", //0x29
"PRINT key", //0x2A
"EXECUTE key", //0x2B
"PRINT SCREEN key", //0x2C
"INS key", //0x2D
"DEL key", //0x2E
"HELP key", //0x2F
"0 key", //0x30
"1 key", //0x31
"2 key", //0x32
"3 key", //0x33
"4 key", //0x34
"5 key", //0x35
"6 key", //0x36
"7 key", //0x37
"8 key", //0x38
"9 key", //0x39
"Undefined", //0x3A
"Undefined", //0x3B
"Undefined", //0x3C
"Undefined", //0x3D
"Undefined", //0x3E
"Undefined", //0x3F
"Undefined", //0x40
"A key", //0x41
"B key", //0x42
"C key", //0x43
"D key", //0x44
"E key", //0x45
"F key", //0x46
"G key", //0x47
"H key", //0x48
"I key", //0x49
"J key", //0x4A
"K key", //0x4B
"L key", //0x4C
"M key", //0x4D
"N key", //0x4E
"O key", //0x4F
"P key", //0x50
"Q key", //0x51
"R key", //0x52
"S key", //0x53
"T key", //0x54
"U key", //0x55
"V key", //0x56
"W key", //0x57
"X key", //0x58
"Y key", //0x59
"Z key", //0x5A
"Left Windows key", //0x5B
"Right Windows key", //0x5C
"Applications key", //0x5D
"Reserved", //0x5E
"Computer Sleep key", //0x5F
"Numeric keypad 0 key", //0x60
"Numeric keypad 1 key", //0x61
"Numeric keypad 2 key", //0x62
"Numeric keypad 3 key", //0x63
"Numeric keypad 4 key", //0x64
"Numeric keypad 5 key", //0x65
"Numeric keypad 6 key", //0x66
"Numeric keypad 7 key", //0x67
"Numeric keypad 8 key", //0x68
"Numeric keypad 9 key", //0x69
"Multiply key", //0x6A
"Add key", //0x6B
"Separator key", //0x6C
"Subtract key", //0x6D
"Decimal key", //0x6E
"Divide key", //0x6F
"F1 key", //0x70
"F2 key", //0x71
"F3 key", //0x72
"F4 key", //0x73
"F5 key", //0x74
"F6 key", //0x75
"F7 key", //0x76
"F8 key", //0x77
"F9 key", //0x78
"F10 key", //0x79
"F11 key", //0x7A
"F12 key", //0x7B
"F13 key", //0x7C
"F14 key", //0x7D
"F15 key", //0x7E
"F16 key", //0x7F
"F17 key", //0x80
"F18 key", //0x81
"F19 key", //0x82
"F20 key", //0x83
"F21 key", //0x84
"F22 key", //0x85
"F23 key", //0x86
"F24 key", //0x87
"Unassigned", //0x88
"Unassigned", //0x89
"Unassigned", //0x8A
"Unassigned", //0x8B
"Unassigned", //0x8C
"Unassigned", //0x8D
"Unassigned", //0x8E
"Unassigned", //0x8F
"NUM LOCK key", //0x90
"SCROLL LOCK key", //0x91
"OEM specific", //0x92
"OEM specific", //0x93
"OEM specific", //0x94
"OEM specific", //0x95
"OEM specific", //0x96
"Unassigned", //0x97
"Unassigned", //0x98
"Unassigned", //0x99
"Unassigned", //0x9A
"Unassigned", //0x9B
"Unassigned", //0x9C
"Unassigned", //0x9D
"Unassigned", //0x9E
"Unassigned", //0x9F
"Left SHIFT key", //0xA0
"Right SHIFT key", //0xA1
"Left CONTROL key", //0xA2
"Right CONTROL key", //0xA3
"Left ALT key", //0xA4
"Right ALT key", //0xA5
"Browser Back key", //0xA6
"Browser Forward key", //0xA7
"Browser Refresh key", //0xA8
"Browser Stop key", //0xA9
"Browser Search key", //0xAA
"Browser Favorites key", //0xAB
"Browser Start and Home key", //0xAC
"Volume Mute key", //0xAD
"Volume Down key", //0xAE
"Volume Up key", //0xAF
"Next Track key", //0xB0
"Previous Track key", //0xB1
"Stop Media key", //0xB2
"Play/Pause Media key", //0xB3
"Start Mail key", //0xB4
"Select Media key", //0xB5
"Start Application 1 key", //0xB6
"Start Application 2 key", //0xB7
"Reserved", //0xB8
"Reserved", //0xB9
"OEM 1 key ';:'", //0xBA
"OEM pluse key '+'", //0xBB
"OEM comma key ','", //0xBC
"OEM minus key '-'", //0xBD
"OEM period key '.'", //0xBE
"OEM 2 key '/?'", //0xBF
"OEM 3 key '`~'", //0xC0
"Reserved", //0xC1
"Reserved", //0xC2
"Reserved", //0xC3
"Reserved", //0xC4
"Reserved", //0xC5
"Reserved", //0xC6
"Reserved", //0xC7
"Reserved", //0xC8
"Reserved", //0xC9
"Reserved", //0xCA
"Reserved", //0xCB
"Reserved", //0xCC
"Reserved", //0xCD
"Reserved", //0xCE
"Reserved", //0xCF
"Reserved", //0xD0
"Reserved", //0xD1
"Reserved", //0xD2
"Reserved", //0xD3
"Reserved", //0xD4
"Reserved", //0xD5
"Reserved", //0xD6
"Reserved", //0xD7
"Unassigned", //0xD8
"Unassigned", //0xD9
"Unassigned", //0xDA
"OEM 4 key '[{'", //0xDB
"OEM 5 key '\\|'", //0xDC
"OEM 6 key ']}'", //0xDD
"OEM 7 key 'quote'", //0xDE
"OEM 8 key ", //0xDF
"Reserved", //0xE0
"OEM specific", //0xE1
"OEM 102 '< >' or '\\'", //0xE2
"OEM specific", //0xE3
"OEM specific", //0xE4
"IME PROCESS key", //0xE5
"OEM specific", //0xE6
"PACKET key for unicode", //0xE7
"Unassigned", //0xE8
"OEM specific", //0xE9
"OEM specific", //0xEA
"OEM specific", //0xEB
"OEM specific", //0xEC
"OEM specific", //0xED
"OEM specific", //0xEE
"OEM specific", //0xEF
"OEM specific", //0xF0
"OEM specific", //0xF1
"OEM specific", //0xF2
"OEM specific", //0xF3
"OEM specific", //0xF4
"OEM specific", //0xF5
"Attn key", //0xF6
"CrSel key", //0xF7
"ExSel key", //0xF8
"Erase EOF key", //0xF9
"Play key", //0xFA
"Zoom key", //0xFB
"Reserved ", //0xFC
"PA1 key", //0xFD
"Clear key", //0xFE
"" //0xFF
};

int main()
{
    SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW); //make console window a topmost window
    POINT cp1, cp2;
    HWND w1, w2;
    COLORREF c1, c2;
    unsigned char k1[256], k2[256];
    //line 1: Mouse position
    //Line 2: Class name of foreground window
    //Line 3: Color of pixel pointed to by the mouse
    //Line 4+: a list of all pressed keys
    HDC hdc=GetDC(GetDesktopWindow());

    color(0,7);
    cout<<"MousePos:\n";
    cout<<"TopWindow:\n";
    cout<<"PointedRGB:\n";
    cout<<"Pressed keys:";
    SetWindowSize(40,20);
    SetConsoleTitleA("Input Status");
    cursor(20,0);
    color(0,10);
    while(1)
    {
        GetCursorPos(&cp2); //get mouse position
        w2 = GetForegroundWindow(); //get HWND of top window
        GetKeyState(0);
        GetKeyboardState(k2); //...

        if(cp2!=cp1)// Mouse moved
        {
            SetCCP(10,0);
            cout<<"("<<cp2.x<<", "<<cp2.y<<")         ";
            c2 = pGetPixel(hdc,cp2.x,cp2.y);
            if(c2!=c1) //color changed
            {
                SetCCP(12,2);
                cout<<(HWND)c2<<"       ";
                c1=c2;
            }
            cp1.x=cp2.x;
            cp1.y=cp2.y;
        }
        if(w2!=w1)// window switched
        {
            SetCCP(11,1);
            char ClassName[100];
            GetClassNameA(w2,ClassName,100);
            cout<<ClassName<<"                 ";
            w1=w2;
        }

        if(k2!=k1) // key pressed/released
        {
            SetCCP(0,4);
            for(unsigned i=0; i<256; i++)
            {
                if(k2[i]>1)
                {
                    cout<<(HWND)i<<" ("<<VirtualKeys[i]<<")                    \n";
                }
            }
            cout<<"                                     \n";
            cout<<"                                     \n";
            cout<<"                                     \n";
            cout<<"                                     \n";
            cout<<"                                     \n";
            cout<<"                                     \n";
            cout<<"                                     \n";
            cout<<"                                     \n";
            for(unsigned i=0; i<256; i++)k1[i]=k2[i];
        }
        Sleep(20);
    }
}
