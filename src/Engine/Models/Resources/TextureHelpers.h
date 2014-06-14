#pragma once

#include <string>

namespace bv { namespace model {

class TextureHelper
{
public:
    static char*            LoadImg ( const std::string& filePath, int* width, int* heigth, int* bpp, bool loadFromMemory = true );
    static void             WriteBMP( const std::string& filePath, const char* data, int width, int height, int bpp );
    static char*            Blur    ( const char* data, int width, int height, int bpp, int blurSize );
};

} // model
} // bv