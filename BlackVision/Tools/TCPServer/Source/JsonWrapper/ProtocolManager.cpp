#include <string>
#include "ProtocolManager.h"

#include "json_spirit/json_spirit.h"
using namespace json_spirit;

const mValue& find_value( const mObject& obj, const string& name  )
{
    mObject::const_iterator i = obj.find( name );

    assert( i != obj.end() );
    assert( i->first == name );

    return i->second;
}
void ProtocolManager::RegisterParser  (CmdBase *parser)
{
    ParseObjects.push_back(parser);
}
void ProtocolManager::RegisterParsers ()
{
    CmdBase *cmd = new CmdSetParam();
    RegisterParser(cmd);

    cmd = new CmdVideoCardInfo();
    RegisterParser(cmd);

    cmd = new CmdGetInfo();
    RegisterParser(cmd);

    cmd  = new CmdTimeline();
    RegisterParser(cmd);

    cmd  = new CmdTimer();
    RegisterParser(cmd);

	cmd  = new CmdWidget();
    RegisterParser(cmd);

}

ProtocolManager::ProtocolManager(void)
{
    
    RegisterParsers();
}


ProtocolManager::~ProtocolManager(void)
{
}


bool ProtocolManager::ParseCmd        (wstring cmd)
{
    wValue value;
    read(cmd,value);
    
    

    const wArray& addr_array = value.get_array();
    
    for(unsigned int i = 0 ; i < addr_array.size(); i++)
    {
        const wObject& obj = addr_array[i].get_obj();   
        std::wstring cmd = find_value(obj,L"cmd").get_str();
        AssignCmd(cmd,obj);
    }
    return true;
}

bool ProtocolManager::AssignCmd     (wstring cmdType,const wObject& obj)
{
    CmdBase *parser=NULL;

    for(unsigned int i=0 ; i < ParseObjects.size() ; i++)
    {
        if(ParseObjects[i]->CmdName == cmdType)
        {
            parser = ParseObjects[i];
            break;
        }
    }
    if(parser == NULL)
        return false;

    parser->parse(obj);
    cmdStack.push_back((CmdBase*)parser);
    return true;
}