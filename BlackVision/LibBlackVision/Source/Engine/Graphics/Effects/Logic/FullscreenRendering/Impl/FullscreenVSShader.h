#pragma once

#include <string>

namespace bv { 

class FullscreenVSShader
{
private:

public:

    static  std::string     GenerateDefaultVS           ();

private:

    // Default names
    static  std::string     DefaultPositionAttrName     ();
    static  std::string     DefaultInUVAttrName         ();
    static  std::string     DefaultOutUVAttrName        ();

    // Default code generator
    static  std::string     GenerateHeader              ();

    static  std::string     GeneratePositionDecl        ( const std::string & positionAttrName );
    static  std::string     GenerateUVDecl              ( unsigned int numUVChannels, const std::string & uvInAttrName );
    static  std::string     GenerateUVOutVarsDecl       ( unsigned int numUVChannels, const std::string & uvOutAttrName );
    static  std::string     GenerateMVPDecl             ();

    static  std::string     GenerateOutGLPositionCode   ( const std::string & positionAttrName );
    static  std::string     GenerateOutUVMappingCode    ( unsigned int numUVChannels, const std::string & uvOutAttrName, const std::string & uvInAttrName );

    static  std::string     GenerateMainStart           ();
    static  std::string     GenerateMainEnd             ();

};


} //bv
