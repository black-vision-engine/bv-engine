#pragma once

#include <string>

namespace bv { namespace model {

class BasicNode;
class ModelScene;

class ModelAccessors
{
private:

public:

    static BasicNode *     GetRootNode  ( ModelScene * scene );
    static BasicNode *     FindChild    ( BasicNode * node, const std::string & name );
    static BasicNode *     FindLayer    ( BasicNode * node, const std::string & name );

};

} //model
} //bv
