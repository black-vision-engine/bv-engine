#include "base/CmdBase.h"
#include<string>
#pragma once
class CmdVideoCardInfo: public CmdBase
{
public:
    wstring  query;
    
    
    CmdVideoCardInfo();
    ~CmdVideoCardInfo();
    void parse(const wObject& obj){
        query = ParseWstring( obj, L"query" );
        
    }
};