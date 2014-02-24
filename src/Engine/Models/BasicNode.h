#pragma once

#include <vector>
#include <string>

#include "Engine/Models/Node.h"

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/DefaultPluginListFinalized.h"

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

class PluginsManager;
class IShaderChannel;
class IConnectedComponent;
class IVertexAttributesChannelDescriptor;
    
typedef std::vector< IModelNodePtr > TNodeVec;

class BasicNode : public IModelNode
{
private:

    const std::string               m_name;

    const PluginsManager *          m_pluginsManager;
    bool                            m_visible;

    TNodeVec                        m_children;
    TNodeVec                        m_layers;

    DefaultPluginListFinalizedPtr   m_pluginList;

public:

    explicit BasicNode( const std::string & name, const PluginsManager * pluginsManager = nullptr );
    virtual ~BasicNode();

    virtual const IPlugin *                 GetPlugin               ( const std::string & name ) const;
    virtual const IModelNode *              GetChild                ( const std::string & name ) const;
    virtual const IModelNode *              GetLayer                ( const std::string & name ) const;

    virtual const IPluginListFinalized *    GetPluginList           () const;

    virtual const std::string &             GetName                 () const;

    virtual SceneNode *                     BuildScene              ();

    void                                    AddChild                ( IModelNode * n );
    void                                    AddLayer                ( IModelNode * n );

    //Convenience API (so that list can be created from external source and simply attached to this node)
    void                                    SetPlugins              ( DefaultPluginListFinalizedPtr plugins );

    //Utility API - plugins can be added on-the-fly by user using an editor
    bool                                    AddPlugin               ( IPlugin * plugin );
    bool                                    AddPlugin               ( const std::string & uid );
    bool                                    AddPlugin               ( const std::string & uid, const std::string & name );

    virtual void                            Print                   ( std::ostream & out, int tabs = 0 ) const;
    virtual void                            Update                  ( TimeType t );

    virtual bool                            IsVisible               ( TimeType t ) const;
    void                                    SetVisible              ( bool visible );

private:

    bool                                    CreateRenderableData    ( /*VertexArray ** vao*/ )          const;

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
        return m_pluginList->GetFinalizePlugin()->GetPixelShaderChannel();
    }

    // ********************************
    //
    template<>
    const IVertexShaderChannel *   GetShaderChannel< VertexShader, IVertexShaderChannel >    () const
    {
        return m_pluginList->GetFinalizePlugin()->GetVertexShaderChannel();
    }

    // ********************************
    //
    template<>
    const IGeometryShaderChannel *   GetShaderChannel< GeometryShader, IGeometryShaderChannel >    () const
    {
        return m_pluginList->GetFinalizePlugin()->GetGeometryShaderChannel();
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

    friend class ModelAccessors;

};


} // model
} // bv