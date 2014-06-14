#include "RectNodeBuilder.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "Engine/Models/Plugins/PluginUtils.cpp"


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
    delete m_node;
}

// ****************************
//
void    RectNodeBuilder::SetW        ( float w, TimeType t )
{
    assert( m_node );
    bool success = model::SetParameter( GetRectPlugin()->GetParameter( WidthParamName() ), t, w );
    assert( success );
}

// ****************************
//
void    RectNodeBuilder::SetH        ( float h, TimeType t )
{
    assert( m_node );
    bool success = model::SetParameter( GetRectPlugin()->GetParameter( HeightParamName() ), t, h );
    assert( success );
}

// ****************************
//
void    RectNodeBuilder::SetPosition ( float x, float y, float z, TimeType t )
{
    assert( m_node );
    bool success = model::SetParameterTranslation( GetTRansformPlugin()->GetParameter( TransformParamName() ), 0, t, glm::vec3( x, y, z ) );
    assert( success );
}

// ****************************
//
void    RectNodeBuilder::SetRotation ( float x, float y, float z, float angle, TimeType t )
{
    assert( m_node );
    bool success = model::SetParameterRotation( GetTRansformPlugin()->GetParameter( TransformParamName() ), 0, t, glm::vec3( x, y, z ), angle );
    assert( success );
}

// ****************************
//
void    RectNodeBuilder::SetScale    ( float sx, float sy, float sz, TimeType t )
{
    assert( m_node );
    bool success = model::SetParameterScale( GetTRansformPlugin()->GetParameter( TransformParamName() ), 0, t, glm::vec3( sx, sy, sz ) );
    assert( success );
}

// ****************************
//
model::BasicNode * RectNodeBuilder::CreateNode  ( const std::string & name, bool resetToBuilderDefaults )
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
model::IPlugin *  RectNodeBuilder::GetTRansformPlugin  ()
{
    return m_node->GetPlugin( TransformPluginName() );
}

// ****************************
//
model::IPlugin *   RectNodeBuilder::GetRectPlugin       ()
{
    return m_node->GetPlugin( RectPluginName() );
}

// ****************************
//
model::IPlugin *   RectNodeBuilder::GetShaderPlugin     ()
{
    return m_node->GetPlugin( ShaderPluginName() );
}

// ****************************
//
model::BasicNode * RectNodeBuilder::CreateNewNode   () const
{
    std::vector< std::string > uids;

    uids.push_back( "DEFAULT_TRANSFORM" );
    uids.push_back( "DEFAULT_RECTANGLE" );
    uids.push_back( m_shaderPluginUID );

    auto node = new model::BasicNode( "", m_timeEvaluator );
    assert( node );
    
    auto success = node->AddPlugins( uids, m_timeEvaluator );
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
    assert( success );    
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
TexturedRectNodeBuilder::TexturedRectNodeBuilder     ( model::ITimeEvaluatorPtr timeEvaluator, const std::string & textureFile, float w, float h, TimeType t )
    : RectNodeBuilder( timeEvaluator,  "DEFAULT_TEXTURE", w, h, t )
{
    SetTextureFile( textureFile );
}

// ****************************
//
TexturedRectNodeBuilder::~TexturedRectNodeBuilder    ()
{
}

// ****************************
//
void    TexturedRectNodeBuilder::SetTextureFile      ( const std::string & textureFile )
{
    assert( m_node );
    bool success = model::LoadTexture( GetShaderPlugin(), textureFile );
    assert( success );
}

// ****************************
//
std::string TexturedRectNodeBuilder::ShaderPluginName() const
{
    return "texture";
}

} //bv
