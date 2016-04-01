#pragma once

#include "CoreDEF.h"
#include "Engine/Interfaces/IUpdatable.h"
#include "Engine/Models/Plugins/Interfaces/IParamSet.h"
#include "Engine/Interfaces/IValueSet.h"
#include "Serialization/ISerializable.h"


namespace bv { namespace model {


class IModelLight : public IUpdatable, public ISerializable, public IParamSet, public IValueSet
{
public:

    virtual std::string             GetTypeName     () const        = 0;
    virtual LightType               GetType         () const        = 0;

};

DEFINE_UPTR_TYPE(IModelLight)

} // model
} // bv
