#include "FontAsset.h"

namespace bv
{

const std::string FontAsset::uid = "FONT_ASSET";

// *******************************
// 
TextConstPtr FontAsset::GetText() const
{
	return m_text;
}

// *******************************
// 
FontAssetConstPtr FontAsset::Create( const TextConstPtr & text )
{
	return FontAssetConstPtr( new FontAsset( text ) );
}

// *******************************
// 
FontAsset::FontAsset( const TextConstPtr & text )
	: m_text( text )
{}

// *******************************
// 
const std::string &	FontAsset::GetUID() const
{
	return FontAsset::uid;
}

// *******************************
// 
const std::string &	FontAsset::UID()
{
	return FontAsset::uid;
}

// *******************************
// 
VoidConstPtr FontAsset::QueryThis() const
{
	return shared_from_this();
}

} // bv