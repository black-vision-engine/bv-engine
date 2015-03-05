#include "FontResourceDescriptor.h"
#include <cassert>

namespace bv
{

const std::string FontResourceDesc::uid = "FONT_RESOURCE_DESC";

// ***********************
//
const std::string &	FontResourceDesc::GetUID() const
{
	return FontResourceDesc::UID();
}

// ***********************
//
const std::string & FontResourceDesc::UID()
{
	return FontResourceDesc::uid;
}

// ***********************
//
bool FontResourceDesc::IsCacheable() const
{
	return true;
}

// ***********************
//
VoidConstPtr FontResourceDesc::QueryThis() const
{
	return shared_from_this();
}

// ***********************
//
FontResourceDescConstPtr FontResourceDesc::Create( const std::string & fontFileName, UInt32 size, UInt32 blurSize, UInt32 outlineSize, bool generateMipmaps, const std::wstring & atlasCharSetFile )
{
	return std::make_shared< FontResourceDesc >( fontFileName, size, blurSize, outlineSize, generateMipmaps, atlasCharSetFile );
}

// ***********************
//
FontResourceDesc::FontResourceDesc( const std::string & fontFileName, UInt32 fontSize, UInt32 blurSize, UInt32 outlineSize, bool generateMipmaps, const std::wstring & atlasCharSetFile )
	: m_fontFileName( fontFileName )
	, m_fontSize( fontSize )
	, m_blurSize( blurSize )
	, m_outlineSize( outlineSize )
	, m_generateMipmaps( generateMipmaps )
	, m_atlasCharSetFile( atlasCharSetFile )
{}

// ***********************
//
const std::string &	FontResourceDesc::GetFontFileName() const
{
	return m_fontFileName;
}

// ***********************
//
UInt32 FontResourceDesc::GetFontSize() const
{
	return m_fontSize;
}

// ***********************
//
UInt32 FontResourceDesc::GetBlurSize() const
{
	return m_blurSize;
}

// ***********************
//
UInt32 FontResourceDesc::GetOutlineSize() const
{
	return m_outlineSize;
}

// ***********************
//
bool FontResourceDesc::GetGenerateMipmaps() const
{
	return m_generateMipmaps;
}

// ***********************
//
const std::wstring & FontResourceDesc::GetAtlasCharSetFile () const
{
	return m_atlasCharSetFile;
}

} // bv

