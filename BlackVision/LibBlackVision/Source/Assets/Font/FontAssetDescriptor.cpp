#include "FontAssetDescriptor.h"
#include <cassert>

namespace bv
{

const std::string FontAssetDesc::uid = "FONT_ASSET_DESC";

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
	return std::make_shared< FontAssetDesc >( fontFileName, size, blurSize, outlineSize, generateMipmaps, atlasCharSetFile );
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

} // bv

