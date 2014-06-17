#pragma once

#include <string>

#include "Engine/Models/BasicNode.h"

namespace bv { namespace model {

class BasicNode;
class ModelScene;

class ModelAccessors
{
private:

public:

    static BasicNodePtr    GetRootNode  ( ModelScene * scene );
    static BasicNodePtr    FindChild    ( BasicNodePtr node, const std::string & name );
    static BasicNodePtr    FindLayer    ( BasicNodePtr node, const std::string & name );

};

} //model
} //bv
