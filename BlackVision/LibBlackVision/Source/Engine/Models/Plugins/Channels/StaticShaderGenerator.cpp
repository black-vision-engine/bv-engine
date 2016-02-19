#include "stdafx.h"

#include "StaticShaderGenerator.h"

#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/Plugins/Interfaces/IPluginDescriptor.h"

#include "System/Path.h"

#include "UseLoggerLibBlackVision.h"

namespace bv { namespace model {

// *****************************
//
std::string         StaticShaderGenerator::GenerateShaderSource( const std::vector< std::string > & uids ) const
{
    std::string filename = GenerateFilename( uids );
    if( Path::Exists( filename ) )
    {
        LOG_MESSAGE( SeverityLevel::debug ) << "Loading pixel shader from: " << filename;
        return ReadShaderContentsFromFile( filename );
    }
    else if( uids.size() == 1 && uids[ 0 ] == "DEFAULT_TRANSFORM" ) // FIXME (?)
        return "";
    else
    {
        LOG_MESSAGE( SeverityLevel::error ) << "File: " << filename << " does not exist. Loading default shader.";
		
		filename = m_shadersDir + "default." + m_shaderExtension;

		if( Path::Exists( filename ) )
			return ReadShaderContentsFromFile( filename );
		else
		{
            LOG_MESSAGE( SeverityLevel::critical ) << "File: " << filename << " does not exist. Loading default shader failed!!!";
			assert( false );
			return "";
		}
    }
}

// *****************************
//
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
    
	//assert( filename.length() > 0 );
	//if( filename.length() == 0 )
	//	filename = "default";

    filename = m_shadersDir + filename;
    filename += "." + m_shaderExtension;

    return filename;
}

// *****************************
//
std::string         StaticShaderGenerator::UID2Abbrv                    ( const std::string & uid ) const
{
    const IPluginDescriptor * pluginDesc = PluginsManager::DefaultInstance().GetDescriptor( uid );
    return pluginDesc->GetPluginTypeAbbrv();
}

// *********************************
//
std::string         ReadShaderContentsFromFile   ( const std::string & fileName )
{
    std::stringstream shaderSource;

    File::Open( fileName ) >> shaderSource;

    return shaderSource.str();
}

} //model
} //bv
