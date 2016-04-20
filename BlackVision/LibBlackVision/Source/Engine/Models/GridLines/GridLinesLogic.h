#pragma once

#include "GridLine.h"

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

    void                    MoveGridLine            ( GridLineType gridType, int gridIndex, Float32 newPosition );
    void                    RenameGridLine          ( GridLineType gridType, int gridIndex, const std::string & newName );
    void                    RemoveGridLine          ( GridLineType gridType, int gridIndex );

    bool                    AlignNodeToGridLine     ( GridLineType gridType, int gridIndex, model::BasicNodePtr node, GridLineAlignement alignement );

    void                    ShowGridLines           ( bool enable );
    bool                    GetGridLinesVisibility  ()                  { return m_showGridLines; }
    
    ConnectedComponentPtr   BuildConnectedComponent ();
    UInt64                  GetUpdateID             ()                  { return m_gridLinesUpdateID; }

private:

    std::vector< GridLine* > &  SelectGridLineVec       ( GridLineType gridType );
    void                        AllocGridLine           ( GridLineType gridType, int gridIndex, std::vector< GridLine* >& gridLinesVec );
};

}   // model
}	// bv
