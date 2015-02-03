#include "ShaderChannel.h"

#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IVertexShaderChannel.h"


namespace bv { namespace model {

// *********************************
//
const std::string                            ReadShaderContentsFromFile             ( const std::string & fileName )
{
    std::stringstream shaderSource;

    File::Open( fileName ) >> shaderSource;

    return shaderSource.str();
}

// *********************************
//
template< typename ShaderChannelIface >
ShaderChannel< ShaderChannelIface >::ShaderChannel  ( const std::string & shaderSource, bv::IValueSetConstPtr valueSet )
    : m_values( valueSet )
    , m_shaderSource( shaderSource )
{
    assert( valueSet != nullptr );
}


// *********************************
//
template< typename ShaderChannelIface >
const std::string &                         ShaderChannel< ShaderChannelIface >::GetShaderSource    () const
{
    return m_shaderSource; 
}

// *********************************
//
template< typename ShaderChannelIface >
bool                                        ShaderChannel< ShaderChannelIface >::IsReadOnly         () const
{
    return true; 
}

// *********************************
//
template< typename ShaderChannelIface >
void                                        ShaderChannel< ShaderChannelIface >::PostUpdate         ()
{
    //Default implementation is empty
}

// *********************************
//
template< typename ShaderChannelIface >
const std::vector< bv::IValueConstPtr > &   ShaderChannel< ShaderChannelIface >::GetValues          () const
{
    return m_values->GetValues(); 
}

// *********************************
//
template< typename ShaderChannelIface >
bv::IValueConstPtr                          ShaderChannel< ShaderChannelIface >::GetValue           ( const std::string & name ) const
{
    for( auto value : m_values->GetValues() )
    {
        if( value->GetName() == name )
        {
            return value;
        }
    }

    return nullptr;
}

// *********************************
//
template< typename ShaderChannelIface >
bv::ITexturesDataConstPtr                   ShaderChannel< ShaderChannelIface >::GetTexturesData    () const
{
    return nullptr;
}

// *********************************
//
template< typename ShaderChannelIface >
const std::string                           ShaderChannel< ShaderChannelIface >::ReadShaderFromFile ( const std::string & fileName )
{
    return ReadShaderContentsFromFile( fileName );
}


} // model
} // bv

//explicit instantiation - this way class' implementation can be stored in cpp file (like here)
template class bv::model::ShaderChannel<bv::model::IPixelShaderChannel>;
template class bv::model::ShaderChannel<bv::model::IVertexShaderChannel>;
