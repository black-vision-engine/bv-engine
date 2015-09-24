#include "ShaderDataSourceCreator.h"

#include <sstream>

#include "System/Path.h"
#include "IO/FileIO.h"

#include "Common/PrototyperCore.h"

#include "Prototypes/Engine/Common/ShaderDataSourceImpl.h"


namespace bv {

// *****************************
//
ShaderDataSourceCreator::ShaderDataSourceCreator()
{
}

// *****************************
//
IShaderDataSourceConstPtr   ShaderDataSourceCreator::VertexShader       ( ShaderDataSourceType sdst )
{
    auto source = VertexShaderSource( sdst );

    if( source != "" )
    {
        return std::make_shared<ShaderDataSourceImpl>( source );
    }

    return nullptr;
}

#if 0
------ Discover test started ------
Test run will use DLL(s) built for framework Framework45 and platform X86. Following DLL(s) will not be part of run: 
BlackVision.exe, EnginePrototyper.exe, TestMipMapBuilder.exe are built for Framework None and Platform X64.
Go to http://go.microsoft.com/fwlink/?LinkID=236877&clcid=0x409 for more details on managing these settings.
#endif

// *****************************
//
IShaderDataSourceConstPtr   ShaderDataSourceCreator::FragmentShader     ( ShaderDataSourceType sdst, const std::string & textureFile, const std::string & textureName )
{
    auto source = FragmentShaderSource( sdst );

    if( source != "" )
    {
        auto shaderDataSource = std::make_shared<ShaderDataSourceImpl>( source );

        if( sdst == ShaderDataSourceType::SDST_SOLID_COLOR )
        {
            shaderDataSource->AddValue( "color", glm::vec4( 1.f, 0.f, 0.f, 1.f ) );
        }
        else if( sdst == ShaderDataSourceType::SDST_ONE_TEXTURE )
        {
            { textureFile; }
            shaderDataSource->AddValue( "alpha", 1.0f );
            shaderDataSource->AddTextureFromFile( textureFile, textureName );
        }

        return shaderDataSource;
    }

    return nullptr;    
}

// *****************************
//
IShaderDataSourceConstPtr   ShaderDataSourceCreator::FragmentShader     ( ShaderDataSourceType sdst, float alfa, const std::string & textureFile, const std::string & textureName )
{
    auto source = FragmentShaderSource( sdst );

    if( source != "" )
    {
        auto shaderDataSource = std::make_shared<ShaderDataSourceImpl>( source );

        if( sdst == ShaderDataSourceType::SDST_SOLID_COLOR )
        {
            shaderDataSource->AddValue( "color", glm::vec4( 1.f, 0.f, 0.f, 1.f ) );
        }
        else if( sdst == ShaderDataSourceType::SDST_ONE_TEXTURE )
        {
            { textureFile; }
            shaderDataSource->AddValue( "alpha", alfa );
            shaderDataSource->AddTextureFromFile( textureFile, textureName );
        }

        return shaderDataSource;
    }

    return nullptr;    
}

// *****************************
//
IShaderDataSourceConstPtr   ShaderDataSourceCreator::GeometryShader     ( ShaderDataSourceType sdst )
{
    { sdst; }
    assert( false );

    return nullptr;
}

// *****************************
//
std::string                 ShaderDataSourceCreator::VertexShaderSource  ( ShaderDataSourceType sdst )
{
    assert( sdst == ShaderDataSourceType::SDST_SOLID_COLOR || sdst == ShaderDataSourceType::SDST_ONE_TEXTURE );

    return ReadSourceFromFile( VertexShaderFile( sdst ) );
}

// *****************************
//
std::string                 ShaderDataSourceCreator::FragmentShaderSource( ShaderDataSourceType sdst )
{
    return ReadSourceFromFile( FragmentShaderFile( sdst ) );
}

// *****************************
//
std::string                 ShaderDataSourceCreator::GeometryShaderSource( ShaderDataSourceType sdst )
{
    { sdst; }

    assert( false );

    return "";
}

// *****************************
//
std::string                 ShaderDataSourceCreator::VertexShaderFile    ( ShaderDataSourceType sdst )
{
    if( sdst == ShaderDataSourceType::SDST_SOLID_COLOR )
    {
        return ShadersRootDir() + "solidcolor.vert";
    }
    else if ( sdst == ShaderDataSourceType::SDST_ONE_TEXTURE )
    {
        return ShadersRootDir() + "onetexture.vert";
    }
    
    assert( false );

    return "";
}

// *****************************
//
std::string                 ShaderDataSourceCreator::FragmentShaderFile  ( ShaderDataSourceType sdst )
{
    if( sdst == ShaderDataSourceType::SDST_SOLID_COLOR )
    {
        return ShadersRootDir() + "solidcolor.frag";
    }
    else if ( sdst == ShaderDataSourceType::SDST_ONE_TEXTURE )
    {
        return ShadersRootDir() + "onetexture.frag";
    }

    assert( false );

    return "";
}

// *****************************
//
std::string                 ShaderDataSourceCreator::GeometryShaderFile  ( ShaderDataSourceType sdst )
{
    { sdst; }

    assert( false );

    return "";
}

// *****************************
//
std::string                 ShaderDataSourceCreator::ShadersRootDir     ()
{
    return config::PROTOTYPES_SHADERS_ROOT + "Engine/";
}

// *****************************
//
std::string                 ShaderDataSourceCreator::ReadSourceFromFile ( const std::string & filename )
{
    if( Path::Exists( filename ) )
    {
        File f = File::Open( filename );

        std::stringstream ss;

        f >> ss;

        return ss.str();
    }

    return "";
}

} //bv
