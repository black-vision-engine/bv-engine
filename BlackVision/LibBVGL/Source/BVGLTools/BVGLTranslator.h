#pragma once

#include "BVGLDefs.h"

#include <string>
#include <hash_map>

namespace bv {

class BVGLTranslator
{
private:

    static std::hash_map< GLenum, std::string > ms_bufferUsage;
    static std::hash_map< GLenum, std::string > ms_bufferTarget;
	static std::hash_map< GLenum, std::string > ms_textureTarget;
	static std::hash_map< GLenum, std::string > ms_textureFormat;
	static std::hash_map< GLenum, std::string > ms_textureInternalFormat;
    static std::hash_map< GLenum, std::string > ms_attribPointerType;

	static std::hash_map< GLenum, unsigned int > ms_pixelSize;

    static BVGLTranslator                       ms_instance;

private:

    BVGLTranslator  ();

public:

    static std::string  TranslateBufferUsage                ( GLenum usage );
    static std::string  TranslateBufferTarget               ( GLenum target );
    static std::string  TranslateTextureTarget              ( GLenum target );
    static std::string  TranslateTextureFormat              ( GLenum format );
    static std::string  TranslateTextureInternalFormat      ( GLenum format );
    static std::string  TranslateFramebufferTarget          ( GLenum target );
    static std::string  TranslateRenderbufferTarget         ( GLenum target );
    static std::string  TranslateRenderbufferInternalFormat ( GLenum format );
    static std::string  TranslateAttribPointerType          ( GLenum type );

    static unsigned int	GetPixelSize						( GLenum format );

    static std::string  RemoveGLPrefix                      ( const std::string & str );

};

// *****************************
//
template< typename Fun >
std::string TranslateNoGLPrefix( Fun fun, GLenum val )
{
    if( val <= 0 )
        return "UNINITIALIZED";

    return BVGLTranslator::RemoveGLPrefix( fun( val ) ); 
}

} //bv
