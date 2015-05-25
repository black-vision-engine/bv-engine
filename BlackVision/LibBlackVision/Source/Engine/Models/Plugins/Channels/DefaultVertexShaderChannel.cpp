#include "DefaultVertexShaderChannel.h"

//FIXME: REMOVE after reimplementing access to values from shader channel implementation
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"

namespace bv { namespace model {

// ******************************
//
DefaultVertexShaderChannel::DefaultVertexShaderChannel  ( const std::string & shaderSource, IValueSetConstPtr values )
    : ShaderChannel( shaderSource, values )
    , m_texturesData( nullptr )
{
    m_texturesData = std::make_shared< DefaultTexturesData >();
}

// ******************************
//
DefaultVertexShaderChannel::~DefaultVertexShaderChannel                     ()
{
}

// ******************************
//
ITexturesDataConstPtr       DefaultVertexShaderChannel::GetTexturesData     () const
{
    return m_texturesData;
}

// ******************************
//
DefaultTexturesDataPtr       DefaultVertexShaderChannel::GetTexturesDataImpl ()
{
    return m_texturesData;
}

// ******************************
//
DefaultVertexShaderChannelPtr DefaultVertexShaderChannel::Create             ( const std::string & shaderFile, IValueSetConstPtr values )
{
    auto shaderSource = ReadShaderFromFile( shaderFile );

    if( shaderSource != "" )
    {
        assert( values );
        
        return std::make_shared< DefaultVertexShaderChannel >( shaderSource, values );
    }

    return nullptr;
}

namespace {

// *********************************
//
std::string shaderSource   = " #version 430 core \n \
								\
								layout (location = 0) in vec3 vertexPosition; \n \
								\
								uniform mat4 MVP; \n \
								\
								void main() \n \
								{ \n \
									gl_Position =  MVP * vec4(vertexPosition, 1.0); \n \
								} \n \
								\
                            ";

}

// ******************************
//
DefaultVertexShaderChannelPtr DefaultVertexShaderChannel::Create ()
{
    //FIXME: remove this DefaultParamValModel construction from here (implement decent ShaderChannel in case of nullptr input IValueSet - simply return empty vector there)
    return std::make_shared< DefaultVertexShaderChannel >( shaderSource, std::make_shared< DefaultParamValModel >() );
}

} //model
} //bv
