#pragma once

#include "Assets/Font/IFontEngine.h"
#include "Assets/Font/3D/TextGeometry.h"
#include "Assets/Font/2D/TextAtlas.h"


namespace bv { 

class Text;
DEFINE_CONST_PTR_TYPE(Text)

class Text
{
private:

    std::wstring                        m_supportedCharsSetFile;
    std::string                         m_fontFile;
    TextAtlasPtr                        m_atlas;
    UInt32                              m_fontSize;
    UInt32                              m_blurSize;
    UInt32                              m_outlineWidth; // if > 0 text is outlined
    IFontEnginePtr                      m_fontEngine;
    bool                                m_withMipmaps;

    //void                                BuildAtlasOutlined    ();
    void                                GenerateMipMaps     ( TextAtlasPtr atlas ) const;
    void                                BlurAtlas           ( TextAtlasPtr atlas ) const;
    void                                AddTexturesKey      ( TextAtlasPtr atlas ) const;
    TextAtlasPtr                        LoadFromCache       () const;
    void                                AddToCache          ( TextAtlasPtr atlas ) const;

public:

    TextAtlasPtr                        BuildAtlas          () const;
    TextGeometryPtr                     BuildGeometry       () const;

    static TextConstPtr                 Create(const std::wstring & supportedCharsSet
                                            , const std::string & fontFile
                                            , UInt32 fontSize
                                            , UInt32 blurSize
                                            , UInt32 outlineSize
                                            , bool withMipmaps );

    explicit                            Text( const std::wstring & supportedCharsSet
                                            , const std::string & fontFile
                                            , UInt32 fontSize
                                            , UInt32 blurSize
                                            , UInt32 outlineSize
                                            , bool withMipmaps );

    UInt32                              GetFontSize         () const    { return m_fontSize; }

};

} // bv
