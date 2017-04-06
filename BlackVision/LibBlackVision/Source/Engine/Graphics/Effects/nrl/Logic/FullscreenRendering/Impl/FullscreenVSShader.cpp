#include "stdafx.h"

#include "FullscreenVSShader.h"


namespace bv { namespace nrl {

// ********************************
//
std::string     FullscreenVSShader::GenerateDefaultVS   ()
{
    std::string posAttr   = DefaultPositionAttrName();
    std::string uvInAttr  = DefaultInUVAttrName();
    std::string uvOutAttr = DefaultOutUVAttrName();

    // Old version which generates uv mappings for all textures
    //std::string decl = GenerateHeader() + GeneratePositionDecl( posAttr ) + GenerateUVDecl( numUVChannels, uvInAttr ) + GenerateMVPDecl() + GenerateUVOutVarsDecl( numUVChannels, uvOutAttr );
    //std::string impl = GenerateMainStart() + GenerateOutGLPositionCode( posAttr ) + GenerateOutUVMappingCode( numUVChannels, uvOutAttr, uvInAttr ) + GenerateMainEnd();

    // Old version which generates one uv mapping for all textures - by default these are identical, so there is no need to generate all of them
    unsigned int numUVChannels = 1;
    std::string decl = GenerateHeader() + GeneratePositionDecl( posAttr ) + GenerateUVDecl( numUVChannels, uvInAttr ) + GenerateMVPDecl() + GenerateUVOutVarsDecl( numUVChannels, uvOutAttr );
    std::string impl = GenerateMainStart() + GenerateOutGLPositionCode( posAttr ) + GenerateOutUVMappingCode( numUVChannels, uvOutAttr, uvInAttr ) + GenerateMainEnd();

    std::string res = decl + impl;

    return res;
}

// ********************************
//
std::string     FullscreenVSShader::DefaultPositionAttrName        ()
{
    return "vertexPosition";
}

// ********************************
//
std::string     FullscreenVSShader::DefaultInUVAttrName            ()
{
    return "vertexTextCoord";
}

// ********************************
//
std::string     FullscreenVSShader::DefaultOutUVAttrName           ()
{
    return "uvCoord";
}

// ********************************
//
std::string     FullscreenVSShader::GenerateHeader                 ()
{
    return "#version 400 \n\n";
}

// ********************************
//
std::string     FullscreenVSShader::GeneratePositionDecl           ( const std::string & positionAttrName )
{
    return  std::string( "layout (location = 0) in vec3 " ) + positionAttrName + std::string( "; \n\n" );
}

// ********************************
//
std::string     FullscreenVSShader::GenerateUVDecl                 ( unsigned int numUVChannels, const std::string & uvInAttrName )
{
    if( numUVChannels == 0 )
    {
        return "";
    }
    else if( numUVChannels == 1 )
    {
        return  std::string( "layout (location = 1) in vec2 " ) + uvInAttrName + std::string( "; \n\n" );
    }
    else
    {
        std::string res = "";

        for( unsigned int i = 0; i < numUVChannels; ++i )
        {
            std::string e = std::string( "layout (location = " ) + std::to_string( i + 1 ) + std::string( ") in vec2 " ) + uvInAttrName + std::to_string( i ) + std::string( "; \n" );            

            res += e;
        }

        res += "\n";

        return res;
    }
}

// ********************************
//
std::string     FullscreenVSShader::GenerateUVOutVarsDecl          ( unsigned int numUVChannels, const std::string & uvOutAttrName )
{
    if( numUVChannels == 0 )
    {
        return "";
    }
    else if( numUVChannels == 1 )
    {
        return  std::string( "out vec2 " ) + uvOutAttrName + std::string( "; \n\n" );
    }
    else
    {
        std::string res = "";

        for( unsigned int i = 0; i < numUVChannels; ++i )
        {
            std::string e = std::string( "out vec2 " ) + uvOutAttrName + std::to_string( i ) + std::string( "; \n\n" );

            res += e;
        }

        res += "\n";
    
        return res;
    }
}

// ********************************
//
std::string     FullscreenVSShader::GenerateMVPDecl                ()
{
    return "uniform mat4 MVP; \n\n";
}

// ********************************
//
std::string     FullscreenVSShader::GenerateOutGLPositionCode      ( const std::string & positionAttrName )
{
    return std::string( "    gl_Position = MVP * vec4( ") + positionAttrName + std::string( ", 1.0 );\n" );
}

// ********************************
//
std::string     FullscreenVSShader::GenerateOutUVMappingCode       ( unsigned int numUVChannels, const std::string & uvOutAttrName, const std::string & uvInAttrName )
{
    if( numUVChannels == 0 )
    {
        return "";
    }
    else if( numUVChannels == 1 )
    {
        return std::string( "    " ) + uvOutAttrName  + std::string( " = " ) + uvInAttrName + std::string( "; \n" );
    }
    else
    {
        std::string res = "";

        for( unsigned int i = 0; i < numUVChannels; ++i )
        {
            std::string e = std::string( "    " ) + uvOutAttrName + std::to_string( i ) + std::string( " = " ) + uvInAttrName + std::to_string( i ) + std::string( "; \n" );

            res += e;
        }

        return res;
    }
}

// ********************************
//
std::string     FullscreenVSShader::GenerateMainStart              ()
{
    return "void main()\n{\n";
}

// ********************************
//
std::string     FullscreenVSShader::GenerateMainEnd                ()
{
    return "}\n";
}

} //nrl
} //bv
