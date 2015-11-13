#pragma once

#include "Engine/Events/Events.h"

namespace bv
{

class BVAppLogic;

class QueryHandlers
{
private:
    BVAppLogic*         m_appLogic;
public:
    QueryHandlers( BVAppLogic* appLogic );
    ~QueryHandlers();

    void        Info        ( bv::IEventPtr evt );

};

} //bv
