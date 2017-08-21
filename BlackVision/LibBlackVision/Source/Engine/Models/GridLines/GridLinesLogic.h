#pragma once

#include "GridLine.h"
#include "Assets/FwdDecls.h"

#include <vector>




namespace bv { namespace model
{

class ConnectedComponent;
DEFINE_PTR_TYPE( ConnectedComponent )


class GridLinesLogic
{
private:

    static UInt16                   m_sMaxGridLines;

    std::vector< GridLine * >       m_horizontalGridLines;
    std::vector< GridLine * >       m_verticalGridLines;

    bool                            m_showGridLines;
    UInt64                          m_gridLinesUpdateID;

public:
    GridLinesLogic();
    ~GridLinesLogic();

    void                    Deserialize             ( const IDeserializer & deser );
    void                    Serialize               ( ISerializer & ser ) const;

    void                    MoveGridLine            ( GridLineType gridType, int gridIndex, Float32 newPosition );
    void                    RenameGridLine          ( GridLineType gridType, int gridIndex, const std::string & newName );
    void                    RemoveGridLine          ( GridLineType gridType, int gridIndex );

    bool                    AlignNodeToGridLine     ( GridLineType gridType, int gridIndex, model::BasicNodePtr node, GridLineAlignement alignement );

    void                    ShowGridLines           ( bool enable );
    bool                    GetGridLinesVisibility  ()                  { return m_showGridLines; }
    UInt64                  GetUpdateID             ()                  { return m_gridLinesUpdateID; }
    
    DataArrayRowAssetDescConstPtr       BuildDataArray  ( float lineSize ) const;

private:

    void                        SerializeGridArray      ( ISerializer & ser,const std::vector< GridLine * > & gridArray ) const;
    std::vector< GridLine* > &  SelectGridLineVec       ( GridLineType gridType );
    void                        AllocGridLine           ( GridLineType gridType, int gridIndex, std::vector< GridLine* >& gridLinesVec );

    void                        ClearAll                ();
    void                        UpdateID                ();
};

}   // model
}	// bv
