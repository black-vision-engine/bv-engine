#include "stdafx.h"

#include "NFullscreenEffectHelpers.h"

//FIXME: remove superfluous includes
#include "Engine/Graphics/SceneGraph/Camera.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"

#include "Engine/Graphics/Resources/VertexDescriptor.h"
#include "Engine/Graphics/Resources/VertexBuffer.h"
#include "Engine/Graphics/Resources/VertexArray.h"
#include "Engine/Graphics/Resources/RenderableArrayDataArrays.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/Impl/NFullscreenVSShader.h"
#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/Impl/NFullscreenRenderableEffect.h"

#include "Engine/Models/Plugins/EngineConstantsMapper.h"

#include "Engine/Models/Builder/RendererStatesBuilder.h"


namespace bv { namespace nrl {

namespace {

// **************************
//
VertexDescriptor *  CreateVertexDescriptor( unsigned int numUVChannels )
{
    VertexDescriptor * vd = new VertexDescriptor( 1 + numUVChannels );

    vd->SetAttribute( 0, 0, 0, AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION );

    // Three position coordinates stored as floats
    unsigned int offset = 3 * sizeof( float );

    for( unsigned int i = 0; i < numUVChannels; ++i )
    {
        vd->SetAttribute( i + 1, i + 1, offset, AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD );
    
        offset += 2 * sizeof( float );
    }

    vd->SetStride( offset );

    return vd;
}

// **************************
//
RenderableArrayDataArraysSingleVertexBuffer *   CreateTriStripArrayData( unsigned int numVertices, unsigned int numUVChannels, float * vbData )
{
    //3 x float (positions) + numUVChannels x 2 x float (uv mapping for all textures)
    unsigned int vertexSize = 3 * sizeof( float ) + 2 * numUVChannels * sizeof( float );

    VertexBuffer * vb       = new VertexBuffer( 4, vertexSize, DataBuffer::Semantic::S_STATIC );    
    VertexDescriptor * vd   = CreateVertexDescriptor( numUVChannels );

    memcpy( vb->Data(), vbData, numVertices * vertexSize );

    VertexArraySingleVertexBuffer * vao = new VertexArraySingleVertexBuffer( vb, vd );
    vao->AddCCEntry( 4 );

    RenderableArrayDataArraysSingleVertexBuffer * rad = new RenderableArrayDataArraysSingleVertexBuffer( vao );

    return rad;
}

} // anonymous

// **************************
//
Camera *                                        NFullscreenEffectHelpers::DisplayCamera                 ()
{
    //FIXME: upewnic sie, ze tu sa prawdziwe macierze jednostkowe bez zadnych piardow z zaokragleniami (i jesli nie sa, to recznie to zapewnic ustawiajac macierze z palca)
    //Ponizszy kod tworzy poprawne macierze identycznosciowe, ale konstruktor domyslnie tez to robi - kod zostaje dla celow szkoleniowych, ale zostawiamy wyniki z konstruktora macierzy w kamerze
    
    //camera->SetFrame( glm::vec3( 0.f, 0.f, 0.f ), glm::vec3( 0.f, 0.f, -1.f ), glm::vec3( 0.f, 1.f, 0.f ) );
    //camera->SetFrustum( -1.f, 1.f, -1.f, 1.f, 1.f, -1.f );

    //FIXME: pewnie mozna na spokojnie te macierze potem poprawic na identycznosciowe (skoro kod do ich generacji jest poporawny i wiadomo, na jakiej zasadzie dziala)
    static Camera camInstance( false );

    return &camInstance;
}

// **************************
//
TriangleStrip *                                 NFullscreenEffectHelpers::CreateFullscreenRenderable    ( const std::string & pixelShaderSrc, const NTexture2DEntryVec & textures, const IValuePtrVec & values, const std::vector< std::string > & rtInputSamplerNames, const NFullscreenEffectRendererState & rendererState )
{
    auto effect = CreateEffect( pixelShaderSrc, textures, values, rtInputSamplerNames, rendererState );
    auto quad   = CreateFullscreenQuad( effect );

    return quad;
}


// ***********************************************************************************************************************************************************************************************
// ************************************************************************************* Shaders and effects *************************************************************************************
// ***********************************************************************************************************************************************************************************************


// **************************
//
TriangleStrip *                                 NFullscreenEffectHelpers::CreateFullscreenQuad        ( RenderableEffectPtr effect )
{
    unsigned int numUVChannels = 1;

    float * vbData = CreateFullscreenQuadVBData();
    
    auto rad = CreateTriStripArrayData( 4, numUVChannels, vbData );
    // FIXME: nrl - verify nullptr bounding box
    auto ret = new TriangleStrip( rad, nullptr, effect );

    delete[] vbData;

    ret->SetWorldTransform( Transform( glm::mat4( 1.0f ), glm::mat4( 1.0f ) ) );

    return ret;
}

// **************************
//
TextureSampler *                                NFullscreenEffectHelpers::CreateSampler             ( const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode )
{
    auto mappedWrapX    = EngineConstantsMapper::EngineConstant( wrapX );
    auto mappedWrapY    = EngineConstantsMapper::EngineConstant( wrapY );            
    auto samplingMode   = SamplerSamplingMode::SSM_MODE_2D;
    auto sfm            = EngineConstantsMapper::EngineConstant( filteringMode );

    SamplerWrappingMode wrappingMode[] = { mappedWrapX, mappedWrapY, SamplerWrappingMode::SWM_REPEAT };

    auto sampler = new TextureSampler( 0, samplerName, samplingMode, sfm, wrappingMode, glm::vec4( 0.f, 0.f, 0.f, 0.f ) ); 

    return sampler;
}

// **************************
//
TextureSampler *                                NFullscreenEffectHelpers::CreateSampler               ( const NTexture2DEntry & entry )
{
    auto wrapX  = entry.GetWrappingModeX();
    auto wrapY  = entry.GetWrappingModeY();
    auto fm     = entry.GetFilteringMode();

    auto sampler = CreateSampler( entry.GetSamplerName(), wrapX, wrapY, fm );

    return sampler;
}

// **************************
//
TextureSampler *                                NFullscreenEffectHelpers::CreateRenderTargetSampler   ( const std::string & rtSamplerName )
{
    auto wrapX  = TextureWrappingMode::TWM_CLAMP;
    auto wrapY  = TextureWrappingMode::TWM_CLAMP;
    auto fm     = TextureFilteringMode::TFM_POINT;

    auto sampler = CreateSampler( rtSamplerName, wrapX, wrapY, fm );

    return sampler;
}

// **************************
//
ShaderParameters *                              NFullscreenEffectHelpers::CreatePixelShaderParams   ( const NTexture2DEntryVec & textures, const IValuePtrVec & values, const std::vector< std::string > & rtInputSamplerNames )
{
    ShaderParameters * shaderParams = new ShaderParameters();

    // Create parameters
    for( auto val : values )
    {
        auto param = ShaderParamFactory::CreateGenericParameter( val.get() );

        shaderParams->AddParameter( param );
    }

    // Add input textures
    for( auto texEntry : textures )
    {
        auto tex        = texEntry.GetTexture();

        shaderParams->AddTexture( tex );
    }

    // Create placeholders for render targets (textures retrieved from render targets)
    for( auto samplerName : rtInputSamplerNames )
    {
        shaderParams->AddTexture( nullptr );
    }

    return shaderParams;
}

// **************************
//
VertexShader *                                  NFullscreenEffectHelpers::CreateVertexShader        ()
{
    auto params     = new ShaderParameters();
    auto mvpParam   = ShaderParamFactory::CreateMVPParameter();

    params->AddParameter( mvpParam );

    auto source = NFullscreenVSShader::GenerateDefaultVS();

    auto shader = new VertexShader( source, params );

    return shader;
}

// **************************
//
PixelShader *                                   NFullscreenEffectHelpers::CreatePixelShader         ( const std::string & shaderSrc, const NTexture2DEntryVec & textures, const IValuePtrVec & values, const std::vector< std::string > & rtInputSamplerNames )
{
    auto shaderParams = CreatePixelShaderParams( textures, values, rtInputSamplerNames );

    auto shader = new PixelShader( shaderSrc, shaderParams );

    // Add texture samples for input renderTargets
    for( auto samplerName : rtInputSamplerNames )
    {
        auto sampler    = CreateRenderTargetSampler( samplerName );

        shader->AddTextureSampler( sampler );
    }

    // Add texture samples for input textures
    for( auto texEntry : textures )
    {
        auto sampler    = CreateSampler( texEntry );
        
        shader->AddTextureSampler( sampler );
    }

    return shader;
}

// **************************
//
void											NFullscreenEffectHelpers::SetRenderState				( RendererStateInstance * inst, const NFullscreenEffectRendererState & rendererState )
{
    auto as = RenderStateAccessor::AccessAlphaState( inst );
    auto ds = RenderStateAccessor::AccessDepthState( inst );
    auto cs = RenderStateAccessor::AccessCullState( inst );

    as->blendEnabled    = rendererState.GetBlendFlag();
    ds->enabled         = rendererState.GetDepthTestFlag();
    cs->enabled         = rendererState.GetCullFlag();
}

// **************************
//
RenderablePass *                                NFullscreenEffectHelpers::CreateRenderablePass          ( PixelShader * ps, VertexShader * vs, const NFullscreenEffectRendererState & rendererState )
{
    RenderablePass * pass = new RenderablePass( ps, vs, nullptr );

    auto sinst = pass->GetStateInstance();
    
    RendererStatesBuilder::Create( sinst );

	SetRenderState( sinst, rendererState );

    return pass;
}

// **************************
//
RenderableEffectPtr                             NFullscreenEffectHelpers::CreateEffect                  ( const std::string & pixelShaderSrc, const NTexture2DEntryVec & textures, const IValuePtrVec & values, const std::vector< std::string > & rtInputSamplerNames, const NFullscreenEffectRendererState & rendererState )
{
    auto vs = CreateVertexShader();
    auto ps = CreatePixelShader ( pixelShaderSrc, textures, values, rtInputSamplerNames );

    RenderablePass * pass = CreateRenderablePass( ps, vs, rendererState );

    auto effect = std::make_shared< NFullscreenRenderableEffect >( pass );

    return effect;
}


// ***********************************************************************************************************************************************************************************************
// *********************************************************************************** Low level memory layout ***********************************************************************************
// ***********************************************************************************************************************************************************************************************


// **************************
//
float *                                         NFullscreenEffectHelpers::CreateFullscreenQuadVBData ()
{
    unsigned int numUVChannels = 1;

    unsigned int numEntries = ( 2 * numUVChannels + 3 ) * 4;
    unsigned int stride = 2 * numUVChannels + 3;

    float * vertices = new float[ numEntries ];

    /* 
            2 - TopLeft         3 - TopRight

            0 - BottomLeft      1 - BottomRight

    */

    //Raw data
    glm::vec3 positions[] = { BottomLeftPosVertex(), BottomRightPosVertex(), TopLeftPosVertex(), TopRightPosVertex()  };
    glm::vec2 uv[] = { BottomLeftUVData(), BottomRightUVData(), TopLeftUVData(), TopRightUVData() };

    for( unsigned int i = 0; i < 4; ++i )
    {
        // Vertex position offset
        float * vertexpos = &vertices[ i * stride ];

        // Current position
        glm::vec3 & pos = positions[ i ];

        for( unsigned j = 0; j < 3; ++j )
        {
            vertexpos[ j ] = pos[ j ];
        }

        // Vertex uv mapping offset
        float * vertexuv = &vertices[ i * stride + 3 ];

        // Current mapping
        glm::vec2 & uvm = uv[ i ];

        for( unsigned int k = 0; k < numUVChannels; ++k )
        {
            for( unsigned j = 0; j < 2; ++j )
            {
                vertexuv[ k * numUVChannels + j ] = uvm[ j ];
            }
        }
    }

    return vertices;
}



// ***********************************************************************************************************************************************************************************************
// ***************************************************************************** Default mapping and positional data *****************************************************************************
// ***********************************************************************************************************************************************************************************************


// **************************
//
glm::vec3                                       NFullscreenEffectHelpers::TopLeftPosVertex          ()
{
    return glm::vec3( -1.0f, 1.0f, 0.0f );
}

// **************************
//
glm::vec3                                       NFullscreenEffectHelpers::TopRightPosVertex         ()
{
    return glm::vec3( 1.0f, 1.0f, 0.0f );
}

// **************************
//
glm::vec3                                       NFullscreenEffectHelpers::BottomLeftPosVertex        ()
{
    return glm::vec3( -1.0f, -1.0f, 0.0f );
}

// **************************
//
glm::vec3                                       NFullscreenEffectHelpers::BottomRightPosVertex       ()
{
    return glm::vec3( 1.0f, -1.0f, 0.0f );
}

// **************************
//
glm::vec2                                       NFullscreenEffectHelpers::TopLeftUVData              ()
{
    return glm::vec2( 0.f, 1.f );
}

// **************************
//
glm::vec2                                       NFullscreenEffectHelpers::TopRightUVData             ()
{
    return glm::vec2( 1.f, 1.f );
}

// **************************
//
glm::vec2                                       NFullscreenEffectHelpers::BottomLeftUVData           ()
{
    return glm::vec2( 0.f, 0.f );
}

// **************************
//
glm::vec2                                       NFullscreenEffectHelpers::BottomRightUVData          ()
{
    return glm::vec2( 1.f, 0.f );
}

} //nrl
} //bv
