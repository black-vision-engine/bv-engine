#include "base/CmdBase.h"
#include <string>
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/lexical_cast.hpp>
#pragma once
class CmdTimer: public CmdBase
{
public:

    wstring  command;
    wstring  NodeName;
    float H;
    float M;
    float S;
    float MS;

    CmdTimer();
    ~CmdTimer();
    void parse(const wObject& obj){
        
        command = ParseWstring( obj, L"command" );
        NodeName = ParseWstring( obj, L"node_name" );

        
        for(unsigned int i=0;i<obj.size();i++)
        {
            if(obj[i].name_==L"h")
            {
                H = boost::lexical_cast<float>(ParseWstring( obj, L"h" ));
            }
            if(obj[i].name_==L"m")
            {
                M = boost::lexical_cast<float>(ParseWstring( obj, L"m" ));
            }
            if(obj[i].name_==L"s")
            {
                S = boost::lexical_cast<float>(ParseWstring( obj, L"s" ));
            }
            if(obj[i].name_==L"ms")
            {
                MS = boost::lexical_cast<float>(ParseWstring( obj, L"ms" ));
            }
        }
    }
};