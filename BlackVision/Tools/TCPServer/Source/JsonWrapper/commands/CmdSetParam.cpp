#pragma warning(disable :4503)
#pragma warning(disable :4996)
#include "CmdSetParam.h"


CmdSetParam::CmdSetParam(void)
{
        Type    =   CmdType::SET_PARAM;
        CmdName =   L"SET_PARAM";
        x=z=y=0;
}


CmdSetParam::~CmdSetParam(void)
{

}
