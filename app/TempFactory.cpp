#include "TempFactory.h"

namespace bv
{

	FloatInterpolator                   CreateConstValueFloat               ( float val )
	{
		FloatInterpolator inter; inter.setWrapPostMethod( bv::WrapMethod::pingPong );
		inter.addKey( 0.f, val );

		return inter;
	}

	Vec4Interpolator                   CreateConstValueVec4                 ( const glm::vec4& val )
	{
		Vec4Interpolator inter; inter.setWrapPostMethod( bv::WrapMethod::pingPong );
		inter.addKey( 0.f, val );

		return inter;
	}

	model::GeometryRectPlugin*          CreateGeometryRectPlugin            ( float w, float h )
	{
		FloatInterpolator wi; wi.setWrapPostMethod( bv::WrapMethod::pingPong );
		FloatInterpolator hi; hi.setWrapPostMethod( bv::WrapMethod::pingPong );
    
		wi.addKey(0.f, w);
		hi.addKey(0.f, h);

		return new model::GeometryRectPlugin(wi, hi);
	}

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

	model::ITransformChannel*           CreateTransformChannel              (TransformF* transformation)
	{
		model::SimpleTransformChannel*      trasformChannel  = new model::SimpleTransformChannel();
		trasformChannel->AddTransform( transformation );

		return trasformChannel;
	}

	model::SolidColorPlugin*            CreateSolidColorPlugin              (model::IPlugin* prevPlugin, const glm::vec4& color)
	{
		auto solidPlugin = new model::SolidColorPlugin( prevPlugin );

		// Set Pixel Shader Channel
		solidPlugin->SetPixelShaderChannel    ( new model::SolidColorShaderChannel( "../dep/media/shaders/solid.frag", CreateConstValueVec4( color ) ) );

		return solidPlugin;
	}

	model::SimpleTexturePlugin*         CreateTexturePlugin                 ( model::IPlugin* prevPlugin, const std::vector< std::string >& texturesPaths )
	{
		auto texturePlugin = new model::SimpleTexturePlugin( prevPlugin, texturesPaths );

		// Set Pixel Shader Channel
		std::vector<TransformF> txMat;
		std::vector<FloatInterpolator> alphas;
		texturePlugin->SetPixelShaderChannel( new model::TexturePixelShaderChannel( "../dep/media/shaders/simpletexture.frag"
											, alphas
											, txMat )
											);

		texturePlugin->SetVertexShaderChannel( new model::TextureVertexShaderChannel( "../dep/media/shaders/simpletexture.vert" )
											);

		return texturePlugin;
	}

	model::SimpleTextPlugin*            CreateTextPlugin                    ( const std::wstring& text, const std::string& fontFile, int size, const Vec4Interpolator& color, TransformF* trans )
	{
		auto texPlugin = new model::SimpleTextPlugin( text, fontFile, size );

		texPlugin->SetPixelShaderChannel     ( new model::TextPixelShaderChannel( "../dep/media/shaders/text.frag", color ) );
		texPlugin->SetVertexShaderChannel    ( new model::TextureVertexShaderChannel( "../dep/media/shaders/simpletexture.vert" ) );

		model::ITransformChannel      * stch  = CreateTransformChannel( trans );

		texPlugin->SetTransformChannel( stch );

		return texPlugin;
	}

	model::IGeometryShaderChannel*      CreateGeometryShaderExtrude         ( float scale )
	{
		FloatInterpolator extrudeScale = CreateConstValueFloat( scale );
		return new model::ExtrudeGeometryShaderChannel("../dep/media/shaders/extrude.geom", extrudeScale);
	}

	size_t GetSizeOfFile(const std::wstring& path)
	{
		struct _stat fileinfo;
		_wstat(path.c_str(), &fileinfo);
		return fileinfo.st_size;
	}

	std::wstring LoadUtf8FileToString(const std::wstring& filename)
	{
		std::wstring buffer;            // stores file contents
		FILE* f = _wfopen(filename.c_str(), L"rtS, ccs=UTF-8");

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

}