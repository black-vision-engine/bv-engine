#pragma once

#include <vector>
#include <string>
#include <hash_map>

#include "Engine/Models/Interfaces/IModelNode.h"
#include "Engine/Models/Plugins/DefaultPluginListFinalized.h"
#include "Engine/Models/BoundingVolume.h"

#include "Serialization/ISerializable.h"

#include "Engine/Models/Gizmos/GizmoContainer.h"



/**@defgroup Model*/



/**@page Nodes Nodes

@section Nodes_General General

@ref Scenes "Scene" consists of hierarchical structure of nodes. Each node inherits transformation from parent and
can have it's own tranformation if it posseses @ref bv::model::DefaultTransformPlugins "TransformPlugin".

Example nodes hierarchy:
- root (id: 12)
    - circles (id: 13)
        - circle1 (id: 14)
        - circle2 (id: 19)
        - circle3 (id: 20)
    - cubes (id: 21)
        - cube1 (id: 23)
        - cube2 (id: 24)
        - cube1 (id: 25)
    - child3 (id: 26)

Nodes are containers for @ref Plugins, @ref NodeLogics and @ref NodeEffects. Single node can contain multiple plugins
and only one effect and one logic.

@section Nodes_AccessingNodesAPI Accessing nodes through API


There are 3 ways to reference node through API:
- Using name path (built from node names)
- Using indexed path (built from nodes index in subtree)
- Using node Unique Id.

@subsection Nodes_AccessingNodesAPI_NamePath Accessing using name path

Each node has it's own non unique name. We can address node using path built from node names.
For example if we want to access node <b>"circle1"</b>, we could write:
<b>"root/circles/circle1"</b>

Since node name is non unique, there's no way to distinguish between nodes named <b>"cube1"</b> in subtree of <b>"root/cubes"</b>.
Address <b>"root/cubes/cube1"</b> will return first found child node named <b>"cube1"</b>.

@subsection Nodes_AccessingNodesAPI_IndexedPath Accessing using indexed path

The second way to address nodes, is to use indexed path. Indexed path consists of indicies of nodes (position in subtree counted from zero)
prefixed by # and separated by slash. For example to access node <b>"circle3"</b> we can write:
<b>"#0/#0/#2"</b> and to access second node <b>"cube1"</b> we can write:
<b>"#0/#1/#2"</b>

@subsection Nodes_AccessingNodesAPI_UniqueID Accessing using unique id

Each node has unique id generated at creation time. We can access node using it's id prefixed with @.
For example to access node <b>"child3"</b> we can write: <b>"@26"</b>.

@copydoc API_NodesManipulation

@see bv::model::BasicNode
*/



namespace bv { namespace model {

class PluginsManager;

class BasicNode;
DEFINE_PTR_TYPE( BasicNode )
DEFINE_CONST_PTR_TYPE( BasicNode )
DEFINE_WEAK_PTR_TYPE( BasicNode )

typedef std::vector< BasicNodePtr > TNodeVec;

class ModelNodeEditor;



/**@brief Model representation of Node.

@copydoc Nodes

@ingroup Model*/
class BasicNode : public IModelNode, public std::enable_shared_from_this< BasicNode >, public ISerializable
{

private:

    std::string                     m_name;
    UniqueID                        m_id;
    
    const PluginsManager *          m_pluginsManager;
    bool                            m_visible;

    TNodeVec                        m_children;

    DefaultPluginListFinalizedPtr   m_pluginList;
    BoundingVolumePtr               m_boundingVolume;

    INodeLogicPtr					m_nodeLogic;
    IModelNodeEffectPtr             m_modelNodeEffect;

    ModelNodeEditor *				m_modelNodeEditor;

    GizmoContainerUPtr              m_gizmos;

protected:

    explicit BasicNode( const std::string & name, const PluginsManager * pluginsManager = nullptr );

public:

    virtual ~BasicNode();

    static BasicNodePtr                     Create                  ( const std::string & name, const PluginsManager * pluginsManager = nullptr );
    static BasicNodePtr                     Create                  ( const IDeserializer& doc );
    virtual void                            Serialize               ( ISerializer& doc ) const;

    BasicNodePtr					        Clone					() const;


    virtual IPluginPtr                      GetPlugin               ( const std::string & name ) const override;
    virtual IFinalizePluginConstPtr         GetFinalizePlugin       () const override;

    /** @param[ path ] relative path */
    virtual IModelNodePtr                   GetNode                 ( const std::string & path, const std::string & separator = "/" ) override;
    virtual IModelNodePtr                   GetNode                 ( UniqueID id, bool recursive = true ) override;
    virtual IModelNodePtr                   GetChild                ( const std::string & name ) override;
    
    INodeLogicPtr							GetLogic				() const override;

    virtual const IPluginListFinalized *    GetPluginList           () const override;
    virtual std::vector< IParameterPtr >    GetParameters           () const override;
    virtual std::vector< ITimeEvaluatorPtr >GetTimelines			( bool recursive ) const override;

    virtual unsigned int                    GetNumChildren          () const override;

    virtual IModelNodeEffectPtr             GetNodeEffect           () const;
    void                                    SetNodeEffect           ( IModelNodeEffectPtr nodeEffect );

    virtual UniqueID                        GetUID                  () const override;
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

    void                                    AddGizmo                ( IModelNodePtr gizmoRoot, UInt32 idx = std::numeric_limits< UInt32 >::max() );
    void                                    RemoveGizmo             ( UInt32 idx );
    void                                    RemoveGizmo             ( IModelNodePtr gizmoRoot );

    IModelNodePtr                           GetGizmo                ( UInt32 idx ) const;
    UInt32                                  GetNumGizmos            () const;

private:

    GizmoContainer *                        AllocateGizmos          ();
    void                                    DeallocateGizmos        ();

public:

    /**@brief Remove prefix from path.
    @param[ path ] Returns path without prefix.
    @return Returns prefix. */
    static std::string                      SplitPrefix             ( std::string & path, const std::string & separator = "/" );

    /**@brief Try to convert string to integer if it matches the pattern "[escapeChar][integer]" (e.g. "#0").
    @return Returns parsed index or -1 if string didn't match the pattern. */
    static Int32                            TryParseIndex           ( std::string & str, const char escapeChar = '#' );

    /**@brief Check if path string contains node uid instead of adress.*/
    static bool                             IsPathWithUID           ( const std::string & path, const char escapeChar = '@' );


    /**@brief Parses path to extract UID.*/
    static Expected< UniqueID >             TryParseUID             ( const std::string & path, const char escapeChar = '@' );

public:

    friend class ModelAccessors;

};


} // model


namespace CloneViaSerialization {

    void                    UpdateTimelines ( model::BasicNode * obj, const std::string & prefix, const std::string & destScene, bool recursive );

} //CloneViaSerialization

} // bv
