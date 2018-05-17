#include "stdafx.h"

#include "VideoOutputFactory.h"

#include <functional>
#include <algorithm>
#include <unordered_set>


namespace bv { 

struct OutputStaticDataHash
{
    size_t operator()( const OutputStaticData & in ) const
    {
        // FIXME: nrl
        UInt64 w = ( (UInt64) in.width ) & 0xFFFFFF;
        UInt64 h = ( (UInt64) in.height ) & 0xFFFFFF;
        UInt64 i = ( (UInt64) in.selectedRenderedChannel ) & 0xFFFF;

        std::hash< UInt64 > hsh;

        return hsh( i << 48 | h << 24 | w );
    }
};

// *********************************
//
bool operator == ( OutputStaticData const & lhs, OutputStaticData const & rhs )
{
    return  ( lhs.width == rhs.width ) && 
            ( lhs.height == rhs.height ) && 
            ( lhs.selectedRenderedChannel == rhs.selectedRenderedChannel ); 
}

// *********************************
//
OutputCompositeVideo *  VideoOutputFactory::CreateCompositeVideoOutput( const OutputDesc & desc )
{
    auto & props  = desc.GetOutputProperties();

    auto videoOutputs   = ReadInputEntryVec( props );
    auto staticInputs   = ReadUniqueStaticDataVec( videoOutputs );
    auto mapping        = ReadMapping( videoOutputs );

    return OutputCompositeVideo::Create( desc.GetWidth(), desc.GetHeight(), staticInputs, mapping );
};

// *********************************
//
VideoOutputFactory::VideoOutputDesc VideoOutputFactory::ReadEntry   ( const OutputDesc::TOutputPropertyMap & props )
{
    // FIXME: nrl - this should be verified by Radek and/or Pawelek
    assert( props.find( "width" ) != props.end() );
    assert( props.find( "height" ) != props.end() );
    assert( props.find( "renderChannelID" ) != props.end() );
    assert( props.find( "outputID" ) != props.end() );

    VideoOutputFactory::VideoOutputDesc d;
    auto & id = d.staticOutputData;

    id.width                    = std::stoul( props.find( "width" )->second );
    id.height                   = std::stoul( props.find( "height" )->second );
    
    auto src                    = std::stoul( props.find( "renderChannelID" )->second );
    assert( src < (unsigned int) RenderChannelType::RCT_TOTAL );
    id.selectedRenderedChannel  = ( RenderChannelType ) src;

    d.outputID                  = std::stoul( props.find( "outputID" )->second );
    
    return d;
}

// *********************************
//
std::vector< VideoOutputFactory::VideoOutputDesc >   VideoOutputFactory::ReadInputEntryVec  ( const OutputDesc::TOutputPropertiesVector & props )
{
    std::vector< VideoOutputFactory::VideoOutputDesc >  res;

    for( auto & p : props )
    {
        auto d = ReadEntry( p );
    
        res.push_back( d );
    }

    return res;
}

// *********************************
//
UintUintMapping VideoOutputFactory::ReadMapping ( const std::vector< VideoOutputFactory::VideoOutputDesc > & videoOutputs )
{
    auto staticInputs = ReadUniqueStaticDataVec( videoOutputs );

    UintUintMapping mapping;

    for( auto & d : videoOutputs )
    {
        auto    oID = d.outputID;
        auto &  id  = d.staticOutputData;

        auto it = std::find( staticInputs.begin(), staticInputs.end(), id );

        assert( it != staticInputs.end() );

        auto entryIdx = (unsigned int) (it - staticInputs.begin());

        mapping[ oID ] = entryIdx;
    }

    return mapping;
}

// *********************************
//
OutputStaticDataVec VideoOutputFactory::ReadUniqueStaticDataVec     ( const std::vector< VideoOutputFactory::VideoOutputDesc > & inputVec )
{
    std::unordered_set< OutputStaticData, OutputStaticDataHash >    uniqueEntries;

    for( auto & d : inputVec )
    {    
        uniqueEntries.insert( d.staticOutputData );
    }

    OutputStaticDataVec res( uniqueEntries.begin(), uniqueEntries.end() );

    return res;
}


} // bv
