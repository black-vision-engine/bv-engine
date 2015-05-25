
#pragma once
#include<vector>
#include<iostream>
#include "json_spirit/json_spirit.h"
#include "commands\base\CmdBase.h"
#include "commands\CmdSetParam.h"
#include "commands\CmdGetInfo.h"
#include "commands\CmdVideoCardInfo.h"
#include "commands\CmdTimeline.h"
#include "commands\CmdTimer.h"
#include "commands\CmdWidget.h"
#include "commands\CmdStruct.h"

using namespace json_spirit;

using namespace std;



class ProtocolManager
{
private:
    
    vector<CmdBase*>             ParseObjects;
public:
    ProtocolManager(void);
    ~ProtocolManager(void);
    vector<CmdBase*>             cmdStack;
    bool                        ParseCmd        (wstring cmd);
    bool                        AssignCmd       (wstring cmdType,const wObject& obj);
    void                        RegisterParser  (CmdBase *parser);   
    void                        RegisterParsers ();   
    


};

