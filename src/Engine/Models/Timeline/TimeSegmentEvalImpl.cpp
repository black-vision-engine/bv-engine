#include "TimeSegmentEvalImpl.h"


namespace bv { namespace model {

// *********************************
//
void    TimeSegmentEvalImpl::InitWrapEvaluators ( TimelineWrapMethod preMethod, TimelineWrapMethod postMethod )
{
    m_wrapEvaluatorsPre.push_back( std::bind( &TimeSegmentEvalImpl::EvalPreClamp, this ) );
    m_wrapEvaluatorsPre.push_back( std::bind( &TimeSegmentEvalImpl::EvalRepeat, this ) );
    m_wrapEvaluatorsPre.push_back( std::bind( &TimeSegmentEvalImpl::EvalMirror, this ) );

    m_wrapEvaluatorsPost.push_back( std::bind( &TimeSegmentEvalImpl::EvalPostClamp, this ) );
    m_wrapEvaluatorsPost.push_back( std::bind( &TimeSegmentEvalImpl::EvalRepeat, this ) );
    m_wrapEvaluatorsPost.push_back( std::bind( &TimeSegmentEvalImpl::EvalMirror, this ) );

    SetWrapEvaluatorPre( preMethod );
    SetWrapEvaluatorPost( postMethod );
}

// *********************************
//
void    TimeSegmentEvalImpl::SetWrapEvaluatorPre ( TimelineWrapMethod method )
{
    m_wrapEvaluatorPre = m_wrapEvaluatorsPre[ (int) method ];
}

// *********************************
//
void    TimeSegmentEvalImpl::SetWrapEvaluatorPost( TimelineWrapMethod method )
{
    m_wrapEvaluatorPost = m_wrapEvaluatorsPost[ (int) method ];
}

} //model
} //bv
