#include "DefaultPlugins.h"

#include "Engine/Models/Plugins/Interfaces/IPluginDescriptor.h"
#include "Engine/Models/Plugins/DefaultRectPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTransformPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultColorPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTexturePlugin.h"


namespace bv { namespace model {

// *********************************
//
std::vector< IPluginDescriptor * >  DefaultBVPluginDescriptors  ()
{
    std::vector< IPluginDescriptor * > descriptors;

    descriptors.push_back( new DefaultTransformPluginDesc() );
    descriptors.push_back( new DefaultRectPluginDesc() );
    descriptors.push_back( new DefaultColorPluginDesc() );
    descriptors.push_back( new DefaultTexturePluginDesc() );

    for( auto descr : descriptors )
    {
        printf( "Registered plugin desc: %s\n", descr->GetPluginTypeUID().c_str() ); 
    }

    return descriptors;
}

} //model
} //bv
