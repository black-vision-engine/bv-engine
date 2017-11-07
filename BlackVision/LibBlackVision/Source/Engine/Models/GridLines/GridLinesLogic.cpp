#include "stdafx.h"
#include "GridLinesLogic.h"

#include "Engine/Models/SceneModel.h"
#include "Engine/Editors/BVProjectEditor.h"

#include "Application/ApplicationContext.h"

#include "Assets/DataArray/DataArrayAssetDescriptor.h"
#include "Assets/DataArray/DataArrayTypedRows.h"


#include "Memory/MemoryLeaks.h"



namespace bv { namespace model
{

UInt16              GridLinesLogic::m_sMaxGridLines     = 32;
const std::string   gGizmoFunctionalityName             = "GridLines";




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
    ClearAll();
}

// ***********************
//
void            GridLinesLogic::Deserialize             ( const IDeserializer & deser )
{
    ClearAll();

    if( deser.EnterChild( "gridlines" ) )
    {
        //m_showGridLines = Convert::String2T( deser.GetAttribute( "show" ), false );

        if( deser.EnterChild( "horizontal" ) )
        {
            if( deser.EnterChild( "gridline" ) )
            {
                do
                {
                    std::string gridLineName = deser.GetAttribute( "name" );
                    float position = Convert::String2T( deser.GetAttribute( "position" ), 0.0f );
                    int index = Convert::String2T( deser.GetAttribute( "index" ), std::numeric_limits< int >::max() );
                
                    if( index != std::numeric_limits< int >::max() )
                    {
                        // This allocates new grid line
                        MoveGridLine( GridLineType::TST_Horizontal, index, position );
                        RenameGridLine( GridLineType::TST_Horizontal, index, gridLineName );
                    }

                } while( deser.NextChild() );
                deser.ExitChild();  //  gridline
            }
            deser.ExitChild();  // horizontal
        }

        if( deser.EnterChild( "vertical" ) )
        {
            if( deser.EnterChild( "gridline" ) )
            {
                do
                {
                    std::string gridLineName = deser.GetAttribute( "name" );
                    float position = Convert::String2T( deser.GetAttribute( "position" ), 0.0f );
                    int index = Convert::String2T( deser.GetAttribute( "index" ), std::numeric_limits< int >::max() );
                
                    if( index != std::numeric_limits< int >::max() )
                    {
                        // This allocates new grid line
                        MoveGridLine( GridLineType::TST_Vertical, index, position );
                        RenameGridLine( GridLineType::TST_Vertical, index, gridLineName );
                    }

                } while( deser.NextChild() );
                deser.ExitChild();  //  gridline
            }
            deser.ExitChild();  // horizontal
        }
        deser.ExitChild();  // gridlines
    }
}

// ***********************
//
void            GridLinesLogic::SerializeGridArray      ( ISerializer & ser, const std::vector< GridLine * > & gridArray ) const
{
    for( int i = 0; i < ( Int32 )gridArray.size(); ++i )
    {
        if( gridArray[ i ] )
        {
            ser.EnterChild( "gridline" );
                ser.SetAttribute( "index", Convert::T2String( i ) );
                ser.SetAttribute( "position", Convert::T2String( gridArray[ i ]->GetPosition() ) );
                ser.SetAttribute( "name", gridArray[ i ]->GetName() );
            ser.ExitChild();    //  gridline
        }
    }
}

// ***********************
//
void            GridLinesLogic::Serialize               ( ISerializer & ser ) const
{
    ser.EnterChild( "gridlines" );

        //ser.SetAttribute( "show", Convert::T2String( m_showGridLines ) );
        
        ser.EnterArray( "horizontal" );
            SerializeGridArray( ser, m_horizontalGridLines );
        ser.ExitChild();    //  horizontal

        ser.EnterArray( "vertical" );
            SerializeGridArray( ser, m_verticalGridLines );
        ser.ExitChild();    //  vertical

    ser.ExitChild();    // gridlines
}

// ***********************
//
void            GridLinesLogic::MoveGridLine    ( GridLineType gridType, int gridIndex, Float32 newPosition )
{
    auto & gridLineVec = SelectGridLineVec( gridType );
    AllocGridLine( gridType, gridIndex, gridLineVec );

    gridLineVec[ gridIndex ]->SetPosition( newPosition );

    UpdateID();
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

    delete gridLineVec[ gridIndex ];    // In case grid line doesn't exists, delete nullptr is safe.
    gridLineVec[ gridIndex ] = nullptr;

    UpdateID();
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
void            GridLinesLogic::ShowGridLines           ( SceneModelPtr scene, BVProjectEditor * editor, bool enable )
{
    auto gridNode = editor->QueryGizmoNode( scene->GetRootNode(), gGizmoFunctionalityName );
    
    gridNode->SetVisible( enable );
    m_showGridLines = enable;
}

// ***********************
//
DataArrayRowAssetDescConstPtr      GridLinesLogic::BuildDataArray   ( float lineSize ) const
{
    std::vector< glm::vec3 > typedRow;
    std::vector< DataArrayRowBase * > rows;

    float halfSize = lineSize / 2.0f;

    for( auto line : m_verticalGridLines )
    {
        if( line )
        {
            typedRow.push_back( glm::vec3( line->GetPosition(), halfSize, 0.0 ) );
            typedRow.push_back( glm::vec3( line->GetPosition(), -halfSize, 0.0 ) );
        }
    }

    for( auto line : m_horizontalGridLines )
    {
        if( line )
        {
            typedRow.push_back( glm::vec3( halfSize, line->GetPosition(), 0.0 ) );
            typedRow.push_back( glm::vec3( -halfSize, line->GetPosition(), 0.0 ) );
        }
    }

    rows.push_back( new DataArrayRow< glm::vec3 >( "Lines", std::move( typedRow ) ) );
    return DataArrayRowAssetDesc::Create( std::move( rows ) );;
}

// ***********************
// Allocates new grid line if it doesn't exists at gridIndex.
void            GridLinesLogic::AllocGridLine   ( GridLineType gridType, int gridIndex, std::vector< GridLine* >& gridLinesVec )
{
    if( gridIndex >= ( Int32 )gridLinesVec.size() )
    {
        gridLinesVec.resize( gridIndex + 1, nullptr );
    }

    if( gridLinesVec[ gridIndex ] == nullptr )
    {
        gridLinesVec[ gridIndex ] = new GridLine( gridType, std::string( "GridLine" + Convert::T2String( gridIndex ) ) );
        UpdateID();
    }
}

// ***********************
//
std::vector< GridLine* >&       GridLinesLogic::SelectGridLineVec       ( GridLineType gridType )
{
    return gridType == GridLineType::TST_Horizontal ? m_horizontalGridLines : m_verticalGridLines;
}

// ***********************
//
void                            GridLinesLogic::ClearAll                ()
{
    for( auto line : m_horizontalGridLines )
        delete line;

    for( auto line : m_verticalGridLines )
        delete line;
}

// ***********************
//
void                            GridLinesLogic::UpdateID                ()
{
    m_gridLinesUpdateID = ApplicationContext::Instance().GetUpdateCounter() + 1;
}

}   // model
}	// bv