#pragma once

#include <vector>
#include <string>
#include <hash_map>

#include "Engine/Models/Interfaces/IModelNode.h"
#include "Engine/Models/Interfaces/INodeLogic.h"

#include "Engine/Models/Plugins/DefaultPluginListFinalized.h"

#include "Engine/Interfaces/ISerializable.h"

namespace bv { namespace model {

class PluginsManager;
class BasicOverrideState;

class BasicNode;
DEFINE_PTR_TYPE(BasicNode)
DEFINE_CONST_PTR_TYPE(BasicNode)

typedef std::vector< BasicNodePtr > TNodeVec;

class ModelNodeEditor;


class BasicNode : public IModelNode, public std::enable_shared_from_this< BasicNode >, public ISerializable
{
public:

    //FIXME: hack
    static std::hash_map< IModelNode *, SceneNode * >    ms_nodesMapping;

private:

    std::string                     m_name;
    
    const PluginsManager *          m_pluginsManager;
    bool                            m_visible;

    BasicOverrideState *            m_overrideState;

    TNodeVec                        m_children;

    DefaultPluginListFinalizedPtr   m_pluginList;

	INodeLogicPtr					m_nodeLogic;

	ModelNodeEditor *				m_modelNodeEditor;

protected:

    explicit BasicNode( const std::string & name, ITimeEvaluatorPtr timeEvaluator, const PluginsManager * pluginsManager = nullptr );

public:

    virtual ~BasicNode();

    static BasicNodePtr                     Create                  ( const std::string & name, ITimeEvaluatorPtr timeEvaluator, const PluginsManager * pluginsManager = nullptr );
    static ISerializablePtr                 Create                  ( DeserializeObject& doc );
    virtual void                            Serialize               ( SerializeObject& /*doc*/ ) const { assert( !"implement" ); }

    virtual IPluginPtr                      GetPlugin               ( const std::string & name ) const override;
    virtual IFinalizePluginConstPtr         GetFinalizePlugin       () const override;

    virtual IModelNodePtr                   GetNode                 ( const std::string & path, const std::string & separator = "/" ) override;
    virtual IModelNodePtr                   GetChild                ( const std::string & name ) override;

    virtual const IPluginListFinalized *    GetPluginList           () const override;

    virtual unsigned int                    GetNumChildren          () const override;

    // FIXME: remove when proper GlobalEfect is implemented
    virtual void                            EnableOverrideStateAM   () override;
    virtual void                            EnableOverrideStateNM   () override;
    virtual void                            DisableOverrideStateAM  () override;
    virtual void                            DisableOverrideStateNM  () override;

    virtual bool                            OverrideStateChangedAM  () const override;
    virtual bool                            OverrideStateChangedNM  () const override;

    virtual void                            SetOverrideStateChgAM   ( bool changed ) override;
    virtual void                            SetOverrideStateChgNM   ( bool changed ) override;

    virtual bool                            IsStateOverridenAM      () const override;
    virtual bool                            IsStateOverridenNM      () const override;

    virtual IOverrideState *                GetOverrideState        () override;
    // FIXME: end of remove
    // FIXME: remove when proper GlobalEfect is implemented

    virtual const std::string &             GetName                 () const override;
    void                                    SetName                 ( const std::string & name );

	// axis-aligned bounding box
	mathematics::Rect 						GetAABB					() const;

    BasicNodePtr                            GetChild                ( unsigned int i );
    unsigned int                            GetNumPlugins           () const;

    void                                    AddChildToModelOnly     ( BasicNodePtr n );
    void                                    DetachChildNodeOnly     ( BasicNodePtr n );

	ModelNodeEditor *						GetModelNodeEditor		();
	void									SetModelNodeEditor		( ModelNodeEditor * editor );

	DefaultPluginListFinalizedPtr			GetPlugins				();

private:

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

    virtual void                            Update                  ( TimeType t ) override;

    virtual bool                            IsVisible               () const override;
    void                                    SetVisible              ( bool visible );

private:

    std::string                         SplitPrefix                     ( std::string & str, const std::string & separator = "/" ) const;

public:

    friend class ModelAccessors;

};


} // model
} // bv
