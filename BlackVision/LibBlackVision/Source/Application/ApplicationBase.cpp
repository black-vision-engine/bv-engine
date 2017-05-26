#include "stdafx.h"

#include "ApplicationBase.h"

#include <cassert>

#include "System/InitSubsystem.h"

bv::ApplicationBase * bv::ApplicationBase::ApplicationInstance		= nullptr;
bv::ApplicationBase::ApplicationMain bv::ApplicationBase::MainFun	= nullptr;




#include "Memory/MemoryLeaks.h"



namespace bv {

// *********************************
//
ApplicationBase::ApplicationBase	()
{
}

// *********************************
//
ApplicationBase::~ApplicationBase	()
{
}

} //bv


