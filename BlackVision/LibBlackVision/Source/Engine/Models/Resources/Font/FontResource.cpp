#include "FontResource.h"

namespace bv
{

const std::string FontResource::uid = "FONT_RESOURCE";

// *******************************
// 
model::TextAtlasConstPtr FontResource::GetAtlas() const
{
	return m_textAtlas;
}

// *******************************
// 
FontResourceConstPtr FontResource::Create( const model::TextAtlasConstPtr & testAtlas )
{
	return std::make_shared< FontResource >( testAtlas );
}

// *******************************
// 
FontResource::FontResource( const model::TextAtlasConstPtr & textAtlas )
	: m_textAtlas( textAtlas )
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