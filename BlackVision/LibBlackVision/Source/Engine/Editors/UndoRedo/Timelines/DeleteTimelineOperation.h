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
class DeleteTimelineOperation : public IRevertable
{
private:

    std::string                 m_sceneName;
    model::ITimeEvaluatorPtr    m_parentTimeline;
    model::ITimeEvaluatorPtr    m_deletedTimeline;

    bool                        m_forceDelete;
    std::string                 m_replaceTimeline;

public:

    DeleteTimelineOperation     ( const std::string & sceneName, model::ITimeEvaluatorPtr parent, model::ITimeEvaluatorPtr deleted );
    DeleteTimelineOperation     ( const std::string & sceneName, model::ITimeEvaluatorPtr parent, model::ITimeEvaluatorPtr deleted, bool force, const std::string & replaceTimeline );


    virtual bool        Undo    ( BVProjectEditor * editor );
    virtual bool        Redo    ( BVProjectEditor * editor );
};

DEFINE_UPTR_TYPE( DeleteTimelineOperation )

}	// bv
