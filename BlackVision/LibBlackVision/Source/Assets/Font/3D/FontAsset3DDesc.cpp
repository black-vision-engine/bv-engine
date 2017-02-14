#include "stdafx.h"

#include "FontAsset3DDesc.h"

#include "Serialization/SerializationHelper.h"

#include "System/Path.h"
#include "ProjectManager.h"


namespace bv {


const std::string FontAsset3DDesc::uid = "FONT_ASSET_3D_DESC";


// ***********************
//
void                    FontAsset3DDesc::Serialize       ( ISerializer& sob ) const
{
    sob.EnterChild( "asset" );

    sob.SetAttribute( "type", GetUID() );
    sob.SetAttribute( "path", m_fontFileName );
    sob.SetAttribute( "size", SerializationHelper::T2String( m_fontSize ) );

    sob.ExitChild();
}

// ***********************
//
ISerializableConstPtr   FontAsset3DDesc::Create          ( const IDeserializer& dob )
{
    assert( dob.GetAttribute( "type" ) == UID() );

    auto path = dob.GetAttribute( "path" );
    auto size = SerializationHelper::String2T( dob.GetAttribute( "size" ), 10 );

    return FontAsset3DDesc::Create( path, size );
}


// ***********************
//
const std::string &	    FontAsset3DDesc::GetUID() const
{
    return FontAsset3DDesc::UID();
}

// ***********************
//
const std::string &     FontAsset3DDesc::UID()
{
    return FontAsset3DDesc::uid;
}

// ***********************
//
bool                    FontAsset3DDesc::IsCacheable() const
{
    return true;
}

// ***********************
//
VoidConstPtr            FontAsset3DDesc::QueryThis() const
{
    return shared_from_this();
}

// ***********************
//
FontAsset3DDescConstPtr FontAsset3DDesc::Create( const std::string & fontFileName, UInt32 size, const std::wstring & atlasCharSetFile )
{
    return FontAsset3DDescConstPtr( new FontAsset3DDesc( fontFileName, size, atlasCharSetFile ) );
}

// ***********************
//
FontAsset3DDescConstPtr FontAsset3DDesc::Create( FontAssetDescConstPtr desc2D, UInt32 fontSize )
{
    return Create( desc2D->GetFontFileName(), fontSize, desc2D->GetAtlasCharSetFile() );
}

// ***********************
//
FontAsset3DDesc::FontAsset3DDesc( const std::string & fontFileName, UInt32 fontSize, const std::wstring & atlasCharSetFile )
    :   m_fontFileName( fontFileName )
    ,   m_fontSize( fontSize )
    ,   m_atlasCharSetFile( atlasCharSetFile )
{}

// ***********************
//
const std::string &	    FontAsset3DDesc::GetFontFileName() const
{
    return m_fontFileName;
}

// ***********************
//
UInt32                  FontAsset3DDesc::GetFontSize() const
{
    return m_fontSize;
}

// ***********************
//
const std::wstring &    FontAsset3DDesc::GetAtlasCharSetFile () const
{
    return m_atlasCharSetFile;
}


// ***********************
//
std::string             FontAsset3DDesc::GetKey		() const
{
    return m_fontFileName + "_" +
        std::to_string( Path::GetTimestamp( ProjectManager::GetInstance()->ToAbsPath( m_fontFileName ) ) ) + "_" +
        std::to_string( m_fontSize );
}

// ***********************
//
std::string             FontAsset3DDesc::GetProposedShortKey () const
{
    return GetKey();
}

// ***********************
//
SizeType                FontAsset3DDesc::EstimateMemoryUsage () const
{
    return 0;
}



} // bv

