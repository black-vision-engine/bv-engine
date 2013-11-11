#pragma once

#include <vector>

#include "Engine/Models/Node.h"

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"


namespace bv {
    
class VertexDescriptor;
class PixelShader;
class VertexShader; 
class GeometryShader;
class ShaderParameters;
class RenderableArrayDataSingleVertexBuffer;
class RenderableArrayDataArraysSingleVertexBuffer;

namespace model {

class IShaderChannel;
class IConnectedComponent;
class IGeometryChannelDescriptor;

typedef std::vector< IModelNode* > TNodeVec;

class BasicNode : public IModelNode
{
private:

    bool                    m_visible;

    TNodeVec                m_children;
    TNodeVec                m_layers;

    std::vector< IPlugin* > m_plugins;

public:

    explicit BasicNode();
    virtual ~BasicNode(){}

    virtual SceneNode*                  BuildScene              ();                              
    void                                AddChild                ( IModelNode* n );
    void                                AddLayer                ( IModelNode* n );
    void                                AddPlugin               ( IPlugin* plugin );

    virtual void                        Print                   ( std::ostream& out, int tabs = 0 ) const;
    virtual void                        Update                  ( float t );

    virtual bool                        IsVisible               ( float t ) const;
    void                                SetVisible              ( bool visible );

private:

    PrimitiveType                       GetRenderableType       ()                                  const;
    bool                                CreateRenderableData    ( /*VertexArray ** vao*/ )          const;

    //FIXME: scene building API should be moved to some more appropriate place
    RenderableArrayDataSingleVertexBuffer *         CreateRenderableArrayData           ( PrimitiveType type ) const; 
    RenderableArrayDataArraysSingleVertexBuffer *   CreateRenderableArrayDataTriStrip   () const;
    RenderableArrayDataArraysSingleVertexBuffer *   CreateRenderableArrayDataArrays     ( const std::vector< IConnectedComponent * > & ccVec, const IGeometryChannelDescriptor * desc, bool isTimeInvariant ) const;
    void                                            AddVertexDataToVBO                  ( char * data, IConnectedComponent * cc ) const;

    VertexDescriptor *                  CreateVertexDescriptor          ( const IGeometryChannelDescriptor * desc ) const;

    unsigned int                        TotalNumVertices                ( const std::vector< IConnectedComponent * > & ccVec) const;
    unsigned int                        TotalSize                       ( const std::vector< IConnectedComponent * > & ccVec, const IGeometryChannelDescriptor * desc ) const;

    RenderableEffect *                  CreateRenderaleEffectMockImplementationForCompleteDummies() const;

    PixelShader *                       CreatePixelShader       ()                      const;
    VertexShader *                      CreateVertexShader      ()                      const;   
    GeometryShader *                    CreateGeometryShader    ()                      const;

public:

    static void                         RegisterShaderParameters(const IShaderChannel* shaderPlugin, ShaderParameters * shParams);

};


} // model
} // bv