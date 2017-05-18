#pragma once

#include "Assets/AssetDescriptor.h"

#include "CoreDEF.h"


namespace bv {


#define SUPPROTED_CHARS_FILE L"fonts/SupportedChars.txt" // FIXME: Sucks as a vacuum

class FontAssetDesc;
DEFINE_CONST_PTR_TYPE( FontAssetDesc )

class FontAssetDesc : public AssetDesc,  public std::enable_shared_from_this< AssetDesc >
{
private:
    static const std::string            uid;
    
protected:
    virtual const std::string &         GetUID              () const override;

public:
    virtual void                        Serialize           ( ISerializer& sob ) const;
    static ISerializableConstPtr        Create              ( const IDeserializer& dob );

    virtual bool                        IsCacheable         () const override;

    virtual VoidConstPtr                QueryThis           () const override;

    const std::string &                 GetFontFileName     () const;
    UInt32                              GetFontSize         () const;
    UInt32                              GetBlurSize         () const;
    UInt32                              GetGlowBlurSize     () const;
    UInt32                              GetOutlineSize      () const;
    bool                                GetGenerateMipmaps  () const;
    const std::wstring &                GetAtlasCharSetFile () const;

    virtual std::string                 GetKey              () const override;
    virtual std::string                 GetProposedShortKey () const override;

    virtual SizeType                    EstimateMemoryUsage () const override;

    static FontAssetDescConstPtr        Create              (   const std::string & fontFileName,
                                                                UInt32 fontSize,
                                                                UInt32 blurSize,
                                                                UInt32 glowBlurSize,
                                                                UInt32 outlineSize,
                                                                bool generateMipmaps,
                                                                const std::wstring & atlasCharSetFile = SUPPROTED_CHARS_FILE );

    static const std::string &          UID                 ();

private:

    explicit                            FontAssetDesc       (   const std::string & fontFileName,
                                                                UInt32 fontSize,
                                                                UInt32 blurSize,
                                                                UInt32 glowBlurSize,
                                                                UInt32 outlineSize,
                                                                bool generateMipmaps,
                                                                const std::wstring & atlasCharSetFile );

    std::string     m_fontFileName;
    UInt32          m_fontSize;
    UInt32          m_blurSize;
    UInt32          m_glowBlurSize;
    UInt32          m_outlineSize;
    bool            m_generateMipmaps;
    std::wstring    m_atlasCharSetFile;

    mutable std::string                 m_key;

private:

    std::string                         ComputeKey          () const;
};

// ***********************
/// Returns AssetDescriptor UID for Asset in template parameter.
/// @note AssetDescriptor uid and Asset uid are different strings.
template<> inline const std::string& GetAssetDescUID<FontAsset>()
{
    return FontAssetDesc::UID();
}


} // bv