#include "MultipassNodeBuilder.h"
#include "Common/GeometryBuilder.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"


namespace bv {


MultipassNodeBuilder::MultipassNodeBuilder(void)
{
}


MultipassNodeBuilder::~MultipassNodeBuilder(void)
{
}


/**
@param[in] passes Number of passes and number of textures in textureFile.*/
SceneNode *		MultipassNodeBuilder::CreateMultipassRectNodeTexture( float w, float h, float z, std::string** textureFile, unsigned short passes )
{
	passes;

    RenderableEffectPtr effect      = CreateMultipassRenderableEffect( ShaderDataSourceType::SDST_ONE_TEXTURE, passes, textureFile, "Tex" );
   
    auto vaobuf                     = GeometryBuilder::CreateRectangle( w, h, z, true );
    RenderableEntity *  renderable  = CreateRenderableTriStrip( vaobuf, effect );
    SceneNode *         node        = new SceneNode( static_cast< TransformableEntity * >( renderable ) );

    return node;
}

RenderableEffectPtr MultipassNodeBuilder::CreateMultipassRenderableEffect( ShaderDataSourceType sdst, unsigned short passes, std::string** textureFile, const std::string & textureName )
{
	// Replace everything
	assert( passes > 0 );
    static std::vector< IShaderDataSourceConstPtr > dummyFuckerReferenceKeeper; // FIXME:L keeps references to shader params - this suxx as hell - and must be fixed in the model and engine

	float alfa =  1.0;

    // FIXME: implement
    // DefaultMultipassEffectPtr effect;

	//IShaderDataSourceConstPtr fsds;
	IShaderDataSourceConstPtr vsds;
	
    // FIXME: implement
	//RenderablePass* newPass;
	//RendererStateInstance* stateInstance;


	for( unsigned short pass = 0; pass < passes; pass++ )
	{
		/*alfa = 0.5;*/
		alfa = (float)(passes - pass) / (float)passes;

		IShaderDataSourceConstPtr fsds = ShaderDataSourceCreator::FragmentShader( sdst, alfa, *(textureFile[pass]), textureName + std::to_string(pass) );
		dummyFuckerReferenceKeeper.push_back( fsds );
		if( fsds == nullptr )
			return nullptr;

		if( pass == 0 )
		{
			// FIXME: these pointers get deleted when function leaves this scope so this is bound to fail, these references must be kept somewhere
			vsds = ShaderDataSourceCreator::VertexShader( sdst );
			dummyFuckerReferenceKeeper.push_back( vsds );

            // FIXME: implement
			//effect = std::make_shared<DefaultMultipassEffect>( fsds.get(), vsds.get(), nullptr );

			//stateInstance = effect->GetPass( 0 )->GetStateInstance();
		}
		else
		{
			// FIXME: implement
            //newPass = effect->addPass( fsds.get(), vsds.get(), nullptr );
			//stateInstance = newPass->GetStateInstance();
		}
		
		AlphaState alfaState;
		alfaState.blendEnabled = true;
		alfaState.blendColor = glm::vec4( alfa, alfa, alfa, alfa );
		alfaState.dstBlendMode = AlphaDstBlendMode::ADBM_ONE_MINUS_SRC_ALPHA;
		alfaState.srcBlendMode = AlphaSrcBlendMode::ASBM_SRC_ALPHA;
		
		//stateInstance->SetState( std::make_shared<AlphaState>( alfaState ) );
	}

	// FIXME: implement
    //return effect;

    return nullptr;
}


}
