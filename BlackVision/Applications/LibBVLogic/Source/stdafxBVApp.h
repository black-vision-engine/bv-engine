#pragma once

//#define DISABLE_PRECOMPILED_HEADER

#ifndef DISABLE_PRECOMPILED_HEADER

#include "CoreDEF.h"
#include "Tools/Logger/Logger.h"

#include "EndUserAPI/RemoteController.h"

#include "BVAppLogic.h"
#include "BVConfig.h"

#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/BVProjectEditor.h"
#include "Engine/Models/ModelState.h"


#include "Engine/Models/NodeLogics/NodeLogicBase.h"
#include "Engine/Models/Builder/NodeLogicHolder.h"
#include "Engine/Models/BasicNode.h"
#include "Engine/Models/NodeLogics/NodeLogicHelper.h"

#include "Engine/Events/EventHandlerHelpers.h"
#include "Engine/Events/Events.h"
#include "Serialization/SerializationHelper.h"
#include "Engine/Events/EventManager.h"

#include <map>
#include <string>
#include <vector>
#include <memory>

#undef near
#undef far
#undef max
#undef min
#undef LoadImage
#undef LoadImageW

#include "Mathematics/Interpolators/CompositeInterpolator.h"
#include "Mathematics/Interpolators/CompositeInterpolator.inl"

#endif

