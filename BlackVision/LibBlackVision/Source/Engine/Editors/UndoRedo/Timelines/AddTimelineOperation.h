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
class AddTimelineOperation : public IRevertable
{
private:

    std::string                 m_sceneName;
    model::ITimeEvaluatorPtr    m_parentTimeline;
    model::ITimeEvaluatorPtr    m_addedTimeline;

public:

    AddTimelineOperation        ( const std::string & sceneName, model::ITimeEvaluatorPtr parent, model::ITimeEvaluatorPtr added );


    virtual bool        Undo    ( BVProjectEditor * editor );
    virtual bool        Redo    ( BVProjectEditor * editor );
};

DEFINE_UPTR_TYPE( AddTimelineOperation )

}	// bv
