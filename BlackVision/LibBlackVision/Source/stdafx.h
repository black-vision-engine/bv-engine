#pragma once

//#define DISABLE_PRECOMPILED_HEADER

#ifndef DISABLE_PRECOMPILED_HEADER

#include "CoreDEF.h"
#include "Tools/Logger/Logger.h"

#include "Mathematics/glm_inc.h"
#include "Mathematics/AABB.h"
#include "Mathematics/Box.h"
#include "Mathematics/Interpolators/Interpolators.h"


#include "Assets/AssetManager.h"

#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"
#include "Serialization/ISerializable.h"
#include "Serialization/DeserializeContext.h"
#include "Serialization/BV/BVSerializeContext.h"
#include "Serialization/BV/BVDeserializeContext.h"
//#include "Serialization/BV/CloneViaSerialization.h"

#include "Serialization/SerializationHelper.h"


//#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Simple/DefaultGeometryPluginBase.h"
#include "Engine/Models/Plugins/Simple/GeometryProcessors/DefaultGeometryProcessorBase.h"


//#include "Memory/MemoryLeaks.h"
//#include <vld.h>

#include "Engine/Models/Plugins/Parameters/AbstractModelParameter.h"


#include <map>
#include <string>
#include <vector>
#include <hash_map>
#include <memory>

#endif

#undef near
#undef far
#undef max
#undef min
#undef LoadImage
#undef LoadImageW

