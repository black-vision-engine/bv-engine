#include "stdafx.h"

#include "FontAssetDescriptor.h"

#include "Serialization/SerializationHelper.h"

#include "DataTypes/Hash.h"
#include "ProjectManager.h"



namespace bv {


const std::string FontAssetDesc::uid = "FONT_ASSET_DESC";


// ***********************
//
void                FontAssetDesc::Serialize       ( ISerializer& ser ) const
{
    ser.EnterChild( "asset" );

    ser.SetAttribute( "type", GetUID() );
    ser.SetAttribute( "path", m_fontFileName );
    ser.SetAttribute( "size", Convert::T2String( m_fontSize ) );
    ser.SetAttribute( "blur", Convert::T2String( m_blurSize ) );
    ser.SetAttribute( "glowBlur", Convert::T2String( m_glowBlurSize ) );
    ser.SetAttribute( "outline", Convert::T2String( m_outlineSize ) );
    ser.SetAttribute( "mipmaps", Convert::T2String( m_generateMipmaps ) );

    ser.ExitChild();
}


// ***********************
//
ISerializableConstPtr FontAssetDesc::Create          ( const IDeserializer& deser )
{
    assert( deser.GetAttribute( "type" ) == UID() );

    auto path = deser.GetAttribute( "path" );
    auto size = Convert::String2T( deser.GetAttribute( "size" ), 10 );
    auto blurSize = Convert::String2T( deser.GetAttribute( "blur" ), 0 );
    auto glowBlurSize = Convert::String2T( deser.GetAttribute( "glowBlur" ), 0 );
    auto outSize = Convert::String2T( deser.GetAttribute( "outline" ), 0 );
	auto mipmaps = Convert::String2T( deser.GetAttribute( "mipmaps" ), true );

    return FontAssetDesc::Create( path, size, blurSize, glowBlurSize, outSize, mipmaps );
}


// ***********************
//
const std::string &    FontAssetDesc::GetUID() const
{
    return FontAssetDesc::UID();
}

// ***********************
//
const std::string & FontAssetDesc::UID()
{
    return FontAssetDesc::uid;
}

// ***********************
//
bool FontAssetDesc::IsCacheable() const
{
    return true;
}

// ***********************
//
VoidConstPtr FontAssetDesc::QueryThis() const
{
    return shared_from_this();
}

// ***********************
//
FontAssetDescConstPtr FontAssetDesc::Create( const std::string & fontFileName, UInt32 size, UInt32 blurSize, UInt32 glowBlurSize, UInt32 outlineSize, bool generateMipmaps, const std::wstring & atlasCharSetFile )
{
    return FontAssetDescConstPtr( new FontAssetDesc( fontFileName, size, blurSize, glowBlurSize, outlineSize, generateMipmaps, atlasCharSetFile ) );
}

// ***********************
//
FontAssetDesc::FontAssetDesc( const std::string & fontFileName, UInt32 fontSize, UInt32 blurSize, UInt32 glowBlurSize, UInt32 outlineSize, bool generateMipmaps, const std::wstring & atlasCharSetFile )
    : m_fontFileName( fontFileName )
    , m_fontSize( fontSize )
    , m_blurSize( blurSize )
    , m_glowBlurSize( glowBlurSize )
    , m_outlineSize( outlineSize )
    , m_generateMipmaps( generateMipmaps )
    , m_atlasCharSetFile( atlasCharSetFile )
{}

// ***********************
//
const std::string &    FontAssetDesc::GetFontFileName() const
{
    return m_fontFileName;
}

// ***********************
//
UInt32 FontAssetDesc::GetFontSize() const
{
    return m_fontSize;
}

// ***********************
//
UInt32 FontAssetDesc::GetBlurSize() const
{
    return m_blurSize;
}

// ***********************
//
UInt32 FontAssetDesc::GetGlowBlurSize() const
{
    return m_glowBlurSize;
}

// ***********************
//
UInt32 FontAssetDesc::GetOutlineSize() const
{
    return m_outlineSize;
}

// ***********************
//
bool FontAssetDesc::GetGenerateMipmaps() const
{
    return m_generateMipmaps;
}

// ***********************
//
const std::wstring &    FontAssetDesc::GetAtlasCharSetFile () const
{
    return m_atlasCharSetFile;
}


// ***********************
//
std::string	            FontAssetDesc::GetKey	() const
{
    if( m_key.empty() )
        m_key = ComputeKey();

    return m_key;
}

// ***********************
//
std::string             FontAssetDesc::ComputeKey           () const
{
    return  m_fontFileName + "_" +
            std::to_string( Path::GetTimestamp( m_fontFileName ) ) + "_" +
            std::to_string( m_fontSize ) + "_" +
            std::to_string( m_blurSize ) + "_" +
            std::to_string( m_glowBlurSize ) + "_" +
            std::to_string( m_outlineSize ) + "_" +
            std::to_string( m_generateMipmaps );
}

// ***********************
//
std::string             FontAssetDesc::GetProposedShortKey () const
{
    return GetKey();
}

// ***********************
//
SizeType                FontAssetDesc::EstimateMemoryUsage () const
{
    return 0;
}



} // bv

