#pragma once

#include "CoreDEF.h"
#include "Engine/Interfaces/IUpdatable.h"
#include "Engine/Models/Plugins/Interfaces/IParamSet.h"
#include "Engine/Interfaces/IValueSet.h"
#include "Serialization/ISerializable.h"

/**@page Lights Lights
@see bv::model::IModelLight
*/

namespace bv { namespace model {


/**@brief Light model interface
@ingroup Model */
class IModelLight : public IUpdatable, public ISerializable, public IParamSet, public IValueSet
{
public:

    virtual LightType               GetType             () const                = 0;
    virtual void                    UpdateToCameraSpace ( const glm::mat4 & viewMat ) = 0;
};

DEFINE_PTR_TYPE(IModelLight)
DEFINE_CONST_PTR_TYPE(IModelLight)
DEFINE_UPTR_TYPE(IModelLight)

} // model
} // bv
