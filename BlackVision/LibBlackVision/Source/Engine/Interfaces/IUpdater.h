#pragma once

#include "CoreDEF.h"

#include "Engine/Models/Updaters/EngineResources.h"



namespace bv
{

// ***********************
//
class IUpdater
{
public:

    virtual         ~IUpdater   () {}

    virtual void                    DoUpdate    ( EngineResources & resources ) = 0;

};

DEFINE_PTR_TYPE(IUpdater)
DEFINE_CONST_PTR_TYPE(IUpdater)


// ***********************
//
class IStateUpdater
{
public:

    virtual void                    DoUpdate    () = 0;

    virtual const std::string &     GetName     () = 0;
};

DEFINE_PTR_TYPE(IStateUpdater)
DEFINE_CONST_PTR_TYPE(IStateUpdater)


} // bv
