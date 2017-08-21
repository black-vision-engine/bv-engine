#include "stdafx.h"
#include "DataArrayLoader.h"
#include "DataArrayAssetDescriptor.h"
#include "DataArrayAsset.h"

#include "Serialization/SerializationHelper.h"

#include <cassert>




#include "Memory/MemoryLeaks.h"



namespace bv
{

const char separator = ';';

// ***********************
//
template< typename RowType >
DataArrayRowBase *      CreateRow  ( DataArrayAssetDescConstPtr desc, int idx )
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

        // Omit separator.
        beginIdx = endIdx + 1;
        endIdx = beginIdx;
    }

    return new DataArrayRow< RowType >( names[ idx ], std::move( typedRow ) );
}


// ******************************
//
AssetConstPtr           DataArrayLoader::LoadAsset( const AssetDescConstPtr & desc ) const
{
    if( desc->GetUID() == DataArrayAssetDesc::UID() )
    {
        auto typedDesc = QueryTypedDesc< DataArrayAssetDescConstPtr >( desc );
        assert( typedDesc );

        auto & rows = typedDesc->GetRows();
        auto & types = typedDesc->GetRowTypes();

        std::vector< DataArrayRowBase * > typedRows;

        for( Int32 i = 0; i < ( Int32 )rows.size(); i++ )
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
    else if( desc->GetUID() == DataArrayRowAssetDesc::UID() )
    {
        auto typedDesc = QueryTypedDesc< DataArrayRowAssetDescConstPtr >( desc );
        assert( typedDesc );

        auto rows = typedDesc->GetRows();
        return DataArrayAsset::Create( std::move( rows ) );
    }
    
    return nullptr;
}

// ******************************
//
AssetDescConstPtr       DataArrayLoader::CreateDescriptor( const IDeserializer& deserializeObject ) const
{
    return std::static_pointer_cast<const AssetDesc>( DataArrayAssetDesc::Create( deserializeObject ) );
}

// ******************************
//
ThumbnailConstPtr       DataArrayLoader::LoadThumbnail   ( const AssetDescConstPtr & /*desc*/ ) const
{
    assert( !"No thumbnails" );
    return nullptr;
}




}	// bv