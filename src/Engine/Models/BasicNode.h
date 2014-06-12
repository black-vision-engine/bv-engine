#pragma once

#include <vector>
#include <string>

#include "Engine/Models/Interfaces/IModelNode.h"

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/DefaultPluginListFinalized.h"

//#include "Engine/Models/Interfaces/ITimeEvaluator.h"

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

class IOverrideState;
class BasicOverrideState;

class BasicNode;
typedef std::shared_ptr< BasicNode > BasicNodePtr;

typedef std::vector< BasicNodePtr > TNodeVec;

class BasicNode : public IModelNode
{
private:

    const std::string               m_name;

    const PluginsManager *          m_pluginsManager;
    bool                            m_visible;

    BasicOverrideState *            m_overrideState;

    TNodeVec                        m_children;
    TNodeVec                        m_layers;

    DefaultPluginListFinalizedPtr   m_pluginList;

public:

    explicit BasicNode( const std::string & name, ITimeEvaluatorPtr timeEvaluator, const PluginsManager * pluginsManager = nullptr );
    virtual ~BasicNode();

    virtual IPlugin *                       GetPlugin               ( const std::string & name ) const override;
    virtual const IModelNode *              GetNode                 ( const std::string & path, const std::string & separator = "/" ) const override;
    virtual const IModelNode *              GetChild                ( const std::string & name ) const override;
    virtual const IModelNode *              GetLayer                ( const std::string & name ) const override;

    virtual const IPluginListFinalized *    GetPluginList           () const override;

    virtual void                            EnableOverrideState     () override;
    virtual void                            DisableOverrideState    () override;

private:

    void                                    PropagateOverrideState ( IOverrideState * state );

public:

    virtual bool                            IsStateOverriden        () const override;
    virtual IOverrideState *                GetOverrideState        () override;

    virtual const std::string &             GetName                 () const override;

    virtual SceneNode *                     BuildScene              () override;

    void                                    AddChild                ( BasicNode * n );
    void                                    AddLayer                ( BasicNode * n );

    //Convenience API (so that list can be created from external source and simply attached to this node)
    void                                    SetPlugins              ( DefaultPluginListFinalizedPtr plugins );

private:

    void                                    NonNullPluginsListGuard ();

public:

    //Utility API - plugins can be added on-the-fly by user using an editor
    bool                                    AddPlugin               ( IPlugin * plugin );
    bool                                    AddPlugin               ( IPluginPtr plugin );
    bool                                    AddPlugin               ( const std::string & uid, ITimeEvaluatorPtr timeEvaluator );
    bool                                    AddPlugin               ( const std::string & uid, const std::string & name, ITimeEvaluatorPtr timeEvaluator );
    bool                                    AddPlugins              ( const std::vector< std::string > & uids, ITimeEvaluatorPtr timeEvaluator );
    bool                                    AddPlugins              ( const std::vector< std::string > & uids, const std::vector< std::string > & names, ITimeEvaluatorPtr timeEvaluator );

    virtual void                            Print                   ( std::ostream & out, int tabs = 0 ) const override;
    virtual void                            Update                  ( TimeType t ) override;

    virtual bool                            IsVisible               () const override;
    void                                    SetVisible              ( bool visible );

private:

    SceneNode *                             CreateSceneNode         ( const IPlugin * finalizer ) const;
    RenderableEntity *                      CreateRenderable        ( const IPlugin * finalizer ) const;
    std::vector< bv::Transform >            CreateTransformVec      ( const IPlugin * finalizer ) const;

    bool                                    CreateRenderableData    ( /*VertexArray ** vao*/ ) const;

    //FIXME: scene building API should be moved to some more appropriate place
    RenderableArrayDataSingleVertexBuffer *         CreateRenderableArrayData           ( PrimitiveType type ) const; 
    RenderableArrayDataArraysSingleVertexBuffer *   CreateRenderableArrayDataTriStrip   () const;
    RenderableArrayDataArraysSingleVertexBuffer *   CreateRenderableArrayDataArrays     ( const std::vector< IConnectedComponent * > & ccVec, const IVertexAttributesChannelDescriptor * desc, bool isTimeInvariant ) const;
    void                                            AddVertexDataToVBO                  ( char * data, IConnectedComponent * cc ) const;

    VertexDescriptor *                  CreateVertexDescriptor          ( const IVertexAttributesChannelDescriptor * desc ) const;

    unsigned int                        TotalNumVertices                ( const std::vector< IConnectedComponent * > & ccVec) const;
    unsigned int                        TotalSize                       ( const std::vector< IConnectedComponent * > & ccVec, const IVertexAttributesChannelDescriptor * desc ) const;

    RenderableEffect *                  CreateDefaultEffect             ( const IPlugin * finalizer );

    std::string                         SplitPrefix                     ( std::string & str, const std::string & separator = "/" ) const;

public:

    static void                         RegisterShaderParameters( const IShaderChannel * shaderPlugin, ShaderParameters * shParams );

    friend class ModelAccessors;

};


} // model
} // bv