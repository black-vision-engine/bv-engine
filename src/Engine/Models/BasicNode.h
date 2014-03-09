#pragma once

#include <vector>
#include <string>

#include "Engine/Models/Interfaces/IModelNode.h"

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
    bool                                    AddPlugin               ( IPluginPtr plugin );
    bool                                    AddPlugin               ( const std::string & uid );
    bool                                    AddPlugin               ( const std::string & uid, const std::string & name );

    virtual void                            Print                   ( std::ostream & out, int tabs = 0 ) const;
    virtual void                            Update                  ( TimeType t );

    virtual bool                            IsVisible               () const override;
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

    RenderableEffect *                  CreateDefaultEffect             ( const IPlugin * finalizer ) const;

public:

    static void                         RegisterShaderParameters( const IShaderChannel * shaderPlugin, ShaderParameters * shParams );

    friend class ModelAccessors;

};


} // model
} // bv