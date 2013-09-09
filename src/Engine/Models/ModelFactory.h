#pragma once

#include "Mathematics\Transform\MatTransform.h"
#include "Mathematics/Interpolators/Interpolators.h"

namespace bv
{

//forward declarations
class IGeometryGenPlugin;
class PluginGeometryRect;
class ITransformPlugin;
class PluginTransformSimple;
class IShaderPlugin;
class BasicNode;
class PluginGeometryRing;

class ModelFactory
{
public:
    static PluginGeometryRect*      CreatePluginGeometryRect(float w = 1.0f, float h = 1.0f);
	static PluginGeometryRing*		CreatePluginGeometryRing(const FloatInterpolator& innerRadius, const FloatInterpolator& outerRadius,
															const FloatInterpolator& startAngle, const FloatInterpolator& endAngle, int numSegments);
    static PluginTransformSimple*   CreatePluginTransformSimple(const TransformF& transform);

    static BasicNode*               CreateNode( ITransformPlugin* tPlugin, IGeometryGenPlugin* gPlugin
                                              , IShaderPlugin* pShaderPlugin, IShaderPlugin* vShaderPlugin, IShaderPlugin* gShaderPlugin
                                              , BasicNode* parent = nullptr);
};

}