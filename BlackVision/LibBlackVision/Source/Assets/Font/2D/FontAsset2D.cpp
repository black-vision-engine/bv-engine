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
    : FontAsset( text )
{}

// *******************************
// 
const std::string &	FontAsset2D::GetUID() const
{
    return FontAsset2D::uid;
}

// *******************************
// 
const std::string &	FontAsset2D::UID()
{
    return FontAsset2D::uid;
}

// *******************************
// 
VoidConstPtr        FontAsset2D::QueryThis() const
{
    return shared_from_this();
}

} // bv
