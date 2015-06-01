#include "MultipassNodeBuilder.h"
#include "Prototypes/Engine/Common/GeometryBuilder.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"


namespace bv {


MultipassNodeBuilder::MultipassNodeBuilder(void)
{
}


MultipassNodeBuilder::~MultipassNodeBuilder(void)
{
}


SceneNode *		MultipassNodeBuilder::CreateMultipassRectNodeTexture( float w, float h, float z, const std::string & textureFile, unsigned short passes )
{
	passes;

    RenderableEffectPtr effect      = CreateMultipassRenderableEffect( ShaderDataSourceType::SDST_ONE_TEXTURE, passes, textureFile, "Tex" );
   
    auto vaobuf                     = GeometryBuilder::CreateRectangle( w, h, z, true );
    RenderableEntity *  renderable  = CreateRenderableTriStrip( vaobuf, effect );
    SceneNode *         node        = new SceneNode( static_cast< TransformableEntity * >( renderable ) );

    return node;
}

RenderableEffectPtr MultipassNodeBuilder::CreateMultipassRenderableEffect( ShaderDataSourceType sdst, unsigned short passes, const std::string & textureFile, const std::string & textureName )
{
	// Replace everything
	passes;
    static std::vector< IShaderDataSourceConstPtr > dummyFuckerReferenceKeeper; // FIXME:L keeps references to shader params - this suxx as hell - and must be fixed in the model and engine

    // FIXME: these pointers get deleted when function leaves this scope so this is bound to fail, these references must be kept somewhere
    auto vsds = ShaderDataSourceCreator::VertexShader( sdst );
    auto fsds = ShaderDataSourceCreator::FragmentShader( sdst, textureFile, textureName );

    dummyFuckerReferenceKeeper.push_back( vsds );
    dummyFuckerReferenceKeeper.push_back( fsds );

    return std::make_shared<DefaultEffect>( fsds.get(), vsds.get(), nullptr );
}


}
