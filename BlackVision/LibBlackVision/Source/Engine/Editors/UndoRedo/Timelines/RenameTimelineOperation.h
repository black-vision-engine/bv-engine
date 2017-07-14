#pragma once

#include "Engine/Editors/UndoRedo/IRevertable.h"



namespace bv
{

namespace model
{

class ITimeEvaluator;
DEFINE_PTR_TYPE( ITimeEvaluator );

} // model


// Add node and copy node operation.
class RenameTimelineOperation : public IRevertable
{
private:

    std::string                 m_timelinePath;
    std::string                 m_newName;
    std::string                 m_previousName;

public:

    RenameTimelineOperation     ( const std::string & timelinePath, const std::string & newName, const std::string & previousName );


    virtual bool        Undo    ( BVProjectEditor * editor );
    virtual bool        Redo    ( BVProjectEditor * editor );
};

DEFINE_UPTR_TYPE( RenameTimelineOperation )

}	// bv
