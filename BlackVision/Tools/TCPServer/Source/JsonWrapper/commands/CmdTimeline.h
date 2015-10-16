#include "base/CmdBase.h"
#include <string>
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/lexical_cast.hpp>
#pragma once
class CmdTimeline: public CmdBase
{
public:
    wstring  command;
    wstring  timeline_name;
    float value;
    CmdTimeline();
    ~CmdTimeline();
    void parse(const wObject& obj){
        
        command = ParseWstring( obj, L"command" );
        timeline_name = ParseWstring( obj, L"timeline_name" );
        
        for(unsigned int i=0;i<obj.size();i++)
        {
            if(obj[i].name_==L"value")
            {
                value = boost::lexical_cast<float>(ParseWstring( obj, L"value" ));
            }
        }

       
       
    }
};