#pragma once

#include "GridLine.h"
#include "Engine/Models/BVProjectEditor.h"

#include <vector>

namespace bv { namespace model
{



class GridLinesLogic
{
private:

    static UInt16                   m_sMaxGridLines;

    std::vector< GridLine* >        m_horizontalGridLines;
    std::vector< GridLine* >        m_verticalGridLines;

public:
    GridLinesLogic();
    ~GridLinesLogic();

    void            MoveGridLine            ( GridLineType gridType, int gridIndex, Float32 newPosition );
    void            RenameGridLine          ( GridLineType gridType, int gridIndex, const std::string & newName );

    bool            AlignNodeToGridLine     ( GridLineType gridType, int gridIndex, model::BasicNodePtr node, GridLineAlignement alignement );


private:

    std::vector< GridLine* > &              SelectGridLineVec       ( GridLineType gridType );


    void            AllocGridLine           ( GridLineType gridType, int gridIndex, std::vector< GridLine* >& gridLinesVec );
};

}   // model
}	// bv
