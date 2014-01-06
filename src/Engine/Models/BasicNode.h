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
class IVertexAttributesChannelDescriptor;

typedef std::vector< IModelNodePtr > TNodeVec;

class BasicNode : public bv::IModelNode
{
private:

    bool                        m_visible;

    TNodeVec                    m_children;
    TNodeVec                    m_layers;

    std::vector< IPluginPtr >   m_plugins;

public:

    explicit BasicNode();
    virtual ~BasicNode()
    {
    }

    virtual SceneNode *                 BuildScene              ();

    void                                AddChild                ( IModelNode * n );
    void                                AddLayer                ( IModelNode * n );
    void                                AddPlugin               ( IPlugin * plugin );

    virtual void                        Print                   ( std::ostream & out, int tabs = 0 ) const;
    virtual void                        Update                  ( TimeType t );

    virtual bool                        IsVisible               ( TimeType t ) const;
    void                                SetVisible              ( bool visible );

private:

    PrimitiveType                       GetRenderableType       ()                                  const;
    bool                                CreateRenderableData    ( /*VertexArray ** vao*/ )          const;

    //FIXME: scene building API should be moved to some more appropriate place
    RenderableArrayDataSingleVertexBuffer *         CreateRenderableArrayData           ( PrimitiveType type ) const; 
    RenderableArrayDataArraysSingleVertexBuffer *   CreateRenderableArrayDataTriStrip   () const;
    RenderableArrayDataArraysSingleVertexBuffer *   CreateRenderableArrayDataArrays     ( const std::vector< IConnectedComponent * > & ccVec, const IVertexAttributesChannelDescriptor * desc, bool isTimeInvariant ) const;
    void                                            AddVertexDataToVBO                  ( char * data, IConnectedComponent * cc ) const;

    VertexDescriptor *                  CreateVertexDescriptor          ( const IVertexAttributesChannelDescriptor * desc ) const;

    unsigned int                        TotalNumVertices                ( const std::vector< IConnectedComponent * > & ccVec) const;
    unsigned int                        TotalSize                       ( const std::vector< IConnectedComponent * > & ccVec, const IVertexAttributesChannelDescriptor * desc ) const;

    RenderableEffect *                  CreateRenderaleEffectMockImplementationForCompleteDummies() const;

    PixelShader *                       CreatePixelShader       ()                      const;
    VertexShader *                      CreateVertexShader      ()                      const;   
    GeometryShader *                    CreateGeometryShader    ()                      const;


    // ********************************
    //
    template< typename ShaderType, typename ChannelType >
    const ChannelType * GetShaderChannel  () const
    {
        return nullptr;
    }

    // ********************************
    //
    template<>
    const IPixelShaderChannel *   GetShaderChannel< PixelShader, IPixelShaderChannel >    () const
    {
        if( !m_plugins.empty() )
        {
            return m_plugins.back()->GetPixelShaderChannel();
        }

        return nullptr;
    }

    // ********************************
    //
    template<>
    const IVertexShaderChannel *   GetShaderChannel< VertexShader, IVertexShaderChannel >    () const
    {
        if( !m_plugins.empty() )
        {
            return m_plugins.back()->GetVertexShaderChannel();
        }

        return nullptr;
    }

    // ********************************
    //
    template<>
    const IGeometryShaderChannel *   GetShaderChannel< GeometryShader, IGeometryShaderChannel >    () const
    {
        if( !m_plugins.empty() )
        {
            return m_plugins.back()->GetGeometryShaderChannel();
        }

        return nullptr;
    }

    // ********************************
    //
    template< typename ShaderType, typename ShaderChannel >
    ShaderType *                        CreateShader            ()                      const
    {
        auto sCh = GetShaderChannel< ShaderType, ShaderChannel >();

        if( sCh != nullptr )
        {
            ShaderType * s = new ShaderType( sCh->GetShaderSource() );

            BasicNode::RegisterShaderParameters( sCh, s->GetOrCreateShaderParameters() );
        
            s->RegisterUpdater( ShaderParamUpdater::Create( sCh, s ) );

            return s;
        }

        return nullptr;
    }

public:

    static void                         RegisterShaderParameters( const IShaderChannel * shaderPlugin, ShaderParameters * shParams );

};


} // model
} // bv