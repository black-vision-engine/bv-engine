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
    :   FontAsset( text, text->BuildGeometry() )
{}

// *******************************
// 
const std::string &	    FontAsset3D::GetUID() const
{
    return FontAsset3D::uid;
}

// *******************************
// 
const std::string &	    FontAsset3D::UID()
{
    return FontAsset3D::uid;
}

// *******************************
// 
VoidConstPtr            FontAsset3D::QueryThis() const
{
    return shared_from_this();
}

// ***********************
//
TextGeometryConstPtr    FontAsset3D::GetTextGeometry () const
{
    return std::static_pointer_cast< const TextGeometry >( m_representation );
}

} // bv

