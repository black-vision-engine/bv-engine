#include "FontAsset.h"

namespace bv
{

const std::string FontAsset::uid = "FONT_RESOURCE";

// *******************************
// 
model::TextConstPtr FontAsset::GetText() const
{
	return m_text;
}

// *******************************
// 
FontAssetConstPtr FontAsset::Create( const model::TextConstPtr & text )
{
	return std::make_shared< FontAsset >( text );
}

// *******************************
// 
FontAsset::FontAsset( const model::TextConstPtr & text )
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