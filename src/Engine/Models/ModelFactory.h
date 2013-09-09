#pragma once

#include "Mathematics\Transform\MatTransform.h"

namespace bv
{

//forward declarations
class IGeometryGenPlugin;
class PluginGeometryRect;
class ITransformPlugin;
class PluginTransformSimple;
class IShaderPlugin;
class BasicNode;

class ModelFactory
{
public:
    static PluginGeometryRect*      CreatePluginGeometryRect(float w = 1.0f, float h = 1.0f);
    static PluginTransformSimple*   CreatePluginTransformSimple(const TransformF& transform);

    static BasicNode*               CreateNode( ITransformPlugin* tPlugin, IGeometryGenPlugin* gPlugin
                                              , IShaderPlugin* pShaderPlugin, IShaderPlugin* vShaderPlugin, IShaderPlugin* gShaderPlugin
                                              , BasicNode* parent = nullptr);
};

}