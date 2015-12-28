#pragma once

#include "CoreDEF.h"
#include "Engine/Interfaces/IUpdatable.h"
#include "Engine/Types/Enums.h"
#include "Serialization/ISerializable.h"
#include "Engine/Interfaces/IValue.h"
#include "Engine/Models/Plugins/Interfaces/IParameter.h"

namespace bv { namespace model {

class IModelNodeEffect : public bv::IUpdatable, public ISerializable
{
public:

    virtual NodeEffectType								GetType			() const = 0;

    virtual IParameterPtr                               GetParameter    ( const std::string & name ) const = 0;

    virtual const std::vector< bv::IValueConstPtr > &   GetValues		() const = 0;

    virtual                 ~IModelNodeEffect   () {};

};

DEFINE_PTR_TYPE(IModelNodeEffect)
DEFINE_CONST_PTR_TYPE(IModelNodeEffect)

} // model
} // bv
