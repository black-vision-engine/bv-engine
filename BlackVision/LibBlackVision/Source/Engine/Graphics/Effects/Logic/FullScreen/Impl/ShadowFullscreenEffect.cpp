#include "ShadowFullscreenEffect.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"

#include "Engine/Types/Values/ValuesFactory.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamVec2.h"



#include "Memory/MemoryLeaks.h"



namespace bv {

// **************************
//
void            ShadowFullscreenEffect::SetColor      ( const glm::vec4 & color )
{
    m_color = color;
    m_colorVal->SetValue( color );

    if( m_pixelShader )
    {
        static_cast< ShaderParamVec4 * >( m_pixelShader->GetParameters()->AccessParam( 0 ) )->SetValue( color );
    }
}

// **************************
//
glm::vec4       ShadowFullscreenEffect::GetColor      () const
{
    return m_color;
}

// **************************
//
void            ShadowFullscreenEffect::SetShift      ( const glm::vec2 & xy )
{
    m_shift = xy;
    m_shiftVal->SetValue( xy );

    if( m_pixelShader )
    {
        static_cast< ShaderParamVec2 * >( m_pixelShader->GetParameters()->AccessParam( 1 ) )->SetValue( xy );
    }
}

// **************************
//
glm::vec2       ShadowFullscreenEffect::GetShift      () const
{
    return m_shift;
}

// **************************
//
void            ShadowFullscreenEffect::SetInner     ( Int32 flag )
{
    m_innerVal->SetValue( flag );

    if( m_pixelShader )
    {
        static_cast< ShaderParamInt * >( m_pixelShader->GetParameters()->AccessParam( 2 ) )->SetValue( flag );
    }
}

// **************************
//
PixelShader *   ShadowFullscreenEffect::CreatePS            () const
{
    ShaderParameters * shaderParams = new ShaderParameters();

    GenericShaderParam * param = ShaderParamFactory::CreateGenericParameter( m_colorVal.get() );
    shaderParams->AddParameter( param );

    param = ShaderParamFactory::CreateGenericParameter( m_shiftVal.get() );
    shaderParams->AddParameter( param );

    param = ShaderParamFactory::CreateGenericParameter( m_innerVal.get() );
    shaderParams->AddParameter( param );

    param = ShaderParamFactory::CreateGenericParameter( m_textureSize.get() );
    shaderParams->AddParameter( param );

    shaderParams->AddTexture( m_texture );
    shaderParams->AddTexture( m_bluredTexture );

    //FIXME: add empty textures (nullptr) and create samplers. Textures can be set later on
    auto shader = new PixelShader( ReadFullscreenShader( "shadow.frag" ), shaderParams );

    auto samplerTex = CreateDefaultSampler( "Tex0" );
    auto samplerBluredTex = CreateDefaultSampler( "BluredTex0" );

    shader->AddTextureSampler( samplerTex );
    shader->AddTextureSampler( samplerBluredTex );

    return shader;
}

// **************************
//
ShadowFullscreenEffect::ShadowFullscreenEffect      ( Texture2DPtr tex, Texture2DPtr bluredTexture )
    : m_pixelShader( nullptr )
{
    m_colorVal      = ValuesFactory::CreateValueVec4( "color" );
    m_shiftVal      = ValuesFactory::CreateValueVec2( "shift" );
    m_innerVal      = ValuesFactory::CreateValueInt( "inner" );

    m_textureSize   = ValuesFactory::CreateValueVec2( "textureSize" );

    m_textureSize->SetValue( glm::vec2( float( tex->GetWidth() ), float( tex->GetHeight() ) ) );

    m_texture       = tex;
    m_bluredTexture = bluredTexture;
}

// **************************
//
ShadowFullscreenEffect::~ShadowFullscreenEffect     ()
{}

// ****************************
//
RenderableEntity *  ShadowFullscreenEffect::CreateFullscreenQuad    () const
{
    m_pixelShader = CreatePS();
    auto quad = CreateDefaultFullscrQuad( m_pixelShader );

    auto eff  = quad->GetRenderableEffect();
    auto pass = eff->GetPass( 0 );

    auto sinst = pass->GetStateInstance();
    
    auto as = RenderStateAccessor::AccessAlphaState( sinst );

    as->blendEnabled = true;

    return quad;
}

} // bv
