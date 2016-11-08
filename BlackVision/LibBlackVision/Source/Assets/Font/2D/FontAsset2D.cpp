#include "stdafx.h"

#include "FontAsset2D.h"


namespace bv {


const std::string FontAsset2D::uid = "FONT_ASSET";


// *******************************
// 
FontAsset2DConstPtr     FontAsset2D::Create( const TextConstPtr & text )
{
    return FontAsset2DConstPtr( new FontAsset2D( text ) );
}

// *******************************
// 
FontAsset2D::FontAsset2D( const TextConstPtr & text )
    : FontAsset( text, text->BuildAtlas() )
{}

// *******************************
// 
const std::string &	    FontAsset2D::GetUID() const
{
    return FontAsset2D::uid;
}

// *******************************
// 
const std::string &	    FontAsset2D::UID()
{
    return FontAsset2D::uid;
}

// ***********************
//
TextAtlasConstPtr       FontAsset2D::GetTextAtlas() const
{
    return std::static_pointer_cast< const TextAtlas >( m_representation );
}

// *******************************
// 
VoidConstPtr            FontAsset2D::QueryThis() const
{
    return shared_from_this();
}

} // bv
