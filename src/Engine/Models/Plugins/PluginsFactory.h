#pragma once

#include "Engine/Models/Plugins/Interfaces/IAttributeChannel.h"

#include "Mathematics/Interpolators/Interpolators.h"

#include "Engine/Models/Plugins/Simple/SimpleTextPlugin.h"
#include "Engine/Models/Plugins/Simple/SimpleTexturePlugin.h"
#include "Engine/Models/Plugins/Simple/SimpleAnimationPlugin.h"
#include "Engine/Models/Plugins/Simple/SimpleGeometryShaderPlugin.h"
#include "Engine/Models/Plugins/Simple/SimplePixelShaderPlugin.h"
#include "Engine/Models/Plugins/Simple/SimpleVertexShaderPlugin.h"
#include "Engine/Models/Plugins/Simple/SimpleColorPlugin.h"
#include "Engine/Models/Plugins/GeometryPlugin.h"
#include "Engine/Models/Plugins/GeometryPluginRect.h"
#include "Engine/Models/Plugins/SolidColorPlugin.h"

#include "Engine/Models/Plugins/Channels/Geometry/Simple/RectComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/AnimatedStripComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/RingComponent.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"

#include "Engine/Models/Plugins/Channels/Transform/SimpleTransformChannel.h"

#include "Engine/Models/Plugins/Channels/VertexShader/TextureVertexShaderChannel.h"

#include "Engine/Models/Plugins/Channels/GeometryShader/ExtrudeGeometryShaderChannel.h"

#include "Engine/Models/Plugins/Channels/PixelShader/SolidColorShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/TexturePixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/TextPixelShaderChannel.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"


namespace bv
{
namespace model 
{

class BasicNode;
class SimpleTransformPlugin;
class GeometryRingPlugin;
class GeometryMultiRectPlugin;
class Timeline;

struct PluginsFactory
{
	static model::GeometryRectPlugin *          CreateGeometryRectPlugin            ( float w, float h );
    static model::GeometryMultiRectPlugin *     CreateGeometryMultiRectPlugin       ();
    static model::GeometryRingPlugin *          CreateGeometryRingPlugin            ( float startAngle, float endAngle, float innerRadius, float outerRadius, int segmentsNum );

    // Simple plugins builders.
    static model::SimpleColorPlugin *           CreateSimpleColorPlugin             ( model::IPlugin * prevPlugin, const ParamVec4 & color );

    static model::SimpleTransformPlugin *       CreateSimpleTransformPlugin         ( const model::IPlugin * prev, const ParamTransform& transformation );

    static model::SimplePixelShaderPlugin *     CreateSimplePixelShaderPlugin       ( model::IPlugin * prevPlugin, const std::string & shaderPath, model::RendererContext * ctx = nullptr );
    static model::SimpleVertexShaderPlugin *    CreateSimpleVertexShaderPlugin      ( model::IPlugin * prevPlugin, const std::string & shaderPath );
    static model::SimpleGeometryShaderPlugin *  CreateSimpleGeometryShaderPlugin    ( model::IPlugin * prevPlugin, const std::string & shaderPath );
                                                                                                    
    static model::SimpleTexturePlugin *         CreateSimpleTexturePlugin           ( model::IPlugin * prevPlugin, const std::vector< std::string > & texturesPaths, const std::vector< TransformF > txTransforms, model::RendererContext * ctx, TextureAttachmentMode mode = TextureAttachmentMode::MM_ATTACHED );
    static model::SimpleTexturePlugin *         CreateSimpleTexturePlugin           ( model::IPlugin * prevPlugin, const std::vector< std::string > & texturesPaths, const std::vector< TransformF > txTransforms, TextureAttachmentMode mode = TextureAttachmentMode::MM_ATTACHED );
	
    static model::SimpleAnimationPlugin *       CreateSimpleAnimationPlugin         ( model::IPlugin * prevPlugin, const std::vector< std::string > & texturesPaths, const ParamFloat & frameCounter, model::RendererContext * ctx = nullptr );
	static model::SimpleTextPlugin *            CreateSimpleTextPlugin              ( const std::wstring & text, const std::string & fontFile, int size, bool bolded = false, bool italic = false );


    // To jest jeszcze jakis smietnik 

    static model::SolidColorPlugin *            CreateSolidColorPlugin              ( model::IPlugin * prevPlugin, const glm::vec4 & color);

    static model::RendererContext *             CreateDefaultRenderableContext      ();

};

} // model
} // bv
