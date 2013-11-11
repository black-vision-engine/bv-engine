#pragma once

#include "Engine/Models/Plugins/Interfaces/IVertexAttributeChannel.h"

#include "Mathematics/Interpolators/Interpolators.h"

#include "Engine/Models/Plugins/SimpleTextPlugin.h"
#include "Engine/Models/Plugins/SimpleTexturePlugin.h"
#include "Engine/Models/Plugins/SimpleAnimationPlugin.h"
#include "Engine/Models/Plugins/SimpleGeometryShaderPlugin.h"
#include "Engine/Models/Plugins/SimplePixelShaderPlugin.h"
#include "Engine/Models/Plugins/SimpleVertexShaderPlugin.h"
#include "Engine/Models/Plugins/SimpleColorPlugin.h"
#include "Engine/Models/Plugins/GeometryPlugin.h"
#include "Engine/Models/Plugins/GeometryPluginRect.h"
#include "Engine/Models/Plugins/SolidColorPlugin.h"

#include "Engine/Models/Plugins/Channels/Geometry/Simple/RectComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/AnimatedStripComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/RingComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/GeometryChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/GeometryChannel.h"

#include "Engine/Models/Plugins/Channels/Transform/SimpleTransformChannel.h"

#include "Engine/Models/Plugins/Channels/VertexShader/TextureVertexShaderChannel.h"

#include "Engine/Models/Plugins/Channels/GeometryShader/ExtrudeGeometryShaderChannel.h"

#include "Engine/Models/Plugins/Channels/PixelShader/SolidColorShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/TexturePixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/TextPixelShaderChannel.h"


namespace bv
{
    namespace model 
	{
		class BasicNode;
        class SimpleTransformPlugin;
        class GeometryRingPlugin;
	}


	FloatInterpolator                   CreateConstValueFloat               ( float val );
	
	Vec4Interpolator                    CreateConstValueVec4                 ( const glm::vec4& val );


	model::GeometryRectPlugin*          CreateGeometryRectPlugin            ( float w, float h );

    model::GeometryRingPlugin*          CreateGeometryRingPlugin            ( float startAngle, float endAngle, float innerRadius, float outerRadius, int segmentsNum );
	
	model::GeometryChannel*             CreateGeometryChannel               (model::IConnectedComponent* connComp);
	

	model::TransformChannel*            CreateTransformChannel              (TransformF* transformation);
	
    model::SimpleColorPlugin*           CreateSimpleColorPlugin             ( model::IPlugin* prevPlugin, const Vec4Interpolator& color);
    model::SimplePixelShaderPlugin*     CreateSimplePixelShaderPlugin       ( model::IPlugin* prevPlugin, const std::string& shaderPath, model::RendererContext * ctx = nullptr );
    model::SimpleVertexShaderPlugin*    CreateSimpleVertexShaderPlugin      ( model::IPlugin* prevPlugin, const std::string& shaderPath);
    model::SimpleGeometryShaderPlugin*  CreateSimpleGeometryShaderPlugin    ( model::IPlugin* prevPlugin, const std::string& shaderPath);

	model::SolidColorPlugin*            CreateSolidColorPlugin              ( model::IPlugin* prevPlugin, const glm::vec4& color);
	

	model::SimpleTexturePlugin *        CreateTexturePlugin                 ( model::IPlugin * prevPlugin, const std::vector< std::string > & texturesPaths );
	model::SimpleAnimationPlugin *      CreateAnimationPlugin               ( model::IPlugin * prevPlugin, const std::vector< std::string > & texturesPaths, unsigned int animationFPS );
	
    model::SimpleTransformPlugin*       CreateTransformPlugin               ( const model::IPlugin* prev, TransformF* trans );



	model::SimpleTextPlugin*            CreateTextPlugin                    ( const std::wstring& text, const std::string& fontFile, int size, bool bolded = false, bool italic = false);
	model::IGeometryShaderChannel*      CreateGeometryShaderExtrude         ( float scale );

    model::RendererContext*             CreateDefaultRenderableContext      ();
}