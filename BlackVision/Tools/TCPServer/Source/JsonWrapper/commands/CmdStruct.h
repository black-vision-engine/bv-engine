#include "base/CmdBase.h"
#include <string>

#pragma once
class CmdStruct: public CmdBase
{
public:
	wstring  Command; // ADD_NODE // REMOVE_NODE // RENAME_NODE // ADD_PLUGIN // ADD_WIDGET // REMOVE_WIDGET // REMOVE_PLUGIN // COPY_NODE // MOVE_NODE // COPY_PLUGIN // ADD_TIMELINE // ADD_KEYFRAME // REMOVE_TIMELINE // REMOVE_KEYFRAME // CHANGE_KEYFRAME_POS // ASSIGN_PROPERTY_TO_TIMELINE // CHANGE_TIMELINE_INTERPOLATION_METHOD // CHANGE_TIMELINE_TYPE // CHANGE_KEYFRAME_TYPE // CHANGE_KEYFRAME_NAME

	wstring NodeParam1;
	wstring NodeParam2;
	wstring PluginName;
	wstring ParamName;
	wstring TimeLineName;
	wstring WidgetName;

    void parse(const wObject& obj){
        
        Command = ParseWstring( obj, L"command" );
		NodeParam1 = ParseWstring( obj, L"node" );
		NodeParam2 = ParseWstring( obj, L"node2" );
	}
	CmdStruct(void);
	~CmdStruct(void);
};

