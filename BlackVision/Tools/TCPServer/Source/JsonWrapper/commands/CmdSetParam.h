#include "base/CmdBase.h"
#include <string>
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/lexical_cast.hpp>
#pragma once
class CmdSetParam: public CmdBase
{
public:
    wstring  path;
    wstring  plugin;
    wstring  param;
    wstring  value;

    float time;
	float x;
    float y;
    float z;

    CmdSetParam();
    ~CmdSetParam();
    void parse(const wObject& obj){
        
        path = ParseWstring( obj, L"path" );
        plugin = ParseWstring( obj, L"plugin" );
        param = ParseWstring( obj, L"param" );
        value = ParseWstring( obj, L"value" );
		
		wstring wtime = ParseWstring( obj, L"time" );
		
		try{
			time =  boost::lexical_cast<float>(wtime);
		}catch(boost::bad_lexical_cast&)
		{
			time =0.0f;
		}

        if(plugin==L"transform")
        {
             std::vector<std::wstring> tok;
             split(tok, value, boost::is_any_of(L" "));
             int count=0;
             for(std::vector<std::wstring>::iterator tok_iter = tok.begin(); tok_iter != tok.end(); ++tok_iter)
             {
                    wstring s = *tok_iter;
                    if(count == 0)
                    {
                        x = boost::lexical_cast<float>(s);
                    }
                    if(count == 1)
                    {
                       y = boost::lexical_cast<float>(s);
                    }
                    if(count == 2)
                    {
                        z = boost::lexical_cast<float>(s);
                    }
                    count++;
             }
        }
    }
};