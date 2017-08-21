#include "stdafx.h"
#include "DataArrayAssetDescriptor.h"


#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVSerializeContext.h"

#include "DataTypes/Hash.h"




#include "Memory/MemoryLeaks.h"



namespace bv
{

const std::string DataArrayAssetDesc::uid = "DATA_ARRAY_ASSET_DESC";
const std::string DataArrayRowAssetDesc::uid = "DATA_ARRAY_ROW_ASSET_DESC";


// ========================================================================= //
// DataArrayAssetDesc
// ========================================================================= //

// ***********************
//
DataArrayAssetDesc::DataArrayAssetDesc( std::vector< std::string > && rowNames, std::vector< std::string > && rows, std::vector< ModelParamType > && types )
{
    m_rowNames = std::move( rowNames );
    m_rows = std::move( rows );
    m_rowTypes = std::move( types );
}



// ***********************
//
void                    DataArrayAssetDesc::Serialize       ( ISerializer& ser ) const
{
    auto context = static_cast<BVSerializeContext*>( ser.GetSerializeContext() );

ser.EnterChild( "asset" );
    ser.SetAttribute( "type", UID() );

    ser.EnterArray( "dataRows" );
        
    for( SizeType i = 0; i < m_rows.size(); ++i )
    {
        ser.EnterChild( "row" );
            
        ser.SetAttribute( "name", m_rowNames[ i ] );
        ser.SetAttribute( "data", m_rows[ i ] );
        ser.SetAttribute( "type", SerializationHelper::T2String( m_rowTypes[ i ] ) );

        ser.ExitChild();    // row
    }

    ser.ExitChild();    // dataRows

    if( context->extendedAssetData )
    {
        ser.SetAttribute( "estimatedMemoryUsage", SerializationHelper::T2String( EstimateMemoryUsage() ) );
    }

ser.ExitChild();
}

// ***********************
//
void                    DataArrayAssetDesc::Deserialize     ( const IDeserializer& /*deser*/ )
{}

// ***********************
//
ISerializableConstPtr   DataArrayAssetDesc::Create          ( const IDeserializer& deser )
{
    std::vector< std::string > rowNames;
    std::vector< std::string > rows;
    std::vector< ModelParamType > types;

    if( deser.EnterChild( "dataRows" ) )
    {
        if( deser.EnterChild( "row" ) )
        {
            do
            {
                rowNames.push_back( deser.GetAttribute( "name" ) );
                rows.push_back( deser.GetAttribute( "data" ) );
                types.push_back( SerializationHelper::String2T( deser.GetAttribute( "type" ), ModelParamType::MPT_TOTAL ) );

            } while( deser.NextChild() );

            deser.ExitChild();  // row
        }

        deser.ExitChild();  // dataRows
    }

    return Create( std::move( rowNames ), std::move( rows ), std::move( types ) );;
}

// ***********************
//
DataArrayAssetDescConstPtr      DataArrayAssetDesc::Create              ( std::vector< std::string > && rowNames, std::vector< std::string > && rows, std::vector< ModelParamType > && types )
{
    return DataArrayAssetDescConstPtr( new DataArrayAssetDesc( std::move( rowNames ), std::move( rows ), std::move( types ) ) );
}

// ***********************
//
const std::string &     DataArrayAssetDesc::GetUID() const
{
	return DataArrayAssetDesc::UID();
}

// ***********************
//
const std::string &     DataArrayAssetDesc::UID()
{
	return DataArrayAssetDesc::uid;
}

// ***********************
//
bool                    DataArrayAssetDesc::IsCacheable() const
{
	return false;
}

// ***********************
//
VoidConstPtr            DataArrayAssetDesc::QueryThis() const
{
	return shared_from_this();
}

// ***********************
//
std::string	            DataArrayAssetDesc::GetKey	() const
{
    if( m_key.empty() )
        m_key = ComputeKey();

    return m_key;
}

// ***********************
//
std::string				DataArrayAssetDesc::ComputeKey		() const
{
    std::string key = uid;
    for( auto row : m_rows )
    {
        key += Hash( row ).Get();
    }

    return key;
}

// ***********************
//
std::string             DataArrayAssetDesc::GetProposedShortKey () const
{
    return AssetDesc::GetProposedShortKey();
}

// ***********************
//
SizeType                DataArrayAssetDesc::EstimateMemoryUsage () const
{
    SizeType assetSize = 0;

    return assetSize;
}

// ***********************
//
const std::vector< std::string > &  DataArrayAssetDesc::GetRowNames         () const
{
    return m_rowNames;
}

// ***********************
//
const std::vector< std::string > &  DataArrayAssetDesc::GetRows             () const
{
    return m_rows;
}

// ***********************
//
const std::vector< ModelParamType > &  DataArrayAssetDesc::GetRowTypes         () const
{
    return m_rowTypes;
}

// ========================================================================= //
// DataArrayRowAssetDesc
// ========================================================================= //


// ***********************
//
DataArrayRowAssetDesc::DataArrayRowAssetDesc( std::vector< DataArrayRowBase * > && rows )
{
    m_rows = std::move( rows );
}



// ***********************
//
void                                DataArrayRowAssetDesc::Serialize      ( ISerializer & /*ser*/ ) const
{
    assert( !"No serialization" );
}

// ***********************
//
void                                DataArrayRowAssetDesc::Deserialize    ( const IDeserializer & /*deser*/ )
{
    assert( !"No deserialization" );
}

// ***********************
//
ISerializableConstPtr               DataArrayRowAssetDesc::Create         ( const IDeserializer & /*deser*/ )
{
    return nullptr;
}

// ***********************
//
DataArrayRowAssetDescConstPtr       DataArrayRowAssetDesc::Create         ( std::vector< DataArrayRowBase * > && rows )
{
    return DataArrayRowAssetDescConstPtr( new DataArrayRowAssetDesc( std::move( rows ) ) );
}

// ***********************
//
const std::string &     DataArrayRowAssetDesc::GetUID() const
{
    return DataArrayRowAssetDesc::UID();
}

// ***********************
//
const std::string &     DataArrayRowAssetDesc::UID()
{
    return DataArrayRowAssetDesc::uid;
}

// ***********************
//
bool                    DataArrayRowAssetDesc::IsCacheable() const
{
    return false;
}

// ***********************
//
VoidConstPtr            DataArrayRowAssetDesc::QueryThis() const
{
    return shared_from_this();
}

// ***********************
//
std::string	            DataArrayRowAssetDesc::GetKey	() const
{
    if( m_key.empty() )
        m_key = ComputeKey();

    return m_key;
}

// ***********************
//
std::string				DataArrayRowAssetDesc::ComputeKey		() const
{
    // TODO: how to compute key ??
    std::string key = uid;
    for( auto row : m_rows )
    {
        key += Hash( row->GetName() ).Get();
    }

    return key;
}

// ***********************
//
std::string             DataArrayRowAssetDesc::GetProposedShortKey () const
{
    return AssetDesc::GetProposedShortKey();
}

// ***********************
//
SizeType                DataArrayRowAssetDesc::EstimateMemoryUsage () const
{
    SizeType assetSize = 0;

    return assetSize;
}



}	// bv