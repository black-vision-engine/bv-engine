#include "stdafx.h"

#include "FontAssetDescriptor.h"
#include "Serialization/SerializationHelper.h"

#include <cassert>



#include "Memory/MemoryLeaks.h"



namespace bv
{

const std::string FontAssetDesc::uid = "FONT_ASSET_DESC";




// ***********************
//
void                FontAssetDesc::Serialize       ( ISerializer& sob ) const
{
    sob.EnterChild( "asset" );

    sob.SetAttribute( "type", GetUID() );
    sob.SetAttribute( "path", m_fontFileName );
    sob.SetAttribute( "size", SerializationHelper::T2String( m_fontSize ) );
    sob.SetAttribute( "blur", SerializationHelper::T2String( m_blurSize ) );
    sob.SetAttribute( "outline", SerializationHelper::T2String( m_outlineSize ) );
    sob.SetAttribute( "mipmaps", m_generateMipmaps ? "true" : "false" );

    sob.ExitChild();
}

//// ***********************
////
//void FontAssetDesc::Deserialize     ( const IDeserializer& /*sob*/ )
//{
//    assert( false );
//}

// ***********************
//
ISerializableConstPtr FontAssetDesc::Create          ( const IDeserializer& dob )
{
    assert( dob.GetAttribute( "type" ) == UID() );

    auto path = dob.GetAttribute( "path" );
    auto size = SerializationHelper::String2T( dob.GetAttribute( "size" ), 10 );
    auto blurSize = SerializationHelper::String2T( dob.GetAttribute( "blur" ), 0 );
    auto outSize = SerializationHelper::String2T( dob.GetAttribute( "outline" ), 0 );
    auto mipmaps = dob.GetAttribute( "mipmaps" ) == "true" ? true : false;

    return FontAssetDesc::Create( path, size, blurSize, outSize, mipmaps );
}


// ***********************
//
const std::string &	FontAssetDesc::GetUID() const
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
FontAssetDescConstPtr FontAssetDesc::Create( const std::string & fontFileName, UInt32 size, UInt32 blurSize, UInt32 outlineSize, bool generateMipmaps, const std::wstring & atlasCharSetFile )
{
	return FontAssetDescConstPtr( new FontAssetDesc( fontFileName, size, blurSize, outlineSize, generateMipmaps, atlasCharSetFile ) );
}

// ***********************
//
FontAssetDesc::FontAssetDesc( const std::string & fontFileName, UInt32 fontSize, UInt32 blurSize, UInt32 outlineSize, bool generateMipmaps, const std::wstring & atlasCharSetFile )
	: m_fontFileName( fontFileName )
	, m_fontSize( fontSize )
	, m_blurSize( blurSize )
	, m_outlineSize( outlineSize )
	, m_generateMipmaps( generateMipmaps )
	, m_atlasCharSetFile( atlasCharSetFile )
{}

// ***********************
//
const std::string &	FontAssetDesc::GetFontFileName() const
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
const std::wstring & FontAssetDesc::GetAtlasCharSetFile () const
{
	return m_atlasCharSetFile;
}


// ***********************
//
std::string           FontAssetDesc::GetKey		() const
{
    return  m_fontFileName + "_" +
            std::to_string( m_fontSize ) + "_" +
            std::to_string( m_blurSize ) + "_" +
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

