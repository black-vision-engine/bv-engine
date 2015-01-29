#pragma once

#include <vector>
#include <string>

#include "Engine/Models/Interfaces/IModelNode.h"
#include "Engine/Models/Interfaces/INodeLogic.h"

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/DefaultPluginListFinalized.h"


#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Mathematics/Rect.h"


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
DEFINE_PTR_TYPE(BasicNode)
DEFINE_CONST_PTR_TYPE(BasicNode)

typedef std::vector< BasicNodePtr > TNodeVec;

class BasicNode : public IModelNode, public std::enable_shared_from_this< BasicNode >
{
private:

    std::string                     m_name;
    
    const PluginsManager *          m_pluginsManager;
    bool                            m_visible;

    BasicOverrideState *            m_overrideState;

    TNodeVec                        m_children;
    TNodeVec                        m_layers;

    DefaultPluginListFinalizedPtr   m_pluginList;

	INodeLogicPtr					m_nodeLogic;

public:

    explicit BasicNode( const std::string & name, ITimeEvaluatorPtr timeEvaluator, const PluginsManager * pluginsManager = nullptr );
    virtual ~BasicNode();

    virtual IPluginPtr                      GetPlugin               ( const std::string & name ) const override;
    virtual IFinalizePluginConstPtr         GetFinalizePlugin       () const override;

    virtual IModelNodePtr                   GetNode                 ( const std::string & path, const std::string & separator = "/" ) override;
    virtual IModelNodePtr                   GetChild                ( const std::string & name ) override;
    virtual IModelNodePtr                   GetLayer                ( const std::string & name ) override;

    virtual const IPluginListFinalized *    GetPluginList           () const override;

    virtual void                            EnableOverrideStateAM   () override;
    virtual void                            EnableOverrideStateNM   () override;
    virtual void                            DisableOverrideStateAM  () override;
    virtual void                            DisableOverrideStateNM  () override;

    virtual bool                            OverrideStateChangedAM  () const override;
    virtual bool                            OverrideStateChangedNM  () const override;

    virtual void                            SetOverrideStateChgAM   ( bool changed ) override;
    virtual void                            SetOverrideStateChgNM   ( bool changed ) override;

public:

    virtual bool                            IsStateOverridenAM      () const override;
    virtual bool                            IsStateOverridenNM      () const override;

    virtual IOverrideState *                GetOverrideState        () override;

    virtual const std::string &             GetName                 () const override;
    void                                    SetName                 ( const std::string & name );

	// axis-aligned bounding box
	mathematics::Rect 						GetAABB					() const;

    virtual SceneNode *                     BuildScene              () override;

    void                                    AddChild                ( BasicNodePtr n );
    void                                    AddLayer                ( BasicNodePtr n );

    //Convenience API (so that list can be created from external source and simply attached to this node)
    void                                    SetPlugins              ( DefaultPluginListFinalizedPtr plugins );

private:

    void                                    NonNullPluginsListGuard ();

	mathematics::Rect 						GetAABB					( const glm::mat4 & currentTransformation ) const;


public:

    //Utility API - plugins can be added on-the-fly by user using an editor
    bool                                    AddPlugin               ( IPluginPtr plugin );
    bool                                    AddPlugin               ( const std::string & uid, ITimeEvaluatorPtr timeEvaluator );
    bool                                    AddPlugin               ( const std::string & uid, const std::string & name, ITimeEvaluatorPtr timeEvaluator );
    bool                                    AddPlugins              ( const std::vector< std::string > & uids, ITimeEvaluatorPtr timeEvaluator );
    bool                                    AddPlugins              ( const std::vector< std::string > & uids, const std::vector< std::string > & names, ITimeEvaluatorPtr timeEvaluator );

	void									SetLogic				( INodeLogicPtr logic );

    virtual void                            Print                   ( std::ostream & out, int tabs = 0 ) const override;
    virtual void                            Update                  ( TimeType t ) override;

    virtual bool                            IsVisible               () const override;
    void                                    SetVisible              ( bool visible );

private:

    SceneNode *                             CreateSceneNode         ( IPluginConstPtr finalizer ) const;
    RenderableEntity *                      CreateRenderable        ( IPluginConstPtr finalizer ) const;
    std::vector< bv::Transform >            CreateTransformVec      ( IPluginConstPtr finalizer ) const;

    bool                                    CreateRenderableData    ( /*VertexArray ** vao*/ ) const;

    //FIXME: scene building API should be moved to some more appropriate place
    RenderableArrayDataSingleVertexBuffer *         CreateRenderableArrayData           ( PrimitiveType type ) const; 
    RenderableArrayDataArraysSingleVertexBuffer *   CreateRenderableArrayDataTriStrip   () const;
    RenderableArrayDataArraysSingleVertexBuffer *   CreateRenderableArrayDataArrays     ( const std::vector< IConnectedComponentPtr > & ccVec, const IVertexAttributesChannelDescriptor * desc, bool isTimeInvariant ) const;
    void                                            AddVertexDataToVBO                  ( char * data, IConnectedComponentPtr cc ) const;

    VertexDescriptor *                  CreateVertexDescriptor          ( const IVertexAttributesChannelDescriptor * desc ) const;

    unsigned int                        TotalNumVertices                ( const std::vector< IConnectedComponentPtr > & ccVec) const;
    unsigned int                        TotalSize                       ( const std::vector< IConnectedComponentPtr > & ccVec, const IVertexAttributesChannelDescriptor * desc ) const;

    RenderableEffectPtr                 CreateDefaultEffect             ( IPluginConstPtr finalizer ) const;

    std::string                         SplitPrefix                     ( std::string & str, const std::string & separator = "/" ) const;

public:

    friend class ModelAccessors;

};


} // model
} // bv