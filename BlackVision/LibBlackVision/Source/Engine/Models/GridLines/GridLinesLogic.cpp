#include "stdafx.h"
#include "GridLinesLogic.h"

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

#include "Application/ApplicationContext.h"


namespace bv { namespace model
{

UInt16  GridLinesLogic::m_sMaxGridLines = 32;

// ***********************
//
GridLinesLogic::GridLinesLogic()
    :   m_showGridLines( false )
{
    m_horizontalGridLines.resize( m_sMaxGridLines, nullptr );
    m_verticalGridLines.resize( m_sMaxGridLines, nullptr );
}

// ***********************
//
GridLinesLogic::~GridLinesLogic()
{
    for( auto line : m_horizontalGridLines )
        delete line;

    for( auto line : m_verticalGridLines )
        delete line;
}

// ***********************
//
void            GridLinesLogic::MoveGridLine    ( GridLineType gridType, int gridIndex, Float32 newPosition )
{
    auto & gridLineVec = SelectGridLineVec( gridType );
    AllocGridLine( gridType, gridIndex, gridLineVec );

    gridLineVec[ gridIndex ]->SetPosition( newPosition );

    m_gridLinesUpdateID = ApplicationContext::Instance().GetTimestamp() + 1;
}

// ***********************
//
void            GridLinesLogic::RenameGridLine  ( GridLineType gridType, int gridIndex, const std::string & newName )
{
    auto & gridLineVec = SelectGridLineVec( gridType );
    AllocGridLine( gridType, gridIndex, gridLineVec );

    gridLineVec[ gridIndex ]->Rename( newName );
}

// ***********************
//
void            GridLinesLogic::RemoveGridLine          ( GridLineType gridType, int gridIndex )
{
    auto & gridLineVec = SelectGridLineVec( gridType );
    AllocGridLine( gridType, gridIndex, gridLineVec );  // In case grid line didn't exist.

    delete gridLineVec[ gridIndex ];
    gridLineVec[ gridIndex ] = nullptr;

    m_gridLinesUpdateID = ApplicationContext::Instance().GetTimestamp() + 1;
}

// ***********************
//
bool            GridLinesLogic::AlignNodeToGridLine     ( GridLineType gridType, int gridIndex, model::BasicNodePtr node, GridLineAlignement alignement )
{
    auto & gridLineVec = SelectGridLineVec( gridType );
    AllocGridLine( gridType, gridIndex, gridLineVec );

    return gridLineVec[ gridIndex ]->AlignNode( node, alignement );
}

// ***********************
//
void            GridLinesLogic::ShowGridLines           ( bool enable )
{
    m_showGridLines = enable;
}

// ***********************
//
ConnectedComponentPtr   GridLinesLogic::BuildConnectedComponent ()
{
    auto comp = model::ConnectedComponent::Create();

    auto * compVertDesc = new model::AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

    auto lineVerts = std::make_shared< model::Float3AttributeChannel >( compVertDesc, "gridLines", false );
    comp->AddAttributeChannel( lineVerts );

    for( auto line : m_verticalGridLines )
    {
        if( line )
        {
            lineVerts->AddAttribute( glm::vec3( line->GetPosition(), 10.0, 0.0 ) );
            lineVerts->AddAttribute( glm::vec3( line->GetPosition(), -10.0, 0.0 ) );
        }
    }

    for( auto line : m_horizontalGridLines )
    {
        if( line )
        {
            lineVerts->AddAttribute( glm::vec3( 10.0, line->GetPosition(), 0.0 ) );
            lineVerts->AddAttribute( glm::vec3( -10.0, line->GetPosition(), 0.0 ) );
        }
    }

    return comp;
}

// ***********************
// Allocates new grid line if it doesn't exists at gridIndex.
void            GridLinesLogic::AllocGridLine   ( GridLineType gridType, int gridIndex, std::vector< GridLine* >& gridLinesVec )
{
    if( gridIndex >= gridLinesVec.size() )
    {
        gridLinesVec.resize( gridIndex + 1, nullptr );
    }

    if( gridLinesVec[ gridIndex ] == nullptr )
    {
        gridLinesVec[ gridIndex ] = new GridLine( gridType, std::string( "GridLine" + SerializationHelper::T2String( gridIndex ) ) );
        m_gridLinesUpdateID = ApplicationContext::Instance().GetTimestamp() + 1;
    }
}

// ***********************
//
std::vector< GridLine* >&       GridLinesLogic::SelectGridLineVec       ( GridLineType gridType )
{
    return gridType == GridLineType::TST_Horizontal ? m_horizontalGridLines : m_verticalGridLines;
}



}   // model
}	// bv