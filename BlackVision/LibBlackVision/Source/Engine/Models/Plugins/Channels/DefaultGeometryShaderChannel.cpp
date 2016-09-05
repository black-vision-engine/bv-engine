#include "stdafx.h"
#include "DefaultGeometryShaderChannel.h"

//FIXME: REMOVE after reimplementing access to values from shader channel implementation
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

// ******************************
//
DefaultGeometryShaderChannel::DefaultGeometryShaderChannel  ( const std::string & shaderSource, IValueSetConstPtr values )
    : ShaderChannel( shaderSource, values )
{}

// ******************************
//
DefaultGeometryShaderChannel::~DefaultGeometryShaderChannel ()
{}


// ******************************
//
DefaultGeometryShaderChannelPtr DefaultGeometryShaderChannel::Create              ( const std::string & shaderFile, IValueSetConstPtr values )
{
    auto shaderSource = ReadShaderFromFile( shaderFile );


    if( shaderSource != "" )
    {
        assert( values );
        
		return std::make_shared< DefaultGeometryShaderChannel >( shaderSource, values );
    }

    return nullptr;
}

// ******************************
//
DefaultGeometryShaderChannelPtr DefaultGeometryShaderChannel::Create              ( IValueSetConstPtr values )
{
    return std::make_shared< DefaultGeometryShaderChannel >( "", values );
}

// ******************************
//
DefaultGeometryShaderChannelPtr DefaultGeometryShaderChannel::Create ()
{
    //FIXME: remove this DefaultParamValModel construction from here (implement decent ShaderChannel in case of nullptr input IValueSet - simply return empty vector there)
    return std::make_shared< DefaultGeometryShaderChannel >( "", std::make_shared< DefaultParamValModel >() );
}

} //model
} //bv

