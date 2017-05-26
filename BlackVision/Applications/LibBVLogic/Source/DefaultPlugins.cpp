#include "stdafxBVApp.h"

#include "DefaultPlugins.h"

#include "Engine/Models/Plugins/Interfaces/IPluginDescriptor.h"

#include "Engine/Models/Plugins/Simple/Shapes/DefaultRectPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTransformPlugin.h"
#include "Engine/Models/Plugins/Simple/AVStreamDecoder/DefaultAVDecoderPlugin.h"
#include "Engine/Models/Plugins/Simple/AVStreamDecoder/DefaultAudioDecoderPlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultColorPlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultGradientPlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultTexturePlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultAnimationPlugin.h"
#include "Engine/Models/Plugins/Simple/TextPlugins/DefaultTextPlugin.h"
#include "Engine/Models/Plugins/Simple/TextPlugins/DefaultText3DPlugin.h"
#include "Engine/Models/Plugins/Simple/TextPlugins/DefaultTimerPlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultAlphaMaskPlugin.h"
#include "Engine/Models/Plugins/Custom/DefaultHeightMapPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultPrismPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultPieChartPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultVideoInputPlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultCirclePlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultEllipsePlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultTrianglePlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultRoundedRectPlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultConePlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultCubePlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultSimpleCubePlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultTorusPlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultNoPerspectiveTexturePlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultSpherePlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultGeospherePlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultSpringPlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultCylinderPlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultCogWheelPlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultMaterialPlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultWallPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultMeshPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultSVGPlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultNormalMapPlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultParallaxMapPlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultEnvironmentTexturePlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultEnvReflectivityMapPlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultBlendTexturePlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultFadePlugin.h"
#include "Engine/Models/Plugins/Simple/GeometryProcessors/DefaultExtrudePlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultLineChartPlugin.h"
#include "Engine/Models/Plugins/Simple/GeometryProcessors/TriangulatePlugin.h"

#include "UseLoggerBVAppModule.h"


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
    descriptors.push_back( new DefaultAudioDecoderPluginDesc() );
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
    descriptors.push_back( new DefaultCubePluginDesc() );
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
    descriptors.push_back( new DefaultSVGPluginDesc() );
    descriptors.push_back( new DefaultNormalMapPluginDesc() );
    descriptors.push_back( new DefaultParallaxMapPluginDesc() );
    descriptors.push_back( new DefaultEnvironmentTexturePluginDesc() );
    descriptors.push_back( new DefaultEnvReflectivityMapPluginDesc() );
    descriptors.push_back( new DefaultBlendTexturePluginDesc() );
    descriptors.push_back( new DefaultFadePluginDesc() );
    descriptors.push_back( new DefaultExtrudePluginDesc() );
    descriptors.push_back( new DefaultLineChartPluginDesc() );
    descriptors.push_back( new TriangulatePluginDesc() );
    descriptors.push_back( new DefaultText3DPluginDesc() );

    for( auto descr : descriptors )
    {
        LOG_MESSAGE( SeverityLevel::info ) << "Registered plugin desc: " << descr->GetPluginTypeUID();
    }

    return descriptors;
}

} //model
} //bv
