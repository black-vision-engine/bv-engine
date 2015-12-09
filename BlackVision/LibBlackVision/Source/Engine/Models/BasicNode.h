#pragma once

#include <vector>
#include <string>
#include <hash_map>

#include "Engine/Models/Interfaces/IModelNode.h"
#include "Engine/Models/Interfaces/INodeLogic.h"
#include "Engine/Models/Plugins/DefaultPluginListFinalized.h"

#include "Serialization/ISerializable.h"


namespace bv { namespace model {

class PluginsManager;

class BasicNode;
DEFINE_PTR_TYPE(BasicNode)
DEFINE_CONST_PTR_TYPE(BasicNode)
typedef std::weak_ptr< BasicNode >  BasicNodeWeakPtr;

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

    TNodeVec                        m_children;

    DefaultPluginListFinalizedPtr   m_pluginList;

	INodeLogicPtr					m_nodeLogic;
    IModelNodeEffectPtr             m_modelNodeEffect;

    ModelNodeEditor *				m_modelNodeEditor;

protected:

    explicit BasicNode( const std::string & name, ITimeEvaluatorPtr, const PluginsManager * pluginsManager = nullptr );

public:

    virtual ~BasicNode();

    static BasicNodePtr                     Create                  ( const std::string & name, ITimeEvaluatorPtr, const PluginsManager * pluginsManager = nullptr );
    static ISerializablePtr                 Create                  ( const IDeserializer& doc );
    virtual void                            Serialize               ( ISerializer& doc ) const;

	virtual IModelNodePtr					Clone					() const override;


    virtual IPluginPtr                      GetPlugin               ( const std::string & name ) const override;
    virtual IFinalizePluginConstPtr         GetFinalizePlugin       () const override;

    virtual IModelNodePtr                   GetNode                 ( const std::string & path, const std::string & separator = "/" ) override;
    virtual IModelNodePtr                   GetChild                ( const std::string & name ) override;

	INodeLogicPtr							GetLogic				();

    virtual const IPluginListFinalized *    GetPluginList           () const override;

	virtual std::vector< ITimeEvaluatorPtr > GetTimelines			() const override;

    virtual unsigned int                    GetNumChildren          () const override;

    virtual IModelNodeEffectPtr             GetNodeEffect           () const;
    void                                    SetNodeEffect           ( IModelNodeEffectPtr nodeEffect );

    virtual const std::string &             GetName                 () const override;
    void                                    SetName                 ( const std::string & name );

	// axis-aligned bounding box
	mathematics::Rect 						GetAABB					() const;

    BasicNodePtr                            GetChild                ( unsigned int i );
    unsigned int                            GetNumPlugins           () const;

    void                                    AddChildToModelOnly     ( BasicNodePtr n );
    void                                    DetachChildNodeOnly     ( BasicNodePtr n );

	ModelNodeEditor *						GetModelNodeEditor		();

	DefaultPluginListFinalizedPtr			GetPlugins				();

private:

    //Convenience API (so that list can be created from external source and simply attached to this node)
    void                                    SetPlugins              ( DefaultPluginListFinalizedPtr plugins );

private:

	mathematics::Rect 						GetAABB					( const glm::mat4 & currentTransformation ) const;

public:

    //Utility API - plugins can be added on-the-fly by user using an editor
    bool                                    AddPlugin               ( IPluginPtr plugin );
    bool                                    AddPlugin               ( const std::string & uid, ITimeEvaluatorPtr timeEvaluator );
    bool                                    AddPlugin               ( const std::string & uid, const std::string & name, ITimeEvaluatorPtr timeEvaluator );
    bool                                    AddPlugins              ( const std::vector< std::string > & uids, ITimeEvaluatorPtr timeEvaluator );

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


namespace CloneViaSerialization {

	model::BasicNodePtr		CloneNode		( const model::BasicNode * obj, const std::string & prefix );

} //CloneViaSerialization

} // bv
