#include "DefaultPlugins.h"

#include "Engine/Models/Plugins/Interfaces/IPluginDescriptor.h"

#include "Engine/Models/Plugins/Simple/DefaultRectPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTransformPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultColorPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultGradientPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTexturePlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultAnimationPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultAVDecoderPlugin.h"
#include "Engine/Models/Plugins/Simple/TextPlugins/DefaultTextPlugin.h"
#include "Engine/Models/Plugins/Simple/TextPlugins/DefaultTimerPlugin.h"
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
#include "Engine/Models/Plugins/Simple/DefaultTorusPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultNoPerspectiveTexturePlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultSpherePlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultGeospherePlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultSpringPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultCylinderPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultCogWheelPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultMaterialPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultWallPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultMeshPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultNormalMapPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultParallaxMapPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultEnvironmentTexturePlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultEnvReflectivityMapPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultBlendTexturePlugin.h"


namespace bv { namespace model {

// *********************************
//
std::vector< IPluginDescriptor * >  DefaultBVPluginDescriptors  ()
{
    std::vector< IPluginDescriptor * > descriptors;

    descriptors.push_back( new DefaultTransformPluginDesc() );
    descriptors.push_back( new DefaultRect::PluginDesc() );
    descriptors.push_back( new DefaultColorPluginDesc() );
	descriptors.push_back( new DefaultGradientPluginDesc() );
    descriptors.push_back( new DefaultTexturePluginDesc() );
	descriptors.push_back( new DefaultNoPerspectiveTexturePluginDesc() );
    descriptors.push_back( new DefaultAnimationPluginDesc() );
    descriptors.push_back( new DefaultAVDecoderPluginDesc() );
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
    descriptors.push_back( new DefaultTorus::PluginDesc() );
	descriptors.push_back( new DefaultSphere::PluginDesc() );
	descriptors.push_back( new DefaultGeosphere::PluginDesc() );
    descriptors.push_back( new DefaultSpring::PluginDesc() );
	descriptors.push_back( new DefaultCylinder::DefaultCylinderPluginDesc() );
	descriptors.push_back( new DefaultCogWheel::PluginDesc() );
    descriptors.push_back( new DefaultMaterialPluginDesc() );
    descriptors.push_back( new DefaultWallPlugin::PluginDesc() );
    descriptors.push_back( new DefaultMeshPluginDesc() );
    descriptors.push_back( new DefaultNormalMapPluginDesc() );
    descriptors.push_back( new DefaultParallaxMapPluginDesc() );
    descriptors.push_back( new DefaultEnvironmentTexturePluginDesc() );
    descriptors.push_back( new DefaultEnvReflectivityMapPluginDesc() );
    descriptors.push_back( new DefaultBlendTexturePluginDesc() );

    for( auto descr : descriptors )
    {
        printf( "Registered plugin desc: %s\n", descr->GetPluginTypeUID().c_str() ); 
    }

    return descriptors;
}

} //model
} //bv
