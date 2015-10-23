#include "base/CmdBase.h"
#include <string>
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/lexical_cast.hpp>
#pragma once
class CmdGetInfo: public CmdBase
{
public:
    wstring  request;
	wstring  param;
	wstring  path;
   
    CmdGetInfo();
    ~CmdGetInfo();

    void parse(const wObject& obj){
        //request = find_value( obj, L"request" ).get_str();
		request = ParseWstring(obj, L"request");
		param = ParseWstring(obj, L"param");
		path = ParseWstring(obj, L"path");
    }
};