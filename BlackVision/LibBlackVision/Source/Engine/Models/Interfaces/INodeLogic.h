#pragma once

#include "CoreDEF.h"
#include "Engine/Interfaces/IUpdatable.h"
#include "Serialization/ISerializable.h"


namespace bv
{

class BVProjectEditor;
    
namespace model
{

class INodeLogic : public IUpdatable, public ISerializable
{
public:

	virtual void					Initialize		()				= 0;
	virtual void					Update			( TimeType t )	= 0;
	virtual void					Deinitialize	()				= 0;

    virtual const std::string       GetType         () const        = 0;
    virtual bool                    HandleEvent     ( IDeserializer & eventStr, ISerializer & response, BVProjectEditor * editor ) = 0;

};

DEFINE_PTR_TYPE(INodeLogic)
DEFINE_CONST_PTR_TYPE(INodeLogic)

} // model
} // bv
