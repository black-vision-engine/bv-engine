#pragma once

#include "Database/DatabaseEntry.h"
#include "2D/TextAtlas.h"


namespace bv { 

class FontAtlasCacheEntry;
DEFINE_PTR_TYPE( FontAtlasCacheEntry )
DEFINE_CONST_PTR_TYPE( FontAtlasCacheEntry )

class FontAtlasCacheEntry : public DatabaseEntry
{
private:

    TextAtlasPtr                            m_textAtlas;

    const static std::vector< std::string > m_sPrimaryKeys;
    const static std::vector< ColumnType >  m_sColumns;

    static std::vector< std::string >       GeneratePrimaryKeys ();
    static std::vector< ColumnType >        GenerateColumnTypes ();

public:

    struct CN
    {
        static const std::string FONT_NAME;
        static const std::string FONT_SIZE;
        static const std::string BLUR_SIZE;
        static const std::string GLOW_BLUR_SIZE;
        static const std::string OUTLINE_WIDTH;
        static const std::string FONT_FILE_NAME;
        static const std::string ATLAS_WIDTH;
        static const std::string ATLAS_HEIGHT;
        static const std::string LEVELS_NUM;
        static const std::string CHAR_SET;
        static const std::string TEXT_ATLAS;
    };

public:

    virtual SizeType                            ColumnTypesCount    () const override;
    virtual ColumnType                          GetColumnType       ( UInt32 idx ) const override;
    virtual ParamType                           GetColumnType       ( const std::string & name ) const override;

    static const std::vector< std::string > &   PrimaryKeys         ();
    static const std::vector< ColumnType > &    ColumnTypes         ();

public:
    
    std::string             GetName         () const;
    SizeType                GetFontSize     () const;
    SizeType                GetBlurSize     () const;
    SizeType                GetGlowBlurSize () const;
    SizeType                GetOutlineWidth () const;
    std::string             GetFontFilePath () const;
    UInt32                  GetLevelsNum    () const;
    UInt32                  GetAtlasWidth   () const;
    UInt32                  GetAtlasHeight  () const;
    std::wstring            GetCharSet      () const;
    TextAtlasPtr            GetTextAtlas    ();

    static FontAtlasCacheEntryPtr   Create  ( TextAtlasPtr & textAtlas, const std::string & fontName, SizeType fontSize,
                                              SizeType blurSize, SizeType glowBlurSize , SizeType outlineWidth, const std::string & fontFilePath,
                                              UInt32 mmLevelsNum, const std::wstring & charSet );

    FontAtlasCacheEntry ();
    FontAtlasCacheEntry ( TextAtlasPtr & textAtlas, const std::string & fontName, SizeType fontSize,
                          SizeType blurSize, SizeType glowBlurSize, SizeType outlineWidth, const std::string & fontFilePath, UInt32 mmLevelsNum,
                          const std::wstring & charSet );
};

} // bv