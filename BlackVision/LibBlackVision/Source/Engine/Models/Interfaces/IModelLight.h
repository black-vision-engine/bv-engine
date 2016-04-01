#pragma once

#include "CoreDEF.h"
#include "Engine/Interfaces/IUpdatable.h"
#include "Serialization/ISerializable.h"


namespace bv { namespace model {


    class IModelLight : public IUpdatable, public ISerializable, public IParamSet, public IValueSet
{
public:

    virtual LightType           GetType         () const        = 0;

};

DEFINE_UPTR_TYPE(IModelLight)

} // model
} // bv
