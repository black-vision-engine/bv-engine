#include "stdafx.h"

#include "FontAsset.h"


namespace bv {


const std::string FontAsset::uid = "FONT_ASSET";

// *******************************
// 
TextConstPtr FontAsset::GetText() const
{
	return m_text;
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

} // bv