#pragma once

#include "Engine/Models/Resources/ResourceLoader.h"

#include "Engine/Models/Resources/Resource.h"
#include "Engine/Types/Enums.h"


namespace bv { namespace model {

class Text;

class FontResource : public Resource
{
private:
    size_t          m_fontSize;

public:
    size_t          GetFontSize     ()   const;

    explicit        FontResource    ( const std::string& filePath, size_t fontSize );
};

class FontLoader : public ResourceLoader
{
private:

    const Text*             TryLoadFont             ( const std::string& file, size_t size ) const;
    const Text*             TryLoadFontBold         ( const std::string& file, size_t size ) const;
    const Text*             TryLoadFontBoldItalic   ( const std::string& file, size_t size ) const;
    const Text*             TryLoadFontItalic       ( const std::string& file, size_t size ) const;

public:
    ResourceHandle *        LoadResource        ( IResource* res )  const;

    explicit                FontLoader( ){}

    virtual ~FontLoader(){}
};

class FontExtraData : public ResourceExtraData
{
    const Text*             m_font;
    const Text*             m_fontBold;
    const Text*             m_fontBoldItalic;
    const Text*             m_fontItalic;
    const Text*             m_fontUnderlined;

    size_t                  m_fontSize;

public:

    const Text*             GetFont             () const;
    const Text*             GetFontBold         () const;
    const Text*             GetFontBoldItalic   () const;
    const Text*             GetFontItalic       () const;
    const Text*             GetFontUnderlined   () const;

    explicit                FontExtraData       ( const Text* font, const Text* fontBold, const Text* fontItalic, const Text* fontBoldItalic, const Text* fontUnderlined, size_t fontSize );
};


ResourceHandle *            LoadFont( const std::string& fontFileName, size_t size );


} // model
} // bv