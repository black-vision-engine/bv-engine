#pragma once

#include "System/BasicTypes.h"
#include "Engine/Interfaces/IUpdatable.h"

namespace bv { namespace model {

class INodeLogic : public IUpdatable
{
public:
	virtual void					Initialize		()				= 0;
	virtual void					Update			( TimeType t )	= 0;
	virtual void					Deinitialize	()				= 0;
};

DEFINE_PTR_TYPE(INodeLogic)
DEFINE_CONST_PTR_TYPE(INodeLogic)

} // model
} // bv