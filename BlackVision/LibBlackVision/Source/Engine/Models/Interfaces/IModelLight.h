#pragma once

#include "CoreDEF.h"
#include "Engine/Interfaces/IUpdatable.h"
#include "Engine/Models/Plugins/Interfaces/IParamSet.h"
#include "Engine/Interfaces/IValueSet.h"
#include "Serialization/ISerializable.h"

/**@page Lights Lights
@section LightAPI API Manipulation

@copydoc API_LightsManagment

Please keep in mind the maximal number of lights per scene is 8.

@section LightTypes Light types
We support the following Light types (see @ref bv::LightType):
- directional,
- point,
- spot.

As usual, you can manipulate the light by changing parameters with @ref API_Parameters "Parameters API".

@subsection DirectionalLight Directional Light

Parameter name         	| Type      	| Default Value     | Description
----------------------- | -------------	| ----------------- | -----------
color					| vec3			| (1,1,1)			| light's color
direction				| vec3			| (0,0,0)			| light's direction

@subsection PointLight Point Light

Parameter name         	| Type      	| Default Value     | Description
----------------------- | -------------	| ----------------- | -----------
color					| vec3			| (1,1,1)			| light's color
position				| vec3			| (0,0,0)			| light's position
attenuation             | vec3          | (1,0,0.02)        | light's attenuation

@subsection SpotLight SpotLight

Parameter name         	| Type      	| Default Value     | Description
----------------------- | -------------	| ----------------- | -----------
color                   | vec3          | (1,1,1)           | light's color
direction               | vec3          | (0,0,0)           | light's direction
position                | vec3          | (0,0,0)           | light's position
attenuation             | vec3          | (1,0,0.02)        | light's attenuation
cutOff                  | float         | 100               | cutoff
exponent                | int           | 10                | exponent


@see bv::model::IModelLight, bv::model::ModelDirectionalLight, bv::model::ModelPointLight, bv::model::ModelSpotLight
*/

namespace bv { namespace model {


/**@brief Light model interface
@copydoc Lights
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
