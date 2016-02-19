#include "stdafx.h"

#include "ShaderSourceProvider.h"

#include <cassert>
#include <sstream>

#include "IO/FileIO.h"


namespace bv {

// *************************
//
ShaderSourceProvider::ShaderSourceProvider    ( const std::string & dir )
    : m_dir( dir )
{
}

// *************************
//
ShaderSourceProvider::~ShaderSourceProvider   ()
{
}

// *************************
//
const ShaderSourceProvider *  ShaderSourceProvider::GetProvider ( ShaderCategory category )
{
    static ShaderSourceProvider fullscreen( "Assets/Shaders/FullscreenEffects/" );
    static ShaderSourceProvider nodeeffect( "Assets/Shaders/NodeEffectsLogic/" );
    static ShaderSourceProvider custom( "Assets/Shaders/Effects/" );

    switch( category )
    {
        case ShaderCategory::SC_FULLSCREENEFFECT:
            return &fullscreen;            
        case ShaderCategory::SC_NODEEFFECTLOGIC:
            return &nodeeffect;
        case ShaderCategory::SC_CUSTOM:
            return &custom;
        default:
            assert( false );
    }

    return nullptr;
}

// *************************
//
std::string             ShaderSourceProvider::ReadShader        ( const std::string & fn ) const
{
    std::string inputFile = m_dir + fn;

    std::stringstream shaderSource;

    File::Open( inputFile ) >> shaderSource;

    if( shaderSource.str().size() == 0 )
    {
        assert( false );
    }

    return shaderSource.str();
}

} //bv
