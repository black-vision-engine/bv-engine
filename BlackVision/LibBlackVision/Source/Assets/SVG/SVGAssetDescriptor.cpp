#include "stdafx.h"

#include "SVGAssetDescriptor.h"

#include "Serialization/ISerializer.h"

namespace bv {

namespace { const std::string           uid = "SVG_ASSET_DESC"; }

// ***********************
//
const std::string &      SVGAssetDescriptor::UID         ()
{
    return uid;
}

SVGAssetDescriptor::SVGAssetDescriptor  ( const std::string & path )
    : m_path( path )
{
}

// ***********************
//
const std::string &		SVGAssetDescriptor::GetUID		() const
{
    return SVGAssetDescriptor::UID();
}

// ***********************
//
bool					SVGAssetDescriptor::IsCacheable	() const
{
    return true;
}

// ***********************
//
std::string				SVGAssetDescriptor::GetKey		() const
{
    return m_path;
}

// ***********************
//
SizeType                SVGAssetDescriptor::EstimateMemoryUsage () const
{
    return 0; // FIXME
}

// ***********************
//
void                    SVGAssetDescriptor::Serialize       ( ISerializer & ser ) const
{
    ser;
}

// ***********************
//
VoidConstPtr            SVGAssetDescriptor::QueryThis	() const
{
    return shared_from_this();
}

// ***********************
//
SVGAssetDescriptorPtr    SVGAssetDescriptor::Create          ( const std::string & path)
{
    return SVGAssetDescriptorPtr( new SVGAssetDescriptor( path ) );
}

}
