
//    A simple demonstrating example of using the WinAPIs GetPixel and SetPixel
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
#include <iostream>
using namespace std;

typedef unsigned long (WINAPI * PGetPixel)(HDC hdc, int x, int y);
typedef unsigned long (WINAPI * PSetPixel)(HDC hdc, int x, int y, unsigned long color);
PGetPixel pGetPixel = (PGetPixel)GetProcAddress(GetModuleHandle("Gdi32.dll"), "GetPixel");
PSetPixel pSetPixel = (PSetPixel)GetProcAddress(GetModuleHandle("Gdi32.dll"), "SetPixel");
#define GetPixel pGetPixel
#define SetPixel pSetPixel



bool operator != (const POINT P1,const POINT P2)
{
    return P1.x!=P2.x || P1.y!=P2.y;
}


void PrintPixel(COLORREF c, int x, int y, int Scale=1)
{
    x*=Scale;
    y*=Scale;
    HDC dc = GetDC(GetConsoleWindow());
    for(int y1=y; y1<y+Scale; y1++)
    {
        for(int x1=x; x1<x+Scale; x1++)
        {
            SetPixel(dc, x1,y1,c);
        }
    }
}


int main()
{
    cout<<"Point your mouse on a colored area on the screen and press Enter\n";
    POINT cp;
    while(1)
    {
        if(GetAsyncKeyState(VK_RETURN))
        {
            system("cls");
            GetCursorPos(&cp);
            COLORREF c[21][21];
            cout<<"Saving image...\n";
            HDC dc = GetDC(GetDesktopWindow());
            for(int y=-10; y<11; y++)
            {
                for(int x=-10; x<11; x++)
                {
                    c[y+10][x+10] = GetPixel(dc,cp.x+x,cp.y+y);
                }
            }
            cout<<"Printing...\n";
            for(int y=-10; y<11; y++)
            {
                for(int x=-10; x<11; x++)
                {
                    PrintPixel(c[y+10][x+10],x+10,y+10,20);
                }
            }
            cout<<"Complete.\n";
        }
        Sleep(50);
    }
}
