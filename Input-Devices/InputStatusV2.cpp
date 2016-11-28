//Copyright (c) 2016 Meena Erian
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

#define _WIN32_WINNT 0x0600
#include <windows.h>

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

void print(char * Line, char color, unsigned x, unsigned y)
{
    unsigned LineSize = strlen(Line);
    CHAR_INFO ci[LineSize];
    SMALL_RECT lpWriteRegion;
    COORD dwBufferSize;
    COORD dwBufferCoord;
    lpWriteRegion.Top=y;
    lpWriteRegion.Bottom=y;
    lpWriteRegion.Right=x+LineSize-1;
    lpWriteRegion.Left=x;
    dwBufferSize.X=LineSize;
    dwBufferSize.Y=1;
    dwBufferCoord.X=0;
    dwBufferCoord.Y=0;
    for(int i=0; i<LineSize; i++)
    {
        ci[i].Char.AsciiChar = Line[i];
        ci[i].Attributes=color;
    }
    WriteConsoleOutputA(GetStdHandle(STD_OUTPUT_HANDLE), ci, dwBufferSize, dwBufferCoord, &lpWriteRegion);
}

void PrintHex(unsigned long value, char color, unsigned x, unsigned y)
{
    char line[9];
    line[0]='0';
    line[1]='x';
    line[2]=(((value/1048576)%16)<10)?(((value/1048576)%16)+48):(((value/1048576)%16)+55);
    line[3]=(((value/65536)%16)<10)?(((value/65536)%16)+48):(((value/65536)%16)+55);
    line[4]=(((value/4096)%16)<10)?(((value/4096)%16)+48):(((value/4096)%16)+55);
    line[5]=(((value/256)%16)<10)?(((value/256)%16)+48):(((value/256)%16)+55);
    line[6]=(((value/16)%16)<10)?(((value/16)%16)+48):(((value/16)%16)+55);
    line[7]=((value%16)<10)?((value%16)+48):((value%16)+55);
    line[8]=0;
    print(line,color,x,y);
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

const char VirtualKeys[256][40]=
{
"0x00", //0x00
"0x01 (Left mouse button)", //0x01
"0x02 (Right mouse button)", //0x02
"0x03 (Control-break processing)", //0x03
"0x04 (Middle mouse button)", //0x04
"0x05 (X1 mouse button)", //0x05
"0x06 (X2 mouse button)", //0x06
"0x07 (Undefined)", //0x07
"0x08 (BACKSPACE key)", //0x08
"0x09 (TAB key)", //0x09
"0x0A (Reserved)", //0x0A
"0x0B (Reserved)", //0x0B
"0x0C (CLEAR key)", //0x0C
"0x0D (ENTER key)", //0x0D
"0x0E (Undefined)", //0x0E
"0x0F (Undefined)", //0x0F
"0x10 (SHIFT key)", //0x10
"0x11 (CTRL key)", //0x11
"0x12 (ALT key)", //0x12
"0x13 (PAUSE key)", //0x13
"0x14 (CAPS LOCK key)", //0x14
"0x15 (IME Hangul mode or IME Kana mode)", //0x15
"0x16 (Undefined)", //0x16
"0x17 (IME Junja mode)", //0x17
"0x18 (IME final mode)", //0x18
"0x19 (IME Hanja/Kanji mode)", //0x19
"0x1A (Undefined)", //0x1A
"0x1B (ESC key)", //0x1B
"0x1C (IME convert)", //0x1C
"0x1D (IME nonconvert)", //0x1D
"0x1E (IME accept)", //0x1E
"0x1F (IME mode change request)", //0x1F
"0x20 (SPACEBAR)", //0x20
"0x21 (PAGE UP key)", //0x21
"0x22 (PAGE DOWN key)", //0x22
"0x23 (END key)", //0x23
"0x24 (HOME key)", //0x24
"0x25 (LEFT ARROW key)", //0x25
"0x26 (UP ARROW key)", //0x26
"0x27 (RIGHT ARROW key)", //0x27
"0x28 (DOWN ARROW key)", //0x28
"0x29 (SELECT key)", //0x29
"0x2A (PRINT key)", //0x2A
"0x2B (EXECUTE key)", //0x2B
"0x2C (PRINT SCREEN key)", //0x2C
"0x2D (INS key)", //0x2D
"0x2E (DEL key)", //0x2E
"0x2F (HELP key)", //0x2F
"0x30 (0 key)", //0x30
"0x31 (1 key)", //0x31
"0x32 (2 key)", //0x32
"0x33 (3 key)", //0x33
"0x34 (4 key)", //0x34
"0x35 (5 key", //0x35
"0x36 (6 key)", //0x36
"0x37 (7 key)", //0x37
"0x38 (8 key)", //0x38
"0x39 (9 key)", //0x39
"0x3A (Undefined)", //0x3A
"0x3B (Undefined)", //0x3B
"0x3C (Undefined)", //0x3C
"0x3D (Undefined)", //0x3D
"0x3E (Undefined)", //0x3E
"0x3F (Undefined)", //0x3F
"0x40 (Undefined)", //0x40
"0x41 (A key)", //0x41
"0x42 (B key)", //0x42
"0x43 (C key)", //0x43
"0x44 (D key)", //0x44
"0x45 (E key)", //0x45
"0x46 (F key)", //0x46
"0x47 (G key)", //0x47
"0x48 (H key)", //0x48
"0x49 (I key)", //0x49
"0x4A (J key)", //0x4A
"0x4B (K key)", //0x4B
"0x4C (L key)", //0x4C
"0x4D (M key)", //0x4D
"0x4E (N key)", //0x4E
"0x4F (O key)", //0x4F
"0x50 (P key)", //0x50
"0x51 (Q key)", //0x51
"0x52 (R key)", //0x52
"0x53 (S key)", //0x53
"0x54 (T key)", //0x54
"0x55 (U key)", //0x55
"0x56 (V key)", //0x56
"0x57 (W key)", //0x57
"0x58 (X key)", //0x58
"0x59 (Y key)", //0x59
"0x5A (Z key)", //0x5A
"0x5B (Left Windows key)", //0x5B
"0x5C (Right Windows key)", //0x5C
"0x5D (Applications key)", //0x5D
"0x5E (Reserved)", //0x5E
"0x5F (Computer Sleep key)", //0x5F
"0x60 (Numeric keypad 0 key)", //0x60
"0x61 (Numeric keypad 1 key)", //0x61
"0x62 (Numeric keypad 2 key)", //0x62
"0x63 (Numeric keypad 3 key)", //0x63
"0x64 (Numeric keypad 4 key)", //0x64
"0x65 (Numeric keypad 5 key)", //0x65
"0x66 (Numeric keypad 6 key)", //0x66
"0x67 (Numeric keypad 7 key)", //0x67
"0x68 (Numeric keypad 8 key)", //0x68
"0x69 (Numeric keypad 9 key)", //0x69
"0x6A (Multiply key)", //0x6A
"0x6B (Add key)", //0x6B
"0x6C (Separator key)", //0x6C
"0x6D (Subtract key)", //0x6D
"0x6E (Decimal key)", //0x6E
"0x6F (Divide key)", //0x6F
"0x70 (F1 key)", //0x70
"0x71 (F2 key)", //0x71
"0x72 (F3 key)", //0x72
"0x73 (F4 key)", //0x73
"0x74 (F5 key)", //0x74
"0x75 (F6 key)", //0x75
"0x76 (F7 key)", //0x76
"0x77 (F8 key)", //0x77
"0x78 (F9 key)", //0x78
"0x79 (F10 key)", //0x79
"0x7A (F11 key)", //0x7A
"0x7B (F12 key)", //0x7B
"0x7C (F13 key)", //0x7C
"0x7D (F14 key)", //0x7D
"0x7E (F15 key)", //0x7E
"0x7F (F16 key)", //0x7F
"0x80 (F17 key)", //0x80
"0x81 (F18 key)", //0x81
"0x82 (F19 key)", //0x82
"0x83 (F20 key)", //0x83
"0x84 (F21 key)", //0x84
"0x85 (F22 key)", //0x85
"0x86 (F23 key)", //0x86
"0x87 (F24 key)", //0x87
"0x88 (Unassigned)", //0x88
"0x89 (Unassigned)", //0x89
"0x8A (Unassigned)", //0x8A
"0x8B (Unassigned)", //0x8B
"0x8C (Unassigned)", //0x8C
"0x8D (Unassigned)", //0x8D
"0x8E (Unassigned)", //0x8E
"0x8F (Unassigned)", //0x8F
"0x90 (NUM LOCK key)", //0x90
"0x91 (SCROLL LOCK key)", //0x91
"0x92 (OEM specific)", //0x92
"0x93 (OEM specific)", //0x93
"0x94 (OEM specific)", //0x94
"0x95 (OEM specific)", //0x95
"0x96 (OEM specific)", //0x96
"0x97 (Unassigned)", //0x97
"0x98 (Unassigned)", //0x98
"0x99 (Unassigned)", //0x99
"0x9A (Unassigned)", //0x9A
"0x9B (Unassigned)", //0x9B
"0x9C (Unassigned)", //0x9C
"0x9D (Unassigned)", //0x9D
"0x9E (Unassigned)", //0x9E
"0x9F (Unassigned)", //0x9F
"0xA0 (Left SHIFT key)", //0xA0
"0xA1 (Right SHIFT key)", //0xA1
"0xA2 (Left CONTROL key)", //0xA2
"0xA3 (Right CONTROL key)", //0xA3
"0xA4 (Left ALT key)", //0xA4
"0xA5 (Right ALT key)", //0xA5
"0xA6 (Browser Back key)", //0xA6
"0xA7 (Browser Forward key)", //0xA7
"0xA8 (Browser Refresh key)", //0xA8
"0xA9 (Browser Stop key)", //0xA9
"0xAA (Browser Search key)", //0xAA
"0xAB (Browser Favorites key)", //0xAB
"0xAC (Browser Start and Home key)", //0xAC
"0xAD (Volume Mute key)", //0xAD
"0xAE (Volume Down key)", //0xAE
"0xAF (Volume Up key)", //0xAF
"0xB0 (Next Track key)", //0xB0
"0xB1 (Previous Track key)", //0xB1
"0xB2 (Stop Media key)", //0xB2
"0xB3 (Play/Pause Media key)", //0xB3
"0xB4 (Start Mail key)", //0xB4
"0xB5 (Select Media key)", //0xB5
"0xB6 (Start Application 1 key)", //0xB6
"0xB7 (Start Application 2 key)", //0xB7
"0xB8 (Reserved)", //0xB8
"0xB9 (Reserved)", //0xB9
"0xBA (OEM 1 key ';:')", //0xBA
"0xBB (OEM pluse key '+')", //0xBB
"0xBC (OEM comma key ',')", //0xBC
"0xBD (OEM minus key '-')", //0xBD
"0xBE (OEM period key '.')", //0xBE
"0xBF (OEM 2 key '/?')", //0xBF
"0xC0 (OEM 3 key '`~')", //0xC0
"0xC1 (Reserved)", //0xC1
"0xC2 (Reserved)", //0xC2
"0xC3 (Reserved)", //0xC3
"0xC4 (Reserved)", //0xC4
"0xC5 (Reserved)", //0xC5
"0xC6 (Reserved)", //0xC6
"0xC7 (Reserved)", //0xC7
"0xC8 (Reserved)", //0xC8
"0xC9 (Reserved)", //0xC9
"0xCA (Reserved)", //0xCA
"0xCB (Reserved)", //0xCB
"0xCC (Reserved)", //0xCC
"0xCD (Reserved)", //0xCD
"0xCE (Reserved)", //0xCE
"0xCF (Reserved)", //0xCF
"0xD0 (Reserved)", //0xD0
"0xD1 (Reserved)", //0xD1
"0xD2 (Reserved)", //0xD2
"0xD3 (Reserved)", //0xD3
"0xD4 (Reserved)", //0xD4
"0xD5 (Reserved)", //0xD5
"0xD6 (Reserved)", //0xD6
"0xD7 (Reserved)", //0xD7
"0xD8 (Unassigned)", //0xD8
"0xD9 (Unassigned)", //0xD9
"0xDA (Unassigned)", //0xDA
"0xDB (OEM 4 key '[{')", //0xDB
"0xDC (OEM 5 key '\\|')", //0xDC
"0xDD (OEM 6 key ']}')", //0xDD
"0xDE (OEM 7 key 'quote')", //0xDE
"0xDF (OEM 8 key )", //0xDF
"0xE0 (Reserved)", //0xE0
"0xE1 (OEM specific)", //0xE1
"0xE2 (OEM 102 '< >' or '\\')", //0xE2
"0xE3 (OEM specific)", //0xE3
"0xE4 (OEM specific)", //0xE4
"0xE5 (IME PROCESS key)", //0xE5
"0xE6 (OEM specific)", //0xE6
"0xE7 (PACKET key for unicode)", //0xE7
"0xE8 (Unassigned)", //0xE8
"0xE9 (OEM specific)", //0xE9
"0xEA (OEM specific)", //0xEA
"0xEB (OEM specific)", //0xEB
"0xEC (OEM specific)", //0xEC
"0xED (OEM specific)", //0xED
"0xEE (OEM specific)", //0xEE
"0xEF (OEM specific)", //0xEF
"0xF0 (OEM specific)", //0xF0
"0xF1 (OEM specific)", //0xF1
"0xF2 (OEM specific)", //0xF2
"0xF3 (OEM specific)", //0xF3
"0xF4 (OEM specific)", //0xF4
"0xF5 (OEM specific)", //0xF5
"0xF6 (Attn key)", //0xF6
"0xF7 (CrSel key)", //0xF7
"0xF8 (ExSel key)", //0xF8
"0xF9 (Erase EOF key)", //0xF9
"0xFA (Play key)", //0xFA
"0xFB (Zoom key)", //0xFB
"0xFC (Reserved) ", //0xFC
"0xFD (PA1 key)", //0xFD
"0xFE (Clear key)", //0xFE
"0xFE" //0xFF
};

int main()
{
    SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW); //make console window a topmost window
    POINT cp1, cp2;
    HWND w1, w2;
    COLORREF c1, c2;
    unsigned char k1[256], k2[256];
    //Line 1: Copyright
    //line 2: Mouse position
    //Line 3: Class name of foreground window
    //Line 4: Color of pixel pointed to by the mouse
    //Line 5+: a list of all pressed keys
    HDC hdc=GetDC(GetDesktopWindow());
    print("Copyright (c) 2016 Meena Erian",8,0,0);
    print("MousePos:",7,0,1);
    print("TopWindow:",7,0,2);
    print("PointedRGB:",7,0,3);
    print("Pressed keys:",7,0,4);
    SetWindowSize(40,20);
    SetConsoleTitleA("Input Status");
    cursor(20,0);
    while(1)
    {
        GetCursorPos(&cp2); //get mouse position
        w2 = GetForegroundWindow(); //get HWND of top window
        GetKeyState(0);
        GetKeyboardState(k2); //...

        if(cp2!=cp1)// Mouse moved
        {
            char line[13];
            line[0]='(';
            line[1]=((cp2.x/1000)%10)+48;
            line[2]=((cp2.x/100)%10)+48;
            line[3]=((cp2.x/10)%10)+48;
            line[4]=(cp2.x%10)+48;
            line[5]=',';
            line[6]=' ';
            line[7]=((cp2.y/1000)%10)+48;
            line[8]=((cp2.y/100)%10)+48;
            line[9]=((cp2.y/10)%10)+48;
            line[10]=(cp2.y%10)+48;
            line[11]=')';
            line[12]=0;
            print(line, 10, 10, 1);
            c2 = pGetPixel(hdc,cp2.x,cp2.y);
            if(c2!=c1) //color changed
            {
                PrintHex(c2,10,12,3);
                c1=c2;
            }
            cp1.x=cp2.x;
            cp1.y=cp2.y;
        }
        if(w2!=w1)// window switched
        {
            char ClassName[100];
            GetClassNameA(w2,ClassName,100);
            print("                   ",10,11,2);//remover any remaining characters from the previously printed class name
            print(ClassName,10,11,2);
            w1=w2;
        }

        if(k2!=k1) // key pressed/released
        {
            for(int i=5; i<15; i++)print("                                       ", 10, 0, i); //clear previously printed keys
            unsigned LineNom=5;
            for(unsigned i=0; i<256; i++)
            {
                if(k2[i]>1)
                {
                    print((char*)VirtualKeys[i], 10, 0,LineNom);
                    LineNom++;
                }
            }
            for(unsigned i=0; i<256; i++)k1[i]=k2[i];
        }
        Sleep(20);//lower the processing load
    }
}
