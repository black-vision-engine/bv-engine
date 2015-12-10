#pragma once

#include "Engine/Models/ModelNodeEditor.h"

#include <string>

namespace bv
{
class BVAppLogic;

model::IModelNodePtr    GetNode     ( BVAppLogic* appLogic, const std::string& sceneName, const std::string& nodeName );
model::IModelNodePtr    GetRootNode ( BVAppLogic* appLogic, const std::string& sceneName );


} //bv

