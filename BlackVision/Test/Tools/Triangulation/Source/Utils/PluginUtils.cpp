#include "PluginUtils.h"

#include "Engine/Models/Plugins/Simple/GeometryProcessors/TriangulatePlugin.h"
#include "Engine/Models/Plugins/Simple/GeometryProcessors/DefaultExtrudePlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultMeshPlugin.h"
#include "Engine/Models/Plugins/Simple/TextPlugins/DefaultText3DPlugin.h"
#include "Engine/Models/Plugins/Simple/GeometryProcessors/TriangulatePlugin.h"


namespace pluginUtils
{

// ***********************
//
std::vector< bv::model::IPluginDescriptor* >    CreatePlugins()
{
    std::vector< bv::model::IPluginDescriptor * > descriptors;

    descriptors.push_back( new bv::model::DefaultExtrudePluginDesc() );
    descriptors.push_back( new bv::model::TriangulatePluginDesc() );
    descriptors.push_back( new bv::model::DefaultText3DPluginDesc() );
    descriptors.push_back( new bv::model::DefaultMeshPluginDesc() );
    descriptors.push_back( new bv::model::TriangulatePluginDesc() );

    return descriptors;
}



}

