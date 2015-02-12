#include "ShaderDataSourceCreator.h"

#include <sstream>

#include "System/FileIO.h"

#include "Common/PrototyperCore.h"

#include "Prototypes/Engine/Common/ShaderDataSourceImpl.h"


namespace bv {

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
IShaderDataSourceConstPtr   ShaderDataSourceCreator::FragmentShader     ( ShaderDataSourceType sdst )
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
            // TODO: implement
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
    assert( sdst == ShaderDataSourceType::SDST_SOLID_COLOR );

    return ReadSourceFromFile( VertexShaderFile( sdst ) );
}

// *****************************
//
std::string                 ShaderDataSourceCreator::FragmentShaderSource( ShaderDataSourceType sdst )
{
    assert( sdst == ShaderDataSourceType::SDST_SOLID_COLOR );

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
    { sdst; }
    assert( sdst == ShaderDataSourceType::SDST_SOLID_COLOR );

    return ShadersRootDir() + "default.vert";
}

// *****************************
//
std::string                 ShaderDataSourceCreator::FragmentShaderFile  ( ShaderDataSourceType sdst )
{
    { sdst; }

    assert( sdst == ShaderDataSourceType::SDST_SOLID_COLOR );

    return ShadersRootDir() + "solidcolor.frag";
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
    if( File::Exists( filename ) )
    {
        File f = File::Open( filename );

        std::stringstream ss;

        f >> ss;

        return ss.str();
    }

    return "";
}

} //bv
