#include "MockScenes.h"

#include "Engine/Models/Plugins/Simple/DefaultTransformPlugin.h"
#include "Engine/Models/BasicNode.h"


namespace bv {

namespace {

// *****************************
//
model::BasicNode *  DefaultTransformPluginOnly  ()
{
    model::BasicNode * root = new model::BasicNode( "Root" );

    root->AddPlugin( model::DefaultTransformPluginDesc::CreatePlugin( nullptr, true ) );

    return root;
}

} // anonymous

// *****************************
//
model::BasicNode *     TestScenesFactory::NewModelTestScene    ()
{
    auto root =  DefaultTransformPluginOnly ();

    return root;
}

} //bv
