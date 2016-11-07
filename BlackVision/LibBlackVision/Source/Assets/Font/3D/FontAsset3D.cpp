#include "stdafx.h"

#include "FontAsset3D.h"


namespace bv {


const std::string FontAsset3D::uid = "FONT_ASSET_3D";


// *******************************
// 
FontAsset3DConstPtr     FontAsset3D::Create( const TextConstPtr & text )
{
    return FontAsset3DConstPtr( new FontAsset3D( text ) );
}

// *******************************
// 
FontAsset3D::FontAsset3D( const TextConstPtr & text )
    :   FontAsset( text )
{}

// *******************************
// 
const std::string &	FontAsset3D::GetUID() const
{
    return FontAsset3D::uid;
}

// *******************************
// 
const std::string &	FontAsset3D::UID()
{
    return FontAsset3D::uid;
}

// *******************************
// 
VoidConstPtr        FontAsset3D::QueryThis() const
{
    return shared_from_this();
}

} // bv

