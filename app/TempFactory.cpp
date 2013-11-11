#include "TempFactory.h"
#include "Engine/Models/Plugins/SimpleTransformPlugin.h"
#include "Engine/Models/Plugins/GeometryPluginRing.h"
#include "Engine/Models/Plugins/Channels/RendererContext/RendererContext.h"

namespace bv
{

// *******************************
//
FloatInterpolator                   CreateConstValueFloat               ( float val )
{
	FloatInterpolator inter; inter.setWrapPostMethod( bv::WrapMethod::pingPong );
	inter.addKey( 0.f, val );

	return inter;
}

// *******************************
//
Vec4Interpolator                   CreateConstValueVec4                 ( const glm::vec4& val )
{
	Vec4Interpolator inter; inter.setWrapPostMethod( bv::WrapMethod::pingPong );
	inter.addKey( 0.f, val );

	return inter;
}

// *******************************
//
model::GeometryRectPlugin*          CreateGeometryRectPlugin            ( float w, float h )
{
	FloatInterpolator wi; wi.setWrapPostMethod( bv::WrapMethod::pingPong );
	FloatInterpolator hi; hi.setWrapPostMethod( bv::WrapMethod::pingPong );
    
	wi.addKey(0.f, w);
	hi.addKey(0.f, h);

    auto rectPlugin = new model::GeometryRectPlugin(wi, hi);

    /// Set Geometry Channel
    model::RectComponent *     rect        = model::RectComponent::Create( w, h );

    model::GeometryChannel *   geomCh      = CreateGeometryChannel( rect );

    rectPlugin->SetGeometryChannel( geomCh );

	return rectPlugin;
}

// *******************************
//
model::GeometryRingPlugin*          CreateGeometryRingPlugin            ( float startAngle, float endAngle, float innerRadius, float outerRadius, int segmentsNum )
{
    model::GeometryRingPlugin*  ringPlugin = new model::GeometryRingPlugin();

    // Set Geometry Channel

    model::RingComponent *      ring        = model::RingComponent::Create( startAngle, endAngle, innerRadius, outerRadius, segmentsNum );

    model::GeometryChannel *    geomCh      = CreateGeometryChannel( ring );

    ringPlugin->SetGeometryChannel( geomCh );

    return ringPlugin;
}

// *******************************
//
model::GeometryChannel*             CreateGeometryChannel               (model::IConnectedComponent* connComp)
{
	model::GeometryChannelDescriptor desc;

	for( auto compDesc : connComp->GetVertexAttributeChannels() )
	{
		desc.AddVertexAttrChannelDesc( static_cast< const model::VertexAttributeChannelDescriptor * >( compDesc->GetDescriptor() ) );
	}

	model::GeometryChannel* ret = new model::GeometryChannel( PrimitiveType::PT_TRIANGLE_STRIP, desc );

	ret->AddConnectedComponent(connComp);

	return ret;
}

// *******************************
//
model::TransformChannel*           CreateTransformChannel              (TransformF* transformation)
{
	model::SimpleTransformChannel*      trasformChannel  = new model::SimpleTransformChannel();
	trasformChannel->AddTransform( transformation );

	return trasformChannel;
}

// *******************************
//
model::SolidColorPlugin*            CreateSolidColorPlugin              (model::IPlugin* prevPlugin, const glm::vec4& color)
{
	auto solidPlugin = new model::SolidColorPlugin( prevPlugin );

	// Set Pixel Shader Channel
	solidPlugin->SetPixelShaderChannel    ( new model::SolidColorShaderChannel( "../dep/media/shaders/solid.frag", CreateConstValueVec4( color ) ) );

	return solidPlugin;
}

// *******************************
//
model::SimpleTexturePlugin*         CreateTexturePlugin                 ( model::IPlugin* prevPlugin, const std::vector< std::string >& texturesPaths )
{
	auto texturePlugin = new model::SimpleTexturePlugin( prevPlugin, texturesPaths );

	// Set Pixel Shader Channel
	std::vector<TransformF> txMat;
	std::vector<FloatInterpolator> alphas;

    auto pixelShaderChannel = new model::TexturePixelShaderChannel( "../dep/media/shaders/simpletexture.frag"
										, alphas
										, txMat );

    pixelShaderChannel->SetRendererContext( CreateDefaultRenderableContext() );
    auto rendContext = pixelShaderChannel->GetRendererContext();
    rendContext->cullCtx->enabled = false;

	texturePlugin->SetPixelShaderChannel( pixelShaderChannel );

	texturePlugin->SetVertexShaderChannel( new model::TextureVertexShaderChannel( "../dep/media/shaders/simpletexture.vert" )
										);
	return texturePlugin;
}

// *******************************
//
model::SimpleAnimationPlugin *      CreateAnimationPlugin               ( model::IPlugin * prevPlugin, const std::vector< std::string > & texturesPaths, unsigned int animationFPS )
{
	auto animationPlugin = new model::SimpleAnimationPlugin( prevPlugin, texturesPaths, animationFPS );

	// Set Pixel Shader Channel
	std::vector<TransformF> txMat;
	std::vector<FloatInterpolator> alphas;

    auto pixelShaderChannel = new model::TexturePixelShaderChannel( "../dep/media/shaders/simpleanimation.frag", alphas, txMat );

    pixelShaderChannel->SetRendererContext( CreateDefaultRenderableContext() );
    auto rendContext = pixelShaderChannel->GetRendererContext();
    rendContext->cullCtx->enabled = false;

	animationPlugin->SetPixelShaderChannel( pixelShaderChannel );

	animationPlugin->SetVertexShaderChannel( new model::TextureVertexShaderChannel( "../dep/media/shaders/simpleanimation.vert" ) );

    return animationPlugin;
}

// *******************************
//
model::SimpleTextPlugin *            CreateTextPlugin                    ( const std::wstring & text, const std::string & fontFile, int size )
{
    auto texPlugin = model::SimpleTextPlugin::Create( text, fontFile, size );

	return texPlugin;
}

// *******************************
//
model::SimpleTransformPlugin *      CreateTransformPlugin               ( const model::IPlugin * prev, TransformF * trans )
{
    return model::SimpleTransformPlugin::Create( prev, trans );
}

// *******************************
//
model::IGeometryShaderChannel *     CreateGeometryShaderExtrude         ( float scale )
{
	FloatInterpolator extrudeScale = CreateConstValueFloat( scale );
	return new model::ExtrudeGeometryShaderChannel("../dep/media/shaders/extrude.geom", extrudeScale);
}

// *******************************
//
size_t GetSizeOfFile( const std::wstring& path )
{
	struct _stat fileinfo;
	_wstat(path.c_str(), &fileinfo);
	return fileinfo.st_size;
}

// *******************************
//
std::wstring LoadUtf8FileToString(const std::wstring& filename)
{
	std::wstring buffer;            // stores file contents
	FILE* f = nullptr;
    _wfopen_s(&f, filename.c_str(), L"rtS, ccs=UTF-8");

	// Failed to open file
	if (f == NULL)
	{
		// ...handle some error...
		return buffer;
	}

	size_t filesize = GetSizeOfFile(filename);

	// Read entire file contents in to memory
	if (filesize > 0)
	{
		buffer.resize(filesize);
		size_t wchars_read = fread(&(buffer.front()), sizeof(wchar_t), filesize, f);
		buffer.resize(wchars_read);
		buffer.shrink_to_fit();
	}

	fclose(f);

	return buffer;
}

// *******************************
//
model::SimpleColorPlugin*           CreateSimpleColorPlugin             ( model::IPlugin* prevPlugin, const Vec4Interpolator& color)
{
    return new model::SimpleColorPlugin( prevPlugin, color );
}

// *******************************
//
model::SimplePixelShaderPlugin*     CreateSimplePixelShaderPlugin       ( model::IPlugin* prevPlugin, const std::string& shaderPath, model::RendererContext * ctx )
{
    return new model::SimplePixelShaderPlugin( prevPlugin, shaderPath, ctx );
}

// *******************************
//
model::SimpleVertexShaderPlugin*    CreateSimpleVertexShaderPlugin      ( model::IPlugin* prevPlugin, const std::string& shaderPath)
{
    return new model::SimpleVertexShaderPlugin( prevPlugin, shaderPath );
}

// *******************************
//
model::SimpleGeometryShaderPlugin*  CreateSimpleGeometryShaderPlugin    ( model::IPlugin* prevPlugin, const std::string& shaderPath)
{
    return new model::SimpleGeometryShaderPlugin( prevPlugin, shaderPath );
}

// *******************************
//
model::RendererContext*             CreateDefaultRenderableContext()
{
    auto ctx = new model::RendererContext();
    ctx->alphaCtx = new model::AlphaContext();
    ctx->cullCtx = new model::CullContext();
    ctx->depthCtx = new model::DepthContext();
    ctx->fillCtx = new model::FillContext();

    return ctx;
}

} //bv
