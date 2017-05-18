#include "stdafx.h"
#include "DataArrayAssetDescriptor.h"


#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVSerializeContext.h"

#include "DataTypes/Hash.h"




#include "Memory/MemoryLeaks.h"



namespace bv
{

const std::string DataArrayAssetDescriptor::uid = "DATA_ARRAY_ASSET_DESC";


// ***********************
//
DataArrayAssetDescriptor::DataArrayAssetDescriptor( std::vector< std::string > && rowNames, std::vector< std::string > && rows, std::vector< ModelParamType > && types )
{
    m_rowNames = std::move( rowNames );
    m_rows = std::move( rows );
    m_rowTypes = std::move( types );
}



// ***********************
//
void                    DataArrayAssetDescriptor::Serialize       ( ISerializer& ser ) const
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
void                    DataArrayAssetDescriptor::Deserialize     ( const IDeserializer& /*deser*/ )
{}

// ***********************
//
ISerializableConstPtr   DataArrayAssetDescriptor::Create          ( const IDeserializer& deser )
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
DataArrayAssetDescriptorConstPtr    DataArrayAssetDescriptor::Create              ( std::vector< std::string > && rowNames, std::vector< std::string > && rows, std::vector< ModelParamType > && types )
{
    return DataArrayAssetDescriptorConstPtr( new DataArrayAssetDescriptor( std::move( rowNames ), std::move( rows ), std::move( types ) ) );
}

// ***********************
//
const std::string &     DataArrayAssetDescriptor::GetUID() const
{
	return DataArrayAssetDescriptor::UID();
}

// ***********************
//
const std::string &     DataArrayAssetDescriptor::UID()
{
	return DataArrayAssetDescriptor::uid;
}

// ***********************
//
bool                    DataArrayAssetDescriptor::IsCacheable() const
{
	return false;
}

// ***********************
//
VoidConstPtr            DataArrayAssetDescriptor::QueryThis() const
{
	return shared_from_this();
}

// ***********************
//
std::string	            DataArrayAssetDescriptor::GetKey	() const
{
    if( m_key.empty() )
        m_key = ComputeKey();

    return m_key;
}

// ***********************
//
std::string				DataArrayAssetDescriptor::ComputeKey		() const
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
std::string             DataArrayAssetDescriptor::GetProposedShortKey () const
{
    return AssetDesc::GetProposedShortKey();
}

// ***********************
//
SizeType                DataArrayAssetDescriptor::EstimateMemoryUsage () const
{
    SizeType assetSize = 0;

    return assetSize;
}

// ***********************
//
const std::vector< std::string > &  DataArrayAssetDescriptor::GetRowNames         () const
{
    return m_rowNames;
}

// ***********************
//
const std::vector< std::string > &  DataArrayAssetDescriptor::GetRows             () const
{
    return m_rows;
}

// ***********************
//
const std::vector< ModelParamType > &  DataArrayAssetDescriptor::GetRowTypes         () const
{
    return m_rowTypes;
}


}	// bv