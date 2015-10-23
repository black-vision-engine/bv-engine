#pragma once

#include <iostream>
#include "win_sock.h"
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string>
#include <time.h> 
#include <vector>


using namespace std;
class LogMsg
{
public:
    LogMsg(wstring _type,wstring _tag,wstring _msg)
    {
        type = _type;
        tag = _tag;
        msg = _msg;
    }
    wstring type;
    wstring tag;
    wstring msg;
};

class Log
{
private:
    static bool ConsoleOutput;
	static CRITICAL_SECTION CriticalSection; 
public:
    static vector<LogMsg> Logs;
    Log(void);
    ~Log(void);
    static SOCKET socket;
    static void Connect();
    static void ConnectThread();
    static void EnableConsoleOutput(){ConsoleOutput=true;}
    static wstring C(string s){return std::wstring(s.begin(),s.end());}
    static void LogMessage(wstring type,wstring tag,wstring msg);
    static void A(wstring msg){LogMessage(L"default",L"test", msg);}
    static void A(wstring type, wstring msg){LogMessage(type,L"test",msg);}
    static void A(wstring type, wstring tag, wstring msg){LogMessage(type,tag,msg);}
    static void A(string msg){LogMessage(L"default",L"test", C(msg));}
    static void A(string type, string msg){LogMessage(C(type),L"test",C(msg));}
    static void A(string type, string tag, string msg){LogMessage(C(type),C(tag),C(msg));}
    static void P(wstring msg){LogMessage(L"plugins",L"test", msg);}
    static void P(wstring type, wstring msg){LogMessage(type,L"test",msg);}
    static void P(wstring type, wstring tag, wstring msg){LogMessage(type,tag,msg);}
    static void P(string msg){LogMessage(L"plugins",L"test", C(msg));}
    static void P(string type, string msg){LogMessage(C(type),L"test",C(msg));}
    static void P(string type, string tag, string msg){LogMessage(C(type),C(tag),C(msg));}


};

