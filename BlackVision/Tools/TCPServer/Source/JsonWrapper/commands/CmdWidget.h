#include "base/CmdBase.h"
#include <string>

#pragma once
class CmdWidget: public CmdBase
{
public:
	wstring  NodePath;
    wstring  WidgetName;
    wstring  Param;
    wstring  Action;

    void parse(const wObject& obj){
        
        NodePath = ParseWstring( obj, L"path" );
        WidgetName = ParseWstring( obj, L"widget" );
        Param = ParseWstring( obj, L"param" );
        Action = ParseWstring( obj, L"action" );
	}
	CmdWidget(void);
	~CmdWidget(void);
};

