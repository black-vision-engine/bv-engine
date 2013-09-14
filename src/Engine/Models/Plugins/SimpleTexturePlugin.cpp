#include "SimpleTexturePlugin.h"
#include "System/Print.h"

namespace bv {

// ***************************** DESCRIPTOR ********************************** 
//PLUGIN NAME
const std::string TexturePluginParametersDescriptor::pluginName( "SimpleTexture" );

// *************************************
//
TexturePluginParametersDescriptor::TexturePluginParametersDescriptor()
    : BaseParametersDescriptor( pluginName )
{
}

// ***************************** PLUGIN ********************************** 

// *************************************
//
SimpleTexturePlugin::SimpleTexturePlugin                    ( const std::string & textureFileName )
    : m_textureFileName( textureFileName )
{
}

// *************************************
//
std::string SimpleTexturePlugin::GetShaderFile              () const
{
    //FIXME: implement that shader for the sake of simplicity and testability
    return "nicht compile kurwile";
}

// *************************************
//
void                SimpleTexturePlugin::Update              ( float t )
{
    //FIXME: implement - whatever is required here
}

// *************************************
//
void                SimpleTexturePlugin::Print               ( std::ostream & out, int tabs ) const
{
    out << GetName() << debug::EndLine( tabs );
    out << "Texture: " << m_textureFileName << debug::EndLine(tabs);
}

}
