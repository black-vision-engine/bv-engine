#include "CmdTimer.h"


CmdTimer::CmdTimer(void)
{
        Type    =   CmdType::TIMER;
        CmdName =   L"TIMER";
        H=M=S=MS=0;
}


CmdTimer::~CmdTimer(void)
{

}
