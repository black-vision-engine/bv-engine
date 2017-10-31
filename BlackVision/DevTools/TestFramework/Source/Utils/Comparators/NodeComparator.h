#pragma once

#include "Engine/Models/BasicNode.h"

namespace bv
{

namespace model {

class NodeComparator
{
public:
    static bool         CompareStructure    ( const BasicNode & expected, const BasicNode & actual )
    {
        auto expectedPlugins = expected.GetPluginList();
        auto actualPlugins = actual.GetPluginList();
        if( expectedPlugins->NumPlugins() != actualPlugins->NumPlugins() )
            return false;
        return true;
    }
};

}

}	// bv