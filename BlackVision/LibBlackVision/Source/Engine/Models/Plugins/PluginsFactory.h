#pragma once

#include "Engine/Models/Plugins/Interfaces/IAttributeChannel.h"

#include "Mathematics/Interpolators/Interpolators.h"

#include "Engine/Models/Plugins/GeometryPlugin.h"
#include "Engine/Models/Plugins/GeometryPluginRect.h"

#include "Engine/Models/Plugins/Channels/Geometry/Simple/RectComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/AnimatedStripComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/RingComponent.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"

#include "Engine/Models/Plugins/Channels/Transform/SimpleTransformChannel.h"

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
class TimerPlugin;

namespace PluginsFactory
{
	model::GeometryRectPlugin *          CreateGeometryRectPlugin            ( float w, float h, ITimeEvaluatorPtr timeEvaluator );
 //   static model::GeometryMultiRectPlugin *     CreateGeometryMultiRectPlugin       ();
 //   static model::GeometryRingPlugin *          CreateGeometryRingPlugin            ( float startAngle, float endAngle, float innerRadius, float outerRadius, int segmentsNum );

 //   // Simple plugins builders.
 //   static model::SimpleColorPlugin *           CreateSimpleColorPlugin             ( model::IPlugin * prevPlugin, const ParamVec4 & color );

 //   static model::SimpleTransformPlugin *       CreateSimpleTransformPlugin         ( const model::IPlugin * prev, const ParamTransformVec & transformVec );

 //   static model::SimplePixelShaderPlugin *     CreateSimplePixelShaderPlugin       ( model::IPlugin * prevPlugin, const std::string & shaderPath, model::RendererContext * ctx = nullptr );
 //   static model::SimpleVertexShaderPlugin *    CreateSimpleVertexShaderPlugin      ( model::IPlugin * prevPlugin, const std::string & shaderPath );
 //   static model::SimpleGeometryShaderPlugin *  CreateSimpleGeometryShaderPlugin    ( model::IPlugin * prevPlugin, const std::string & shaderPath );
 //                                                                                                   
 //   static model::SimpleTexturePlugin *         CreateSimpleTexturePlugin           ( model::IPlugin * prevPlugin, const std::vector< const TextureDescriptor > & textureDescs, model::RendererContext * ctx, TextureAttachmentMode mode = TextureAttachmentMode::MM_ATTACHED );
 //   static model::SimpleTexturePlugin *         CreateSimpleTexturePlugin           ( model::IPlugin * prevPlugin, const std::vector< const TextureDescriptor > & textureDescs, TextureAttachmentMode mode = TextureAttachmentMode::MM_ATTACHED );
	//
 //   static model::SimpleAnimationPlugin *       CreateSimpleAnimationPlugin         ( model::IPlugin * prevPlugin, const std::vector< std::string > & texturesPaths, const ParamFloat & frameCounter, model::RendererContext * ctx = nullptr );
	//static model::SimpleTextPlugin *            CreateSimpleTextPlugin              ( const std::wstring & text, const std::string & fontFile, int size, bool bolded = false, bool italic = false );

 //   static model::TimerPlugin *                 CreateTimerPlugin                   ( const ParamFloat & time, unsigned int size );

 //   // More complex plugins.
 //   // return last plugin added to root node.
 //   static model::IPlugin *                     CreateSolidColorPlugin              ( BasicNode* root, model::IPlugin * prevPlugin, const glm::vec4 & color);
};

} // model
} // bv
