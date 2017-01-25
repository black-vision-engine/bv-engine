#pragma once

//#define DISABLE_PRECOMPILED_HEADER

#ifndef DISABLE_PRECOMPILED_HEADER

#include "CoreDEF.h"
#include "Tools/Logger/Logger.h"

#include "EndUserAPI/RemoteController.h"

#include "BVAppLogic.h"
#include "BVConfig.h"

#include "UseLoggerBVAppModule.h"

#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/BVProjectEditor.h"
#include "Engine/Models/ModelState.h"


#include "Widgets/NodeLogicBase.h"
#include "Engine/Models/Builder/NodeLogicHolder.h"
#include "Engine/Models/BasicNode.h"
#include "Widgets/NodeLogicHelper.h"

#include "Engine/Events/EventHandlerHelpers.h"
#include "Engine/Events/Events.h"
#include "Serialization/SerializationHelper.h"
#include "Engine/Events/EventManager.h"

#include <map>
#include <string>
#include <vector>
#include <memory>

#endif

#undef near
#undef far
#undef max
#undef min
#undef LoadImage
#undef LoadImageW

