#pragma once

#include <string>

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/ModelScene.h"

namespace bv { namespace model {

class ModelAccessors
{
private:

public:

    static BasicNodePtr    GetRootNode  ( ModelScenePtr scene );
    static BasicNodePtr    FindChild    ( BasicNodePtr node, const std::string & name );

};

} //model
} //bv
