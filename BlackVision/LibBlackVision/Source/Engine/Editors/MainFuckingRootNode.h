#pragma once

#include "Engine/Models/BasicNode.h"




namespace bv {
namespace model
{

class MainFuckingRootNode;
DEFINE_PTR_TYPE( MainFuckingRootNode )



// ***********************
/* BasicNode requires unique name for children nodes. This causes that root nodes for different scene
must have different name. To avoid this here is overloaded AddChildToModelOnly.

FIXME: Remove main root from engine and find other solution to store scene roots.*/
class MainFuckingRootNode : public BasicNode
{
    /// FIXME: should be privete
public:

    explicit MainFuckingRootNode        ( const std::string & name, const PluginsManager * pluginsManager = nullptr );

public:

    virtual void            AddChildToModelOnly     ( BasicNodePtr n, UInt32 idx ) override;

public:

    static MainFuckingRootNodePtr  Create           ( const std::string & name, const PluginsManager * pluginsManager = nullptr );
};




// ========================================================================= //
// Implementation
// ========================================================================= //

// ***********************
//
inline MainFuckingRootNode::MainFuckingRootNode        ( const std::string & name, const PluginsManager * pluginsManager )
    :   BasicNode( name, pluginsManager )
{}

// ***********************
//
inline MainFuckingRootNodePtr       MainFuckingRootNode::Create         ( const std::string & name, const PluginsManager * pluginsManager )
{
    return std::make_shared< MainFuckingRootNode >( name, pluginsManager );
}


// ***********************
//
inline void             MainFuckingRootNode::AddChildToModelOnly        ( BasicNodePtr n, UInt32 idx )
{
    AddChildToModelOnlyImpl( n, idx );
}
    
}   //model
}	// bv


