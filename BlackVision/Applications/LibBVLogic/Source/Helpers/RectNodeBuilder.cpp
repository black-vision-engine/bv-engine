#include "stdafxBVApp.h"

#include "RectNodeBuilder.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "Engine/Models/Plugins/PluginUtils.h"

#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/RendererContext/RendererContext.h"


namespace bv {

// **************************************************************** RECT NODE BUILDER ****************************************************************

// ****************************
//
RectNodeBuilder::RectNodeBuilder     ( model::ITimeEvaluatorPtr timeEvaluator, const std::string & shaderPluginUID, float w, float h, TimeType t )
    : m_node( nullptr )
    , m_shaderPluginUID( shaderPluginUID )
    , m_timeEvaluator( timeEvaluator )
{
    m_node = CreateNewNode();

    SetW( w, t );
    SetH( h, t );

    m_w = w;
    m_h = h;
    m_t = t;
}

// ****************************
//
RectNodeBuilder::~RectNodeBuilder    ()
{
}

// ****************************
//
void    RectNodeBuilder::SetW        ( float w, TimeType t )
{
    assert( m_node );
    bool success = model::SetParameter( GetRectPlugin()->GetParameter( WidthParamName() ), t, w );
    { success; } // FIXME: suppress unused warning
    assert( success );
}

// ****************************
//
void    RectNodeBuilder::SetH        ( float h, TimeType t )
{
    assert( m_node );
    bool success = model::SetParameter( GetRectPlugin()->GetParameter( HeightParamName() ), t, h );
    { success; } // FIXME: suppress unused warning
    assert( success );
}

// ****************************
//
void    RectNodeBuilder::SetPosition ( float x, float y, float z, TimeType t )
{
    assert( m_node );
    bool success = model::SetParameterTranslation( GetTRansformPlugin()->GetParameter( TransformParamName() ), t, glm::vec3( x, y, z ) );
    { success; } // FIXME: suppress unused warning
    assert( success );
}

// ****************************
//
void    RectNodeBuilder::SetRotation ( float x, float y, float z, TimeType t )
{
    assert( m_node );
    bool success = model::SetParameterRotation( GetTRansformPlugin()->GetParameter( TransformParamName() ), t, glm::vec3( x, y, z ) );
    { success; } // FIXME: suppress unused warning
    assert( success );
}

// ****************************
//
void    RectNodeBuilder::SetScale    ( float sx, float sy, float sz, TimeType t )
{
    assert( m_node );
    bool success = model::SetParameterScale( GetTRansformPlugin()->GetParameter( TransformParamName() ), t, glm::vec3( sx, sy, sz ) );
    { success; } // FIXME: suppress unused warning
    assert( success );
}

// ****************************
//
model::BasicNodePtr RectNodeBuilder::CreateNode  ( const std::string & name, bool resetToBuilderDefaults )
{
    auto node = m_node;
    m_node = CreateNewNode();

    if( resetToBuilderDefaults )
    {
        SetDefaults();
    }

    node->SetName( name );

    return node;
}

// ****************************
//
void        RectNodeBuilder::SetDefaults        ()
{
    SetW( m_w, m_t );
    SetH( m_h, m_t );
}

// ****************************
//
std::string RectNodeBuilder::TransformPluginName () const
{
    return "transform";
}

// ****************************
//
std::string RectNodeBuilder::RectPluginName      () const
{
    return "rectangle";
}

// ****************************
//
std::string RectNodeBuilder::WidthParamName      () const
{
    return "width";
}

// ****************************
//
std::string RectNodeBuilder::HeightParamName     () const
{
    return "height";
}

// ****************************
//
std::string RectNodeBuilder::TransformParamName  () const
{
    return "simple_transform";
}

// ****************************
//
model::IPluginPtr   RectNodeBuilder::GetTRansformPlugin  ()
{
    return m_node->GetPlugin( TransformPluginName() );
}

// ****************************
//
model::IPluginPtr  RectNodeBuilder::GetRectPlugin       ()
{
    return m_node->GetPlugin( RectPluginName() );
}

// ****************************
//
model::IPluginPtr  RectNodeBuilder::GetShaderPlugin     ()
{
    return m_node->GetPlugin( ShaderPluginName() );
}

// ****************************
//
model::BasicNodePtr RectNodeBuilder::CreateNewNode   () const
{
    std::vector< std::string > uids;

    uids.push_back( "DEFAULT_TRANSFORM" );
    uids.push_back( "DEFAULT_RECTANGLE" );
    uids.push_back( m_shaderPluginUID );

    auto node = model::BasicNode::Create( "" );
    assert( node );
    
    auto success = node->AddPlugins( uids, m_timeEvaluator );
    { success; } // FIXME: suppress unused warning
    assert( success );

    return node;
}


// **************************************************************** SOLID RECT NODE BUILDER ****************************************************************

// ****************************
//
SolidRectNodeBuilder::SolidRectNodeBuilder    ( model::ITimeEvaluatorPtr timeEvaluator, const glm::vec4 & color, float w, float h, TimeType t )
    : RectNodeBuilder( timeEvaluator, "DEFAULT_COLOR", w, h, t )
    , m_color( color )
{
    SetColor( color.r, color.g, color.b, color.a, t );
}

// ****************************
//
SolidRectNodeBuilder::~SolidRectNodeBuilder   ()
{
}

// ****************************
//
void    SolidRectNodeBuilder::SetColor        ( float r, float g, float b, float a, TimeType t )
{
    assert( m_node );
    bool success = model::SetParameter( GetShaderPlugin()->GetParameter( ColorParamName() ), t, glm::vec4( r, g, b, a ) );
    { success; } // FIXME: suppress unused warning
    assert( success );    

    if( a < 1.0f )
    {
        GetShaderPlugin()->GetRendererContext()->alphaCtx->blendEnabled = true;
    }
}

// ****************************
//
void    SolidRectNodeBuilder::SetDefaults   ()
{
    RectNodeBuilder::SetDefaults();

    SetColor( m_color.r, m_color.g, m_color.b, m_color.a );
}

// ****************************
//
std::string         SolidRectNodeBuilder::ShaderPluginName() const
{
    return "solid color";
}

// ****************************
//
std::string         SolidRectNodeBuilder::ColorParamName  () const
{
    return "color";
}


// **************************************************************** TEXTURED RECT NODE BUILDER ****************************************************************

// ****************************
//
TexturedRectNodeBuilder::TexturedRectNodeBuilder     ( model::ITimeEvaluatorPtr timeEvaluator, const std::string & textureFile, bool hasAlpha, float w, float h, TimeType t )
    : RectNodeBuilder( timeEvaluator,  "DEFAULT_TEXTURE", w, h, t )
    , m_textureFile( textureFile )
    , m_hasAlpha( hasAlpha )
{
    SetTextureFile( textureFile, hasAlpha );
}

// ****************************
//
TexturedRectNodeBuilder::~TexturedRectNodeBuilder    ()
{
}

// ****************************
//
void    TexturedRectNodeBuilder::SetTextureFile      ( const std::string & textureFile, bool hasAlpha )
{
    assert( m_node );
    bool success = model::LoadTexture( GetShaderPlugin(), textureFile );
    { success; } // FIXME: suppress unused warning
    assert( success );

    if( hasAlpha )
    {
        GetShaderPlugin()->GetRendererContext()->alphaCtx->blendEnabled = true;
    }
    else
    {
        GetShaderPlugin()->GetRendererContext()->alphaCtx->blendEnabled = false;
    }
}

// ****************************
//
std::string TexturedRectNodeBuilder::ShaderPluginName() const
{
    return "texture";
}

// ****************************
//
model::BasicNodePtr  TexturedRectNodeBuilder::CreateNode      ( const std::string & name, bool resetToBuilderDefaults )
{
    auto n = RectNodeBuilder::CreateNode( name, resetToBuilderDefaults );

    SetTextureFile( m_textureFile, m_hasAlpha );

    return n;
}

} //bv
