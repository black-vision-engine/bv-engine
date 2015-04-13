#include "DefaultPlugins.h"

#include "Engine/Models/Plugins/Interfaces/IPluginDescriptor.h"

#include "Engine/Models/Plugins/Simple/DefaultRectPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTransformPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultColorPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultGradientPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTexturePlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultAnimationPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTextPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTimerPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultAlphaMaskPlugin.h"
#include "Engine/Models/Plugins/Custom/DefaultHeightMapPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultPrismPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultPieChartPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultVideoInputPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultCirclePlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultEllipsePlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTrianglePlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultRoundedRectPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultConePlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultCubePlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultSimpleCubePlugin.h"

namespace bv { namespace model {

// *********************************
//
std::vector< IPluginDescriptor * >  DefaultBVPluginDescriptors  ()
{
    std::vector< IPluginDescriptor * > descriptors;

    descriptors.push_back( new DefaultTransformPluginDesc() );
    descriptors.push_back( new DefaultRectPluginDesc() );
    descriptors.push_back( new DefaultColorPluginDesc() );
	descriptors.push_back( new DefaultGradientPluginDesc() );
    descriptors.push_back( new DefaultTexturePluginDesc() );
    descriptors.push_back( new DefaultAnimationPluginDesc() );
    descriptors.push_back( new DefaultTextPluginDesc() );
    descriptors.push_back( new DefaultAlphaMaskPluginDesc() );
    descriptors.push_back( new DefaultTimerPluginDesc() );
    descriptors.push_back( new DefaultHeightMapPluginDesc() );
	descriptors.push_back( new DefaultPrismPluginDesc() );
	descriptors.push_back( new DefaultPieChartPluginDesc() );
	descriptors.push_back( new DefaultVideoInputPluginDesc() );
	descriptors.push_back( new DefaultCirclePluginDesc() );
	descriptors.push_back( new DefaultEllipsePluginDesc() );
    descriptors.push_back( new DefaultTrianglePluginDesc() );
    descriptors.push_back( new DefaultRoundedRectPluginDesc() );
    descriptors.push_back( new DefaultCone::DefaultConePluginDesc() );
    descriptors.push_back( new DefaultCube::PluginDesc() );
	descriptors.push_back( new DefaultSimpleCube::PluginDesc() );

    for( auto descr : descriptors )
    {
        printf( "Registered plugin desc: %s\n", descr->GetPluginTypeUID().c_str() ); 
    }

    return descriptors;
}

} //model
} //bv
