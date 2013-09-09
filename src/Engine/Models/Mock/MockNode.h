#pragma once

#include <vector>

#include "MockPluginTransform.h"
#include "MockPluginShader.h"
#include "MockPluginGeometry.h"


namespace bv {

class MockNode;
class MockLayerNode;

typedef std::vector< MockLayerNode* > TMockLayerNodeVec;
typedef std::vector< MockLayerNode* > TMockNodeVec;


class MockNode
{
private:

    TMockNodeVec        m_children;
    TMockLayerNodeVec   m_layers;


};

}
