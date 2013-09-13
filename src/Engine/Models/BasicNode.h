#pragma once

#include "Node.h"
#include "Plugin.h"
#include "Engine\Models\Plugins\Geometry\PluginGeometry.h"
#include "Engine\Models\Plugins\Transform\PluginTransform.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include <vector>

namespace bv {

class InterpolatorsList;
class LayerNode;
class PixelShader;
class VertexShader; 
class GeometryShader;
class ShaderParameters;

typedef std::vector< LayerNode* > TLayerNodeVec;
typedef std::vector< Node* > TNodeVec;

class BasicNode : public Node
{
private:
    TNodeVec                m_children;
    TLayerNodeVec           m_layers;

    TTransformPluginVec     m_transformPlugins;
    TGeometryPluginVec      m_geometryPlugins;

    IShaderPlugin*          m_pshaderPlugin;
    IShaderPlugin*          m_vshaderPlugin;
    IShaderPlugin*          m_gshaderPlugin;

public:
    explicit BasicNode();
    virtual ~BasicNode(){}

    virtual SceneNode*      buildScene              ();                              
    
    bool                    addChild                (Node* n);

    void                    addTransformPlugin      (ITransformPlugin* tPlugin);
    void                    addGeometryPlugin       (IGeometryGenPlugin* gPlugin);
    void                    setPixelShaderPlugin    (IShaderPlugin* sPlugin);
    void                    setVertexShaderPlugin   (IShaderPlugin* sPlugin);
    void                    setGeometryShaderPlugin (IShaderPlugin* sPlugin);

    virtual void            Print                   (std::ostream& out, int tabs = 0)             const;
    virtual void            Update                  (float t);

private:
    //VertexDescriptor * vd, VertexBuffer * vb, IndexBuffer * ib, RenderableEffect * effect
    RenderableEntity::RenderableType    GetRenderableType() const;
    bool                                CreateRenderableData(VertexDescriptor** vd, VertexBuffer** vb, IndexBuffer** ib, VertexArray ** vao) const;
    RenderableEffect*                   CreateRenderaleEffectMockImplementationForCompleteDummies() const;

    PixelShader*                        CreatePixelShader       ()                      const;
    VertexShader*                       CreateVertexShader      ()                      const;   
    GeometryShader*                     CreateGeometryShader    ()                      const;

public:
    static void                         RegisterShaderParameters(IShaderPlugin* shaderPlugin, ShaderParameters * shParams);
};




} // bv