#pragma once

#include <string>
#include <memory>
#include <assert.h>

#include "Engine/Types/Enums.h"

#include "Engine/Models/Interfaces/ITimeEvaluator.h"



/**@page Parameters Parameters
Parameters are a way to animate some @ref Plugins "Plugin" / @ref Effects "Effect" / @ref Logics "Logic" features.

They consist of two major ingredients: a @ref Curves "Curve" and @ref Timelines "Timeline".
Timeline determines the time at which we evaluate the Curve.

@section Curves Curves
We implement one, generic class of Curves: @ref bv::CompositeInterpolator. 
On every interval defined by consecutive key frames, you can have the following interpolators:
- Linear (see @ref bv::LinearEvaluator)
- Bezier and Cosine-like (see @ref bv::BezierEvaluator and subsection below)
- (one of) Polynomial (see @ref bv::PolynomialEvaluator and subsection below)
- Point (see @ref bv::ConstEvaluator)

See @ref bv::CurveType for the complete list of supported curve types.

CompositeInterpolator supports different value types (float, int, bool, int, vectors, strings, etc.), but beware that not every 
combination of value type and interpolator type makes sense. For example, strings cannot be interpolated in any other way than point.

@subsection CosineLikeCurves Bezier and Cosine-like curves

Bezier curves are represented as usual, as two endpoints (which are also keyframes) and two vectors in each endpoint.

We implement Cosine-like curves just as Bezier curves with vectors parallel to OX axis and length equal to 1/5 of the distance between two keyframes.

@subsection PolynomialCurves Polynomial curves

We have several pre-defined curve types that are special case of polynomial curves. They are sometimes called "easing functions".
We support following curves:
- CT_CUBIC_IN,
- CT_CUBIC_OUT,
- CT_ELASTIC_IN,
- CT_ELASTIC_OUT,
- CT_ELASTIC_IN_BOUNCE,
- CT_ELASTIC_OUT_BOUNCE,
- CT_QUARTIC_INOUT,
- CT_CUBIC_INTOUT.

@subsection APIControl API control

See @ref API_Parameters for detailed description of possible Parameter's manipulation.


@see bv::model::IParameter
*/


namespace bv { namespace model {

    class IParameter;
    DEFINE_PTR_TYPE(IParameter)

    class IParameter;
    typedef std::shared_ptr< const IParameter > IParameterConstPtr;




    /**@brief Parameter interface.
    @copydoc Parameters
    @ingroup Model*/
    class IParameter : public ISerializable
    {
    public:
        virtual void                    Serialize       ( ISerializer& ser ) const              = 0;
        virtual const std::string &     GetName         () const								= 0;
        virtual void                    SetName         ( const std::string & name )            = 0;
        virtual ModelParamType          GetType         () const								= 0;

        virtual ITimeEvaluatorPtr       GetTimeEvaluator() const    							= 0;
        virtual void                    SetTimeEvaluator( ITimeEvaluatorPtr timeEvaluator )		= 0;

        virtual VoidPtr                 QueryParamTyped ()										= 0;

        virtual std::string             EvaluateToString( TimeType t ) const                    = 0;

        virtual ~IParameter() {}

    };

    // *********************************
    //
    template< typename ParamTypePtr >
    ParamTypePtr  QueryTypedParam( IParameterPtr param )
    {
        assert( param );
        ModelParamType paramType = param->GetType();
        ModelParamType elementType = ParamTypePtr::element_type::Type();
        if( paramType != elementType)
        {
            return nullptr;
        }

        return std::static_pointer_cast< ParamTypePtr::element_type >( param->QueryParamTyped() );
    }

    // *********************************
    //
    template< typename ParamTypeConstPtr >
    ParamTypeConstPtr QueryTypedParam( IParameterConstPtr param )
    {
        if( param->GetType() != ParamTypeConstPtr::element_type::Type() )
        {
            return nullptr;
        }

        return std::static_pointer_cast< ParamTypeConstPtr::element_type >( std::const_pointer_cast< IParameter >( param )->QueryParamTyped() );
    }

    // *********************************
    //
    template< typename ParamType >
    ParamType * QueryTypedParam( IParameter * param )
    {
        if( param->GetType() != ParamType::Type() )
        {
            return nullptr;
        }

        return static_cast< ParamType * >( param->QueryParamTyped().get() );
    }

    // *********************************
    //
    template< typename ParamType >
    const ParamType * QueryTypedParam( const IParameter * param )
    {
        if( param->GetType() != ParamType::Type() )
        {
            return nullptr;
        }

        return static_cast< const ParamType * >( std::const_pointer_cast< IParameter * >( param )->QueryParamTyped().get() );
    }

    void    SetParamTimeline( IParameterPtr param, ITimeEvaluatorPtr timeline );

} //model
} //bv
