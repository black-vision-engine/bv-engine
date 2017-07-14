#pragma once

#include "Engine/Editors/UndoRedo/IRevertable.h"



namespace bv
{

namespace model
{

class ITimeEvaluator;
class IParameter;
DEFINE_PTR_TYPE( ITimeEvaluator );
DEFINE_PTR_TYPE( IParameter );

} // model


// Add node and copy node operation.
class AssignTimelineOperation : public IRevertable
{
private:

    std::string                 m_sceneName;
    model::ITimeEvaluatorPtr    m_timeline;
    model::ITimeEvaluatorPtr    m_prevTimeline;
    model::IParameterPtr        m_param;

public:

    AssignTimelineOperation     ( const std::string & sceneName, model::ITimeEvaluatorPtr timeline, model::ITimeEvaluatorPtr prevTimeline, model::IParameterPtr param );


    virtual bool        Undo    ( BVProjectEditor * editor );
    virtual bool        Redo    ( BVProjectEditor * editor );
};

DEFINE_UPTR_TYPE( AssignTimelineOperation )

}	// bv
