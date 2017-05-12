#pragma once

#include <vector>
#include <string>
#include <hash_map>

#include "Engine/Models/Interfaces/IModelNode.h"
#include "Engine/Models/Plugins/DefaultPluginListFinalized.h"
#include "Engine/Models/BoundingVolume.h"

#include "Serialization/ISerializable.h"


namespace bv { namespace model {

class PluginsManager;

class BasicNode;
DEFINE_PTR_TYPE( BasicNode )
DEFINE_CONST_PTR_TYPE( BasicNode )
DEFINE_WEAK_PTR_TYPE( BasicNode )

typedef std::vector< BasicNodePtr > TNodeVec;

class ModelNodeEditor;


class BasicNode : public IModelNode, public std::enable_shared_from_this< BasicNode >, public ISerializable
{

private:

    std::string                     m_name;
    
    const PluginsManager *          m_pluginsManager;
    bool                            m_visible;

    TNodeVec                        m_children;

    DefaultPluginListFinalizedPtr   m_pluginList;
    BoundingVolumePtr               m_boundingVolume;

    INodeLogicPtr					m_nodeLogic;
    IModelNodeEffectPtr             m_modelNodeEffect;

    ModelNodeEditor *				m_modelNodeEditor;

protected:

    explicit BasicNode( const std::string & name, ITimeEvaluatorPtr, const PluginsManager * pluginsManager = nullptr );

public:

    virtual ~BasicNode();

    static BasicNodePtr                     Create                  ( const std::string & name, ITimeEvaluatorPtr, const PluginsManager * pluginsManager = nullptr );
    static BasicNodePtr                     Create                  ( const IDeserializer& doc );
    virtual void                            Serialize               ( ISerializer& doc ) const;

    BasicNodePtr					        Clone					() const;


    virtual IPluginPtr                      GetPlugin               ( const std::string & name ) const override;
    virtual IFinalizePluginConstPtr         GetFinalizePlugin       () const override;

    /** @param[ path ] relative path */
    virtual IModelNodePtr                   GetNode                 ( const std::string & path, const std::string & separator = "/" ) override;
    virtual IModelNodePtr                   GetChild                ( const std::string & name ) override;
    
    INodeLogicPtr							GetLogic				() const override;

    virtual const IPluginListFinalized *    GetPluginList           () const override;
    virtual std::vector< IParameterPtr >    GetParameters           () const override;
    virtual std::vector< ITimeEvaluatorPtr >GetTimelines			( bool recursive ) const override;

    virtual unsigned int                    GetNumChildren          () const override;

    virtual IModelNodeEffectPtr             GetNodeEffect           () const;
    void                                    SetNodeEffect           ( IModelNodeEffectPtr nodeEffect );

    virtual const std::string &             GetName                 () const override;
    void                                    SetName                 ( const std::string & name );

    // axis-aligned bounding box
    mathematics::Rect 						GetAABB					    () const;
    BoundingVolumeConstPtr				    GetBoundingVolume		    () const;
    mathematics::Box				        GetBoundingBoxRecursive     () const;

	void									RecreateBoundingVolume		();

    BasicNodePtr                            GetChild                ( unsigned int i );
    const BasicNode *                       GetChild                ( unsigned int i ) const;
    unsigned int                            GetNumPlugins           () const;

    void                                    AddChildToModelOnly     ( BasicNodePtr n );
    void                                    AddChildToModelOnly     ( BasicNodePtr n, UInt32 idx );
    void                                    DetachChildNodeOnly     ( BasicNodePtr n );

    ModelNodeEditor *						GetModelNodeEditor		();

    DefaultPluginListFinalizedPtr			GetPlugins				();

private:

    //Convenience API (so that list can be created from external source and simply attached to this node)
    void                                    SetPlugins              ( DefaultPluginListFinalizedPtr plugins );

private:

    mathematics::Rect 						GetAABB					( const glm::mat4 & currentTransformation ) const;
    BoundingVolumeConstPtr 				    GetBoundingVolume		( const glm::mat4 & currentTransformation ) const;

public:

    //Utility API - plugins can be added on-the-fly by user using an editor
    bool                                    AddPlugin               ( IPluginPtr plugin );
    bool                                    AddPlugin               ( const std::string & uid, ITimeEvaluatorPtr timeEvaluator );
    bool                                    AddPlugin               ( const std::string & uid, const std::string & name, ITimeEvaluatorPtr timeEvaluator );
    bool                                    AddPlugins              ( const std::vector< std::string > & uids, ITimeEvaluatorPtr timeEvaluator );

    void									SetLogic				( INodeLogicPtr logic );
    void                                    RemoveLogic             ();

    virtual void                            Update                  ( TimeType t ) override;

    virtual bool                            IsVisible               () const override;
    void                                    SetVisible              ( bool visible );

public:

    /**@brief Remove prefix from path.
    @param[ path ] Returns path without prefix.
    @return Returns prefix. */
    static std::string                      SplitPrefix             ( std::string & path, const std::string & separator = "/" );

    /**@brief Try to convert string to integer if it matches the pattern "[escapeChar][integer]" (e.g. "#0").
    @return Returns parsed index or -1 if string didn't match the pattern. */
    static Int32                            TryParseIndex           ( std::string & str, const char escapeChar = '#' );

public:

    friend class ModelAccessors;

};


} // model


namespace CloneViaSerialization {

    void                    UpdateTimelines ( model::BasicNode * obj, const std::string & prefix, const std::string & destScene, bool recursive );

} //CloneViaSerialization

} // bv
