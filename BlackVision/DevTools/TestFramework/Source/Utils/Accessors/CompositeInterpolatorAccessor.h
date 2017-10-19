#pragma once


#include "CoreDEF.h"
#include "Mathematics/Interpolators/CompositeInterpolator.h"

namespace bv
{


// ***********************
//
class TEST_ACCESSOR( CompositeInterpolator )
{
private:
public:

    template< typename ParamType >
    static std::vector< std::shared_ptr< IEvaluator< TimeType, ParamType > > > &        GetEvaluators   ( std::shared_ptr< CompositeInterpolator< TimeType, ParamType > > interpolator );

    template< typename ParamType >
    static std::vector< std::shared_ptr< IEvaluator< TimeType, ParamType > > > &       GetEvaluators   ( CompositeInterpolator< TimeType, ParamType > * interpolator );

};

// ========================================================================= //
// Implementation
// ========================================================================= //


// ***********************
//
template< typename ParamType >
std::vector< std::shared_ptr< IEvaluator< TimeType, ParamType > > > &       TEST_ACCESSOR( CompositeInterpolator )::GetEvaluator    ( std::shared_ptr< CompositeInterpolator< TimeType, ParamType > > interpolator )
{
    return GetEvaluator( interpolator.get() );
}

// ***********************
//
template< typename ParamType >
std::vector< std::shared_ptr< IEvaluator< TimeType, ParamType > > > &       TEST_ACCESSOR( CompositeInterpolator )::GetEvaluator    ( CompositeInterpolator< TimeType, ParamType > * interpolator )
{
    return &( interpolator->interpolators );
}


}	// bv


