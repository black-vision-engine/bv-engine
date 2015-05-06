#include "StaticShaderGenerator.h"

#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/Plugins/Interfaces/IPluginDescriptor.h"

namespace bv { namespace model {

std::string         StaticShaderGenerator::GenerateShaderSource( const std::vector< std::string > & uids ) const
{
    std::string filename = GenerateFilename( uids );
    if( File::Exists( filename ) )
    {
        std::cout << "Loading pixel shader from: " << filename << std::endl;
        return ReadShaderContentsFromFile( filename );
    }
    else
    {
        std::cout << "File: " << filename << " does not exist. Loading pixel shader failed." << std::endl;
        assert( false );
        return "";
    }
}

std::string         StaticShaderGenerator::GenerateFilename( const std::vector< std::string > & uids ) const
{
    std::string filename = "";
    for( std::string uid : uids )
    {
        std::string abbrv = UID2Abbrv( uid );
        if( abbrv != "" )
		{
            if( filename.length() == 0 )
                filename = abbrv;
            else
                filename += "_" + abbrv;
		}
	}
    assert( filename.length() > 0 );
    filename = m_shadersDir + filename;
    filename += "." + m_shaderExtension;

    return filename;
}

std::string         StaticShaderGenerator::UID2Abbrv( const std::string uid ) const
{
    const IPluginDescriptor * pluginDesc = PluginsManager::DefaultInstance().GetDescriptor( uid );
    return pluginDesc->GetPluginTypeAbbrv();
}

// *********************************
//
const std::string                            ReadShaderContentsFromFile             ( const std::string & fileName )
{
    std::stringstream shaderSource;

    File::Open( fileName ) >> shaderSource;

    return shaderSource.str();
}


} //model
} //bv
