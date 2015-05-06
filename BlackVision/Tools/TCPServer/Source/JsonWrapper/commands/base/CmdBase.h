#pragma once
#include <string>
#include <iostream>
#include "../../json_spirit/json_spirit.h"
enum CmdType{SET_PARAM,GET_PARAM,GET_INFO,GET_VERSION,VIDEOCARD_INFO,TIMELINE,TIMER,WIDGET,STRUCT};
using namespace std;
using namespace json_spirit;
/*
const wValue& find_value( const wObject& obj, const wstring& name  )
{
    for( wObject::size_type i = 0; i != obj.size(); ++i )
    {
        const wPair& pair = obj[i];

        const wstring& _name  = pair.name_;
        if(_name==name)
        {
            return pair.value_;
        }
    }
    return  wObject::value_type::Value_type::null;
}*/


class CmdBase
{
public:
    enum CmdType Type;
    wstring CmdName;
    int ID;
    CmdBase();
    ~CmdBase();
    virtual void parse(const wObject& obj){ (obj); cout<<"nie ten parse"<<endl;}
	wstring ParseWstring(const wObject& obj, wstring key);
    
};