#include "System/BasicTypes.h"

namespace bv {

class IAICommand
{
public:

    virtual bool        TriggerCommand  ( TimeType t ) = 0;

    virtual TimeType    LastTriggerTime () const = 0;
    virtual TimeType    GetTriggerTime  () const = 0;

    virtual             ~IAICommand     () {}

};

} //bv
