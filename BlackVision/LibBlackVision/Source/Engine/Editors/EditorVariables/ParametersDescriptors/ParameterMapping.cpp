#include "stdafx.h"

#include "ParameterMapping.h"

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Cameras/CameraModel.h"
#include "Engine/Models/Interfaces/IModelLight.h"



namespace bv
{

template class ParameterMapping< model::CameraModelPtr >;
template class ParameterMapping< model::IModelLightPtr >;
template class ParameterMapping< model::BasePluginPtr >;
template class ParameterMapping< model::INodeLogicPtr >;
template class ParameterMapping< model::IModelNodeEffectPtr >;


}	// bv


