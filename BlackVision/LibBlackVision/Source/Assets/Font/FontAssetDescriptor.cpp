#include "FontAssetDescriptor.h"
#include <cassert>

namespace bv
{

const std::string FontAssetDesc::uid = "FONT_ASSET_DESC";


// ***********************
//
template<class Serializer>
void FontAssetDesc::SerializeAsset( Serializer& sob ) const
{
    sob.SetName( "asset" );

    sob.SetValue( "type", "font" );
    sob.SetValue( "path", m_fontFileName );
    sob.SetValue( "size", std::to_string( m_fontSize ) );
    sob.SetValue( "blur", std::to_string( m_blurSize ) );
    sob.SetValue( "outline", std::to_string( m_outlineSize ) );
    sob.SetValue( "mipmaps", m_generateMipmaps ? "true" : "false" );

    sob.Pop();
}

// ***********************
//
template<class Deserializer>
FontAssetDescConstPtr FontAssetDesc::DeserializeAsset( Deserializer& dob )
{
    assert( dob.GetValue( "type" ) == "font" );

    auto path = dob.GetValue( "path" );
    auto size = stoul( dob.GetValue( "size" ) );
    auto blurSize = stoul( dob.GetValue( "blur" ) );
    auto outSize = stoul( dob.GetValue( "outline" ) );
    auto mipmaps = dob.GetValue( "mipmaps" ) == "true" ? true : false;

    return FontAssetDesc::Create( path, size, blurSize, outSize, mipmaps );
}


// ***********************
//
void                FontAssetDesc::Serialize       ( SerializeObject & sob ) const
{
	SerializeAsset( sob );
}

// ***********************
//
ISerializableConstPtr FontAssetDesc::Create          ( DeserializeObject & dob )
{
	return DeserializeAsset( dob );
}

// ***********************
//
void FontAssetDesc::Serialize( JsonSerializeObject & sob ) const
{
	SerializeAsset( sob );
}

// ***********************
//
ISerializableConstPtr FontAssetDesc::Create( JsonDeserializeObject & dob )
{
	return DeserializeAsset( dob );
}

// ***********************
//
const std::string &	FontAssetDesc::GetUID() const
{
	return FontAssetDesc::UID();
}

// ***********************
//
const std::string & FontAssetDesc::UID()
{
	return FontAssetDesc::uid;
}

// ***********************
//
bool FontAssetDesc::IsCacheable() const
{
	return true;
}

// ***********************
//
VoidConstPtr FontAssetDesc::QueryThis() const
{
	return shared_from_this();
}

// ***********************
//
FontAssetDescConstPtr FontAssetDesc::Create( const std::string & fontFileName, UInt32 size, UInt32 blurSize, UInt32 outlineSize, bool generateMipmaps, const std::wstring & atlasCharSetFile )
{
	return FontAssetDescConstPtr( new FontAssetDesc( fontFileName, size, blurSize, outlineSize, generateMipmaps, atlasCharSetFile ) );
}

// ***********************
//
FontAssetDesc::FontAssetDesc( const std::string & fontFileName, UInt32 fontSize, UInt32 blurSize, UInt32 outlineSize, bool generateMipmaps, const std::wstring & atlasCharSetFile )
	: m_fontFileName( fontFileName )
	, m_fontSize( fontSize )
	, m_blurSize( blurSize )
	, m_outlineSize( outlineSize )
	, m_generateMipmaps( generateMipmaps )
	, m_atlasCharSetFile( atlasCharSetFile )
{}

// ***********************
//
const std::string &	FontAssetDesc::GetFontFileName() const
{
	return m_fontFileName;
}

// ***********************
//
UInt32 FontAssetDesc::GetFontSize() const
{
	return m_fontSize;
}

// ***********************
//
UInt32 FontAssetDesc::GetBlurSize() const
{
	return m_blurSize;
}

// ***********************
//
UInt32 FontAssetDesc::GetOutlineSize() const
{
	return m_outlineSize;
}

// ***********************
//
bool FontAssetDesc::GetGenerateMipmaps() const
{
	return m_generateMipmaps;
}

// ***********************
//
const std::wstring & FontAssetDesc::GetAtlasCharSetFile () const
{
	return m_atlasCharSetFile;
}


// ***********************
//
std::string           FontAssetDesc::GetKey		() const
{
    return  m_fontFileName + "_" +
            std::to_string( m_fontSize ) + "_" +
            std::to_string( m_blurSize ) + "_" +
            std::to_string( m_outlineSize ) + "_" +
            std::to_string( m_generateMipmaps );
}

// ***********************
//
std::string             FontAssetDesc::GetProposedShortKey () const
{
    return GetKey();
}

} // bv

