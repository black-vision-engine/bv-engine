#include "stdafx.h"
#include "DataArrayLoader.h"
#include "DataArrayAssetDescriptor.h"
#include "DataArrayAsset.h"

#include "Serialization/SerializationHelper.h"

#include <cassert>


namespace bv
{

const char separator = ';';

// ***********************
//
template< typename RowType >
DataArrayRowBase *      CreateRow  ( DataArrayAssetDescriptorConstPtr desc, int idx )
{
    std::vector< RowType > typedRow;

    auto & names = desc->GetRowNames();
    auto & rows = desc->GetRows();
    auto & row = rows[ idx ];

    SizeType beginIdx = 0;
    SizeType endIdx = 0;

    while( endIdx < row.size() )
    {
        endIdx = row.find_first_of( separator, beginIdx );
        typedRow.push_back( SerializationHelper::String2T( std::string( row.begin() + beginIdx, row.begin() + endIdx ), std::numeric_limits< RowType >::quiet_NaN() ) );

        beginIdx = endIdx;
    }

    return new DataArrayRow< RowType >( names[ idx ], std::move( typedRow ) );
}


// ******************************
//
AssetConstPtr DataArrayLoader::LoadAsset( const AssetDescConstPtr & desc ) const
{
	auto typedDesc = QueryTypedDesc< DataArrayAssetDescriptorConstPtr >( desc );
	assert( typedDesc );

    auto & rows = typedDesc->GetRows();
    auto & types = typedDesc->GetRowTypes();

    std::vector< DataArrayRowBase * > typedRows;

    for( int i = 0; i < rows.size(); i++ )
    {
        if( types[ i ] == ModelParamType::MPT_FLOAT )
        {
            typedRows.push_back( CreateRow< float >( typedDesc, i ) );
        }
        else if( types[ i ] == ModelParamType::MPT_VEC2 )
        {
            typedRows.push_back( CreateRow< glm::vec2 >( typedDesc, i ) );
        }
        else if( types[ i ] == ModelParamType::MPT_VEC3 )
        {
            typedRows.push_back( CreateRow< glm::vec3 >( typedDesc, i ) );
        }
        else if( types[ i ] == ModelParamType::MPT_VEC4 )
        {
            typedRows.push_back( CreateRow< glm::vec4 >( typedDesc, i ) );
        }
    }

    return DataArrayAsset::Create( std::move( typedRows ) );
}

// ******************************
//
AssetDescConstPtr DataArrayLoader::CreateDescriptor( const IDeserializer& deserializeObject ) const
{
    return std::static_pointer_cast<const AssetDesc>( DataArrayAssetDescriptor::Create( deserializeObject ) );
}

// ******************************
//
ThumbnailConstPtr DataArrayLoader::LoadThumbnail   ( const AssetDescConstPtr & /*desc*/ ) const
{
    assert( !"No thumbnails" );
    return nullptr;
}




}	// bv