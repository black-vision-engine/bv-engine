#include "stdafx.h"

#include "SVGAssetDescriptor.h"

#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"

#include "DataTypes/Hash.h"
#include "ProjectManager.h"



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
const std::string &     SVGAssetDescriptor::GetPath     () const
{
    return m_path;
}

// ***********************
//
std::string	            SVGAssetDescriptor::GetKey	() const
{
    if( m_key.empty() )
        m_key = ComputeKey();

    return m_key;
}

// ***********************
//
std::string             SVGAssetDescriptor::ComputeKey() const
{
    return m_path + std::to_string( Path::GetTimestamp( ProjectManager::GetInstance()->ToAbsPath( m_path ) ) );
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
    ser.EnterChild( "asset" );

    ser.SetAttribute( "type", UID() );
    ser.SetAttribute( "path", m_path );

    ser.ExitChild();
}

// ***********************
//
ISerializableConstPtr	SVGAssetDescriptor::Create          ( const IDeserializer & deser )
{
    auto path = deser.GetAttribute( "path" );

    return Create( path );
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
