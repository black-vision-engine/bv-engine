#include "SimpleTexturePlugin.h"
#include "System/Print.h"
#include "Engine\Models\Plugins\Channels\GeometryChannelImpl.h"

namespace bv {

// ***************************** DESCRIPTOR ********************************** 
//PLUGIN NAME
const std::string SimpleTexturePixelPluginPD::pluginName( "SimpleTexturePixel" );

// *************************************
//
SimpleTexturePixelPluginPD::SimpleTexturePixelPluginPD()
    : BaseParametersDescriptor( pluginName )
{
}

// ***************************** PLUGIN ********************************** 

// *************************************
//
SimpleTexturePixelPlugin::SimpleTexturePixelPlugin                    ( const std::string & textureFileName )
    : m_textureFileName( textureFileName )
{
    m_geomChannel = model::GeometryChannelStaticRectTextured::Create();
    m_textures.push_back( textureFileName );
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
    //FIXME: implement - whatever is required here
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
