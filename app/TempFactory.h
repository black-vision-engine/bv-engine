#pragma once
#include "Mathematics\Interpolators\Interpolators.h"

#include "Engine/Models/Plugins/GeometryPlugin.h"
#include "Engine/Models/Plugins/GeometryPluginRect.h"
#include "Engine/Models/Plugins/Channels/Transform/SimpleTransformChannel.h"
#include "Engine/Models/Plugins/SolidColorPlugin.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/RectComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/AnimatedStripComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/RingComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/GeometryChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Interfaces/IVertexAttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/GeometryChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/SolidColorShaderChannel.h"
#include "Engine/Models/Plugins/SimpleTexturePlugin.h"
#include "Engine/Models/Plugins/Channels/PixelShader/TexturePixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/VertexShader/TextureVertexShaderChannel.h"
#include "Engine\Models\Plugins\SimpleTextPlugin.h"
#include "Engine\Models\Plugins\Channels\PixelShader\TextPixelShaderChannel.h"
#include "Engine\Models\Plugins\Channels\GeometryShader\ExtrudeGeometryShaderChannel.h"

namespace bv
{
    namespace model 
	{
		class BasicNode;
	}


	FloatInterpolator                   CreateConstValueFloat               ( float val );
	
	Vec4Interpolator                   CreateConstValueVec4                 ( const glm::vec4& val );


	model::GeometryRectPlugin*          CreateGeometryRectPlugin            ( float w, float h );
	
	model::GeometryChannel*             CreateGeometryChannel               (model::IConnectedComponent* connComp);
	

	model::ITransformChannel*           CreateTransformChannel              (TransformF* transformation);
	

	model::SolidColorPlugin*            CreateSolidColorPlugin              (model::IPlugin* prevPlugin, const glm::vec4& color);
	

	model::SimpleTexturePlugin*         CreateTexturePlugin                 ( model::IPlugin* prevPlugin, const std::vector< std::string >& texturesPaths );
	

	model::SimpleTextPlugin*            CreateTextPlugin                    ( const std::wstring& text, const std::string& fontFile, int size, const Vec4Interpolator& color, TransformF* trans );
	model::IGeometryShaderChannel*      CreateGeometryShaderExtrude         ( float scale );
	size_t GetSizeOfFile(const std::wstring& path);

	std::wstring LoadUtf8FileToString(const std::wstring& filename);
}