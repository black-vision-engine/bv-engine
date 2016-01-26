#pragma once

#include "Engine/Interfaces/IUpdatable.h"
#include "Engine/Interfaces/IValue.h"
#include "Engine/Models/Plugins/Interfaces/IParameter.h"
#include "Engine/Graphics/Effects/NodeEffectTr/NodeEffectTypes.h"

#include "CoreDEF.h"


namespace bv { namespace model {

class IModelFullscreenEffect : public bv::IUpdatable
{
public:

    virtual NodeEffectType					        GetType			() const = 0;
    virtual const std::string &						GetName			() const = 0;

    virtual IParameterPtr                           GetParameter    ( const std::string & name ) const = 0;
    virtual const std::vector< IParameterPtr > &    GetParameters	() const = 0;

    virtual const std::vector< IValueConstPtr > &   GetValues		() const = 0;

    virtual                             ~IModelFullscreenEffect     () {};

};

DEFINE_PTR_TYPE(IModelFullscreenEffect)
DEFINE_CONST_PTR_TYPE(IModelFullscreenEffect)

} // model
} // bv
