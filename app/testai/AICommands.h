#include "testai/IAICommand.h"

namespace bv {

class AICommandBase : public IAICommand
{
private:

    TimeType    m_triggerTime;
    TimeType    m_lastTriggerTime;

public:

    virtual TimeType    LastTriggerTime () const override;
    virtual TimeType    GetTriggerTime  () const override;

};

class AICommandStart : public AICommandBase
{
private:

public:


};

class AICommandStop : public AICommandBase
{
};

class AICommandReverse : public AICommandBase
{
};

} //bv

