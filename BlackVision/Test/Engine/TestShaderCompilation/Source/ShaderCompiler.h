#pragma once

#include "Framework/FrameworkTest.h"


#include "Engine/Graphics/Renderers/OGLRenderer/PdrShader.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrGLSLProgram.h"

#include "Engine/Graphics/Shaders/PixelShader.h"
#include "Engine/Graphics/Shaders/VertexShader.h"

#include "IO/FileIO.h"
#include "System/Path.h"

#include <string>


using namespace bv;


typedef std::pair< std::string, bool > CompilationResult;


// ***********************
//
inline Path                 FindMatchingPS      ( const Path & vsPath )
{
    auto fileName = File::GetFileName( vsPath.Str(), false );
    return vsPath.ParentPath() / Path( fileName + ".frag" );
}

// ***********************
//
inline Path                 FindMatchingGS      ( const Path & vsPath )
{
    auto fileName = File::GetFileName( vsPath.Str(), false );
    return vsPath.ParentPath() / Path( fileName + ".geom" );
}


// ***********************
//
inline CompilationResult    CompileAndLink      ( const std::string & vertexShader, const std::string & pixelShader, const std::string & geomShader = "" )
{
    VertexShader * vert = new VertexShader( vertexShader, nullptr );
    PixelShader * pix = new PixelShader( pixelShader, nullptr );

    if( geomShader.empty() )
    {
        PdrGLSLProgram program( *pix, *vert, nullptr );
        return CompilationResult( program.Log(), program.IsCompiled() );
    }
    else
    {
        GeometryShader * geom = new GeometryShader( geomShader, nullptr );

        PdrGLSLProgram program( *pix, *vert, geom );
        return CompilationResult( program.Log(), program.IsCompiled() );
    }
}


// ***********************
//
inline void                 TestCompilation     ( const std::string & vsPath, const std::string & psPath )
{
    auto vsSource = File::Read( vsPath );
    auto psSource = File::Read( psPath );

    auto result = CompileAndLink( vsSource, psSource );

    if( !result.second )
    {
        auto message = "Compiling shaders: [" + vsPath + ", " + psPath + "] " + result.first;

        // This is a little bit hackish. We know already that shader didn't compile.
        // This chack will always fail bu thanks to this we can print error message and shader files.
        ADD_FAILURE() << message;
    }
}

// ***********************
//
inline void                 TestCompilation     ( const std::string & vsPath, const std::string & psPath, const std::string & gsPath )
{
    auto vsSource = File::Read( vsPath );
    auto psSource = File::Read( psPath );
    auto gsSource = File::Read( gsPath );

    auto result = CompileAndLink( vsSource, psSource, gsSource );

    if( !result.second )
    {
        auto message = "Compiling shaders: [" + vsPath + ", " + gsPath + ", " + psPath + "] " + result.first;

        // This is a little bit hackish. We know already that shader didn't compile.
        // This chack will always fail bu thanks to this we can print error message and shader files.
        ADD_FAILURE() << message;
    }
}


