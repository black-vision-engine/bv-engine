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

// *****************************
//
IShaderDataSourceConstPtr   ShaderDataSourceCreator::FragmentShader     ( ShaderDataSourceType sdst )
{
    auto source = FragmentShaderSource( sdst );

    if( source != "" )
    {
        return std::make_shared<ShaderDataSourceImpl>( source );
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

    return ShadersRootDir() + "default.frag";
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
