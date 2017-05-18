#pragma once

#include "Assets/AssetDescriptor.h"
#include "Assets/Font/2D/FontAssetDescriptor.h"

#include "CoreDEF.h"


namespace bv {


#define SUPPROTED_CHARS_FILE L"fonts/SupportedChars.txt" // FIXME: Sucks as a vacuum

class FontAsset3DDesc;
DEFINE_CONST_PTR_TYPE( FontAsset3DDesc )



class FontAsset3DDesc : public AssetDesc, public std::enable_shared_from_this< AssetDesc >
{
private:
    static const std::string							uid;

protected:
    virtual const std::string &			GetUID				() const override;

public:
    virtual void						Serialize       ( ISerializer& ser ) const;
    static ISerializableConstPtr		Create          ( const IDeserializer& deser );

    virtual bool						IsCacheable			() const override;

    virtual VoidConstPtr				QueryThis			() const override;

    const std::string &					GetFontFileName		() const;
    UInt32								GetFontSize			() const;
    const std::wstring &				GetAtlasCharSetFile () const;

    virtual std::string					GetKey				() const override;
    virtual std::string                 GetProposedShortKey () const override;

    virtual SizeType                    EstimateMemoryUsage () const override;

    static FontAsset3DDescConstPtr      Create				( const std::string & fontFileName,
                                                              UInt32 fontSize,
                                                              const std::wstring & atlasCharSetFile = SUPPROTED_CHARS_FILE );
    static FontAsset3DDescConstPtr      Create				( FontAssetDescConstPtr desc2D, UInt32 fontSize = 600 );

    static const std::string &			UID();

private:

    explicit							FontAsset3DDesc	( const std::string & fontFileName,
                                                          UInt32 fontSize,
                                                          const std::wstring & atlasCharSetFile );

    std::string		m_fontFileName;
    UInt32			m_fontSize;
    std::wstring	m_atlasCharSetFile;

    mutable std::string                 m_key;

private:

    std::string                         ComputeKey          () const;
};

// ***********************
/// Returns AssetDescriptor UID for Asset in template parameter.
/// @note AssetDescriptor uid and Asset uid are different strings.
template<> inline const std::string & GetAssetDescUID< FontAsset3D >()
{
    return FontAsset3DDesc::UID();
}


} // bv
