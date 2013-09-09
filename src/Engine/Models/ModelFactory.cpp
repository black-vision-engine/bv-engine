#include "ModelFactory.h"

#include "Engine\Models\Plugins\Transform\PluginTransformSimple.h"
#include "Engine\Models\Plugins\Geometry\PluginGeometryRect.h"
#include "Engine\Models\BasicNode.h"

namespace bv
{

PluginGeometryRect*      ModelFactory::CreatePluginGeometryRect(float w, float h)
{
    return new PluginGeometryRect(w,h);
}

PluginTransformSimple*   ModelFactory::CreatePluginTransformSimple(const TransformF& transform)
{
    return new PluginTransformSimple(transform);
}

BasicNode*               ModelFactory::CreateNode( ITransformPlugin* tPlugin, IGeometryGenPlugin* gPlugin
                                        , IShaderPlugin* pShaderPlugin, IShaderPlugin* vShaderPlugin, IShaderPlugin* gShaderPlugin
                                        , BasicNode* parent)
{
    BasicNode* ret = new BasicNode();

    ret->addTransformPlugin(tPlugin);
    ret->addGeometryPlugin(gPlugin);
    ret->setGeometryShaderPlugin(gShaderPlugin);
    ret->setPixelShaderPlugin(pShaderPlugin);
    ret->setVertexShaderPlugin(vShaderPlugin);

    if(parent != nullptr)
    {
        parent->addChild(ret);
    }

    return ret;
}
}