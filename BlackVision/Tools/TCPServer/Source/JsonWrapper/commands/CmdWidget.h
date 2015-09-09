#include "base/CmdBase.h"
#include <string>
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/lexical_cast.hpp>

#pragma once
class CmdWidget: public CmdBase
{
public:
	wstring  NodePath;
    wstring  WidgetName;
    wstring  Param;
    wstring  Action;
	wstring  Value;

    float	 Time;

    void parse(const wObject& obj){
        
		std::wstring wtime = ParseWstring( obj, L"time" );
		
		try{
			Time = boost::lexical_cast<float>(wtime);
		}catch(boost::bad_lexical_cast&)
		{
			Time = 0.0f;
		}


		Value = ParseWstring( obj, L"value" );
        NodePath = ParseWstring( obj, L"path" );
        WidgetName = ParseWstring( obj, L"widget" );
        Param = ParseWstring( obj, L"param" );
        Action = ParseWstring( obj, L"action" );
	}
	CmdWidget(void);
	~CmdWidget(void);
};

