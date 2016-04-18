#include "stdafx.h"
#include "GridLinesLogic.h"


namespace bv { namespace model
{

UInt16  GridLinesLogic::m_sMaxGridLines = 32;

// ***********************
//
GridLinesLogic::GridLinesLogic()
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
bool            GridLinesLogic::AlignNodeToGridLine     ( GridLineType gridType, int gridIndex, model::BasicNodePtr node, GridLineAlignement alignement )
{
    auto & gridLineVec = SelectGridLineVec( gridType );
    AllocGridLine( gridType, gridIndex, gridLineVec );

    return gridLineVec[ gridIndex ]->AlignNode( node, alignement );
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