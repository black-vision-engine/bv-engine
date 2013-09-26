#include "SimpleTexturePlugin.h"
#include "System/Print.h"
#include "Engine/Models/Plugins/Channels/Geometry/GeometryChannelImpl.h"
#include "Engine/Models/Resources/TextureLoader.h"

#include "Engine/Models/Parameter.h"

namespace bv { namespace model {

// ***************************** DESCRIPTOR ********************************** 
//PLUGIN NAME
const std::string SimpleTexturePluginPD::pluginName( "SimpleTexturePlugin" );

//PLUGIN PARAMETERS
const std::string SimpleTexturePluginPD::alphaParamName( "alpha" );
const std::string SimpleTexturePluginPD::txMatrix0ParamName( "txMat0" );
const std::string SimpleTexturePluginPD::txMatrix1ParamName( "txMat1" );


// *************************************
//
SimpleTexturePluginPD::SimpleTexturePluginPD()
    : BaseParametersDescriptor( pluginName )
{
    m_params[ alphaParamName ] = ParamType::PT_FLOAT1;
    m_params[ txMatrix0ParamName ] = ParamType::PT_MAT4;
    m_params[ txMatrix1ParamName ] = ParamType::PT_MAT4;
}

// ***************************** PLUGIN ********************************** 

// *************************************
//
SimpleTexturePlugin::SimpleTexturePlugin                    ( const std::string & textureFileName, const std::string & textureFileName1 )
    : m_textureFileName( textureFileName )
    , m_textureFileName1( textureFileName1 )
{

    //FIXME: pass params in constructor
   // m_alphaParam = new ParamFloat( ParamDesc::alphaParamName, alpha );
    //m_alphaValue = new ValueFloat( ParamDesc::alphaParamName );

    //m_tex0TransformParam = new ParamTransform( ParamDesc::txMatrix0ParamName, tex0Transform );
    //m_tex0TransformValue = new ValueMat4( ParamDesc::txMatrix0ParamName );

    //m_tex1TransformParam = new ParamTransform( ParamDesc::txMatrix1ParamName, tex1Transform );
    //m_tex1TransformValue = new ValueMat4( ParamDesc::txMatrix1ParamName);

    ////FIXME: GetShaderResource should be used instead
    ////PluginParamDesc().ValidateParameters( m_GetShaderFile() );

    //RegisterValue( m_alphaValue );
    //RegisterValue( m_tex0TransformValue );
    //RegisterValue( m_tex1TransformValue );

    m_geomChannel = model::GeometryChannelStaticRectTextured::Create();

    m_textures.push_back( LoadTexture( "Tex0", textureFileName ) );
    m_textures.push_back( LoadTexture( "Tex1", textureFileName1 ) );
}

// *************************************
//
ResourceHandle* SimpleTexturePlugin::LoadTexture( const std::string& name, const std::string& path ) const
{
    TextureLoader texLoader( false );

    Resource texture( name, path );

    return texLoader.LoadResource( &texture );
}

// *************************************
//
SimpleTexturePlugin::~SimpleTexturePlugin   ()
{
    //delete m_alphaParam;
    //delete m_alphaValue;

    //delete m_tex0TransformParam;
    //delete m_tex0TransformValue;

    //delete m_tex1TransformParam;
    //delete m_tex1TransformValue;
}

// *************************************
//
void                SimpleTexturePlugin::Update              ( float t )
{
    BasePlugin::Update( t );

//    m_alphaValue->SetValue( m_alphaParam->Evaluate( t ) );
//    m_tex0TransformValue->SetValue( m_tex0TransformParam->Evaluate( t ) );
//    m_tex1TransformValue->SetValue( m_tex1TransformParam->Evaluate( t ) );
}

// *************************************
//
void                SimpleTexturePlugin::Print               ( std::ostream & out, int tabs ) const
{
    out << GetName() << debug::EndLine( tabs );
    out << "Texture: " << m_textureFileName << debug::EndLine( tabs );
}


//// ***************************** DESCRIPTOR ********************************** 
////PLUGIN NAME
//const std::string SimpleTextureVertexPluginPD::pluginName( "SimpleTextureVertex" );
//
//// *************************************
////
//SimpleTextureVertexPluginPD::SimpleTextureVertexPluginPD()
//    : BaseParametersDescriptor( pluginName )
//{
//}
//
//// ***************************** PLUGIN ********************************** 
//
//// *************************************
////
//SimpleTextureVertexPlugin::SimpleTextureVertexPlugin            ()
//{
//}
//
//// *************************************
////
//std::string SimpleTextureVertexPlugin::GetShaderFile              () const
//{
//    // TODO; do not bind SmpleTexturePlugin with exact shader - at some point it will be provided by GLSL shader service or whatever else resource manager
//    return "../dep/media/shaders/simpletexture.vert";
//}
//
//// *************************************
////
//void                SimpleTextureVertexPlugin::Update              ( float t )
//{
//    //FIXME: implement - whatever is required here
//}
//
//// *************************************
////
//void                SimpleTextureVertexPlugin::Print               ( std::ostream & out, int tabs ) const
//{
//    out << GetName() << debug::EndLine( tabs );
//}

} // model
} // bv
