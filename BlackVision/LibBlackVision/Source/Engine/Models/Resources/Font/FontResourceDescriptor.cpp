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
FontResourceDescConstPtr FontResourceDesc::Create( const std::string & fontFileName, SizeType size, SizeType blurSize, SizeType outlineSize, bool generateMipmaps, const std::wstring & atlasCharSetFile )
{
	return std::make_shared< FontResourceDesc >( fontFileName, size, blurSize, outlineSize, generateMipmaps, atlasCharSetFile );
}

// ***********************
//
FontResourceDesc::FontResourceDesc( const std::string & fontFileName, SizeType size, SizeType blurSize, SizeType outlineSize, bool generateMipmaps, const std::wstring & atlasCharSetFile )
	: m_fontFileName( fontFileName )
	, m_size( size )
	, m_blurSize( blurSize )
	, m_outlineSize( outlineSize )
	, m_generateMipmaps( generateMipmaps )
	, m_atlasCharSetFile( atlasCharSetFile )
{}

} // bv

