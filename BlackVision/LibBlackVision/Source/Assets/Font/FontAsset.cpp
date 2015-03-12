#include "FontAsset.h"

namespace bv
{

const std::string FontResource::uid = "FONT_RESOURCE";

// *******************************
// 
model::TextConstPtr FontResource::GetText() const
{
	return m_text;
}

// *******************************
// 
FontResourceConstPtr FontResource::Create( const model::TextConstPtr & text )
{
	return std::make_shared< FontResource >( text );
}

// *******************************
// 
FontResource::FontResource( const model::TextConstPtr & text )
	: m_text( text )
{}

// *******************************
// 
const std::string &	FontResource::GetUID() const
{
	return FontResource::uid;
}

// *******************************
// 
const std::string &	FontResource::UID()
{
	return FontResource::uid;
}

// *******************************
// 
VoidConstPtr FontResource::QueryThis() const
{
	return shared_from_this();
}

} // bv