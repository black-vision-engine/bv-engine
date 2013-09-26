#include "SimpleTexturePlugin.h"
#include "System/Print.h"
#include "Engine\Models\Plugins\Channels\GeometryChannelImpl.h"

#include "Engine/Models/Parameter.h"

namespace bv {

// ***************************** DESCRIPTOR ********************************** 
//PLUGIN NAME
const std::string SimpleTexturePixelPluginPD::pluginName( "SimpleTexturePlugin" );

//PLUGIN PARAMETERS
const std::string SimpleTexturePixelPluginPD::alphaParamName( "alpha" );
const std::string SimpleTexturePixelPluginPD::txMatrix0ParamName( "txMat0" );
const std::string SimpleTexturePixelPluginPD::txMatrix1ParamName( "txMat1" );


// *************************************
//
SimpleTexturePixelPluginPD::SimpleTexturePixelPluginPD()
    : BaseParametersDescriptor( pluginName )
{
    m_params[ alphaParamName ] = ParamType::PT_FLOAT1;
    m_params[ txMatrix0ParamName ] = ParamType::PT_MAT4;
    m_params[ txMatrix1ParamName ] = ParamType::PT_MAT4;
}

// ***************************** PLUGIN ********************************** 

// *************************************
//
SimpleTexturePixelPlugin::SimpleTexturePixelPlugin                    ( const std::string & textureFileName, const std::string & textureFileName1, const FloatInterpolator & alpha, const TransformF & tex0Transform, const TransformF & tex1Transform )
    : m_textureFileName( textureFileName )
    , m_textureFileName1( textureFileName1 )
{

    //FIXME: pass params in constructor
    m_alphaParam = new ParamFloat( ParamDesc::alphaParamName, alpha );
    m_alphaValue = new ValueFloat( ParamDesc::alphaParamName );

    m_tex0TransformParam = new ParamTransform( ParamDesc::txMatrix0ParamName, tex0Transform );
    m_tex0TransformValue = new ValueMat4( ParamDesc::txMatrix0ParamName );

    m_tex1TransformParam = new ParamTransform( ParamDesc::txMatrix1ParamName, tex1Transform );
    m_tex1TransformValue = new ValueMat4( ParamDesc::txMatrix1ParamName);

    //FIXME: GetShaderResource should be used instead
    PluginParamDesc().ValidateParameters( GetShaderFile() );

    RegisterValue( m_alphaValue );
    RegisterValue( m_tex0TransformValue );
    RegisterValue( m_tex1TransformValue );

    m_geomChannel = model::GeometryChannelStaticRectTextured::Create();
    m_textures.push_back( textureFileName );
    m_textures.push_back( textureFileName1 );
}

// *************************************
//
SimpleTexturePixelPlugin::~SimpleTexturePixelPlugin   ()
{
    delete m_alphaParam;
    delete m_alphaValue;

    delete m_tex0TransformParam;
    delete m_tex0TransformValue;

    delete m_tex1TransformParam;
    delete m_tex1TransformValue;
}

// *************************************
//
std::string SimpleTexturePixelPlugin::GetShaderFile              () const
{
    // TODO; do not bind SmpleTexturePlugin with exact shader - at some point it will be provided by GLSL shader service or whatever else resource manager
    return "../dep/media/shaders/simpletexture.frag";
}

// *************************************
//
void                SimpleTexturePixelPlugin::Update              ( float t )
{
    BasePlugin::Update( t );

    m_alphaValue->SetValue( m_alphaParam->Evaluate( t ) );
    m_tex0TransformValue->SetValue( m_tex0TransformParam->Evaluate( t ) );
    m_tex1TransformValue->SetValue( m_tex1TransformParam->Evaluate( t ) );
}

// *************************************
//
void                SimpleTexturePixelPlugin::Print               ( std::ostream & out, int tabs ) const
{
    out << GetName() << debug::EndLine( tabs );
    out << "Texture: " << m_textureFileName << debug::EndLine( tabs );
}


// ***************************** DESCRIPTOR ********************************** 
//PLUGIN NAME
const std::string SimpleTextureVertexPluginPD::pluginName( "SimpleTextureVertex" );

// *************************************
//
SimpleTextureVertexPluginPD::SimpleTextureVertexPluginPD()
    : BaseParametersDescriptor( pluginName )
{
}

// ***************************** PLUGIN ********************************** 

// *************************************
//
SimpleTextureVertexPlugin::SimpleTextureVertexPlugin            ()
{
}

// *************************************
//
std::string SimpleTextureVertexPlugin::GetShaderFile              () const
{
    // TODO; do not bind SmpleTexturePlugin with exact shader - at some point it will be provided by GLSL shader service or whatever else resource manager
    return "../dep/media/shaders/simpletexture.vert";
}

// *************************************
//
void                SimpleTextureVertexPlugin::Update              ( float t )
{
    //FIXME: implement - whatever is required here
}

// *************************************
//
void                SimpleTextureVertexPlugin::Print               ( std::ostream & out, int tabs ) const
{
    out << GetName() << debug::EndLine( tabs );
}

}
