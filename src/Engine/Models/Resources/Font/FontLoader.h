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
    size_t          m_blurSize;
	size_t          m_outlineSize;
    std::wstring    m_atlasCharSetFile;

public:
    size_t                  GetFontSize             ()  const;
    size_t                  GetBlurSize             ()  const;
	size_t                  GetOutlineSize          ()  const;
    const std::wstring&     GetAtlasCharSetFile     ()  const;

    explicit        FontResource    ( const std::string& filePath, size_t fontSize, size_t blurSize, size_t outlineSize, const std::wstring& atlasCharSetFile );
};

class FontLoader : public ResourceLoader
{
private:

    const Text*             TryLoadFont             ( const std::string& file, size_t size, size_t blurSize, size_t oulineSize, const std::wstring& atlasCharSetFile ) const;
    const Text*             TryLoadFontBold         ( const std::string& file, size_t size, size_t blurSize, size_t oulineSize, const std::wstring& atlasCharSetFile ) const;
    const Text*             TryLoadFontBoldItalic   ( const std::string& file, size_t size, size_t blurSize, size_t oulineSize, const std::wstring& atlasCharSetFile ) const;
    const Text*             TryLoadFontItalic       ( const std::string& file, size_t size, size_t blurSize, size_t oulineSize, const std::wstring& atlasCharSetFile ) const;

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

} // model
} // bv