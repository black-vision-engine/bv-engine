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

    static BVGLTranslator                       ms_instance;

private:

    BVGLTranslator  ();

public:

    static std::string  TranslateBufferUsage    ( GLenum usage );
    static std::string  TranslateBufferTarget   ( GLenum target );

};

} //bv
