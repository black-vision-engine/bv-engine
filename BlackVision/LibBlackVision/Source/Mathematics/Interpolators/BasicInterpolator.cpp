#include "BasicInterpolator.h"

#include <cassert>
#include <iostream>
#include <algorithm>

#include "Mathematics/glm_inc.h"

#include "CoreDEF.h"
#include "Mathematics/Core/mathfuncs.h"
#include "Mathematics/Defines.h"

namespace bv {

namespace {

// *************************************
//
template<class TimeValueT, class ValueT, class FloatT >
ValueT EvaluateLinear( const Key<TimeValueT, ValueT> & k0, const Key<TimeValueT, ValueT> & k1, TimeValueT t )
{
    if ( !( k0.t <= k1.t && k0.t <= t && t <= k1.t ) )
    {
        std::cerr << "Invalid interval ("<< k0.t <<", " << k1.t << ") or param " << t;
    }

    assert( k0.t <= k1.t && k0.t <= t && t <= k1.t );

    if( k0.t == k1.t )
        return k0.val;

    FloatT scl =FloatT( ( FloatT )1.0 / ( k1.t - k0.t ) );
    FloatT w0 = FloatT( scl * FloatT( k1.t - t ) );
    FloatT w1 = FloatT( ( FloatT )1.0 - w0 );

    ValueT v0 = k0.val;
    ValueT v1 = k1.val;

    return ValueT( v0*w0 + v1*w1 );
}

// *************************************
//

bool Int2Bool( int intVal )
{
    if( intVal >= 1 ) 
        return true;
    else
        return false;
}

// *************************************
//
//template<class TimeValueT, class ValueT, class FloatT >
template<>
bool EvaluateLinear< bv::TimeType, bool, float > ( const Key<bv::TimeType, bool> & k0, const Key<bv::TimeType, bool> & k1, bv::TimeType t )
{
    int intVal = EvaluateLinear< bv::TimeType, int, float > ( Key< bv::TimeType, int >( k0.t, int( k0.val ) ), Key< bv::TimeType, int >( k1.t, int( k1.val ) ), t );
    return Int2Bool( intVal );
}

// *************************************
//
template<class TimeValueT, class ValueT, class FloatT>
ValueT EvaluateCosine( const Key<TimeValueT, ValueT> & k0, const Key<TimeValueT, ValueT> & k1, TimeValueT t )
{
    if ( !( k0.t <= k1.t && k0.t <= t && t <= k1.t ) )
    {
        std::cerr << "Invalid interval ("<< k0.t <<", " << k1.t << ") or param " << t;
    }

    assert( k0.t <= k1.t && k0.t <= t && t <= k1.t );

    if( k0.t == k1.t )
        return k0.val;

    FloatT scaledT = FloatT( PI * ( t - k0.t) / ( k1.t - k0.t ) );
    FloatT w0 = FloatT( ( cos( scaledT ) + 1 ) / 2 );
    FloatT w1 = FloatT( ( FloatT )1.0 - w0 );

    ValueT v0 = k0.val;
    ValueT v1 = k1.val;

    return ValueT( v0*w0 + v1*w1 );
}

// *************************************
//
//template<class TimeValueT, class ValueT, class FloatT >
template<>
bool EvaluateCosine< bv::TimeType, bool, float > ( const Key<bv::TimeType, bool> & k0, const Key<bv::TimeType, bool> & k1, bv::TimeType t )
{
    int intVal = EvaluateCosine< bv::TimeType, int, float > ( Key< bv::TimeType, int >( k0.t, int( k0.val ) ), Key< bv::TimeType, int >( k1.t, int( k1.val ) ), t );
    return Int2Bool( intVal );
}

// *************************************
//
template<class TimeValueT, class ValueT>
ValueT EvaluatePoint( const Key<TimeValueT, ValueT> & k0, const Key<TimeValueT, ValueT> & k1, TimeValueT t )
{
    if ( !( k0.t <= k1.t && k0.t <= t && t <= k1.t ) )
    {
        std::cerr << "Invalid interval ("<< k0.t <<", " << k1.t << ") or param " << t;
    }

    assert( k0.t <= k1.t && k0.t <= t && t <= k1.t );

    if ( k1.t == t )
        return k1.val;
    
    return k0.val;
}

} //anonynous
// *************************************
//

template<class TimeValueT, class ValueT >
Key<TimeValueT, ValueT>::Key(TimeValueT t, ValueT val)
    : t(t), val(val)
{
}

// *************************************
//
template<class TimeValueT, class ValueT, class FloatT >
BasicInterpolator<TimeValueT, ValueT, FloatT>::BasicInterpolator(TimeValueT tolerance)
    : tolerance( tolerance )
    , wrapPost( WrapMethod::clamp )
    , wrapPre( WrapMethod::clamp )
{
    assert( tolerance > static_cast<TimeValueT>(0.) );
    SetInterpolationMethod( model::IParameter::InterpolationMethod::LINEAR );
    //SetInterpolationMethod( model::IParameter::InterpolationMethod::COSINE );
}

// *************************************
//
//template<class TimeValueT, class ValueT, class FloatT>
//void                    BasicInterpolator<TimeValueT, ValueT, FloatT>::SetInterpolationMethod ( model::IParameter::InterpolationMethod method )
//{
//    this->method = method;
//}
//
//// *************************************
////
//template<class TimeValueT, class ValueT, class FloatT>
//model::IParameter::InterpolationMethod     BasicInterpolator<TimeValueT, ValueT, FloatT>::GetInterpolationMethod () const
//{
//    return method;
//}

// *************************************
//
template<class TimeValueT, class ValueT, class FloatT >
void BasicInterpolator<TimeValueT, ValueT, FloatT>::AddKey( TimeValueT t, const ValueT & v )
{
    AddKey( Key<TimeValueT, ValueT>( t, v ) );
}

// *************************************
//
template<class TimeValueT, class ValueT, class FloatT >
void BasicInterpolator<TimeValueT, ValueT, FloatT>::AddKey( const Key<TimeValueT, ValueT> & key )
{
    if( keys.size() == 0 )
    {
        keys.push_back( key );
        return;
    }
    
    TimeValueT t = key.t;
    ValueT v = key.val;

    TimeValueT tStart = keys.front().t;
    TimeValueT tEnd = keys.back().t;

    if ( t < tStart )
    {
        if ( tStart - t > tolerance )
        {
            keys.insert( keys.begin(), key );
        }
        else
        {
            keys[ 0 ].val = v;
        }

        return;
    }

    if ( t > tEnd )
    {
        if ( t - tEnd > tolerance )
        {
            keys.push_back( key );
        }
        else
        {
            ( *keys.end() ).val = v;
        }

        return;
    }

    for( auto it = keys.begin(); it != keys.end(); ++it )
    {
        if ( std::fabs( (*it).t - t ) <= tolerance )
        {
            (*it).val = v;
            return;
        }

        auto next = it + 1;

        if (    t > (*it).t 
            &&  t < (*next).t
            &&  std::fabs( t - (*next).t) > tolerance )
        {
            keys.insert( next, key );

            return;
        }

    }
}

// *************************************
//
template<class TimeValueT, class ValueT, class FloatT  >
ValueT BasicInterpolator<TimeValueT, ValueT, FloatT>::Evaluate( TimeValueT t ) const
{
    assert( keys.size() > 0 && "No keys added to the interpolator" );

    TimeValueT tStart = keys.front().t;
    TimeValueT tEnd = keys.back().t;

    if ( t < tStart )
        t = CalcPreT( t );
    else
        if( t > tEnd )
            t = CalcPostT( t );

    SizeType maxKeyIdx = keys.size() - 1;

    for ( SizeType i = 0; i < keys.size(); ++i )
    {
        if( t >= keys[ i ].t )
        {
            auto i0 = i;
            auto i1 = std::min( maxKeyIdx, i + 1 );

            auto k1 = keys[ i1 ];

            if( t <= k1.t )
            {
                switch( GetInterpolationMethod() )
                {
                case model::IParameter::InterpolationMethod::LINEAR:
                    return EvaluateLinear<TimeValueT, ValueT, FloatT>( keys[ i0 ], keys[ i1 ], t );
                    break;
                case model::IParameter::InterpolationMethod::COSINE:
                    return EvaluateCosine<TimeValueT, ValueT, FloatT>( keys[ i0 ], keys[ i1 ], t );
                    break;
                default:
                    assert( false );
                }
            }
        }
    }

    switch( GetInterpolationMethod() )
    {
    case model::IParameter::InterpolationMethod::LINEAR:
        return EvaluateLinear<TimeValueT, ValueT, FloatT>( keys[ maxKeyIdx ], keys[ maxKeyIdx ], t );
        break;
    case model::IParameter::InterpolationMethod::COSINE:
        return EvaluateCosine<TimeValueT, ValueT, FloatT>( keys[ maxKeyIdx ], keys[ maxKeyIdx ], t );
        break;
    default: 
        assert( false );
        return EvaluatePoint( t );
    }
}

// *************************************
//
template<class TimeValueT, class ValueT, class FloatT >
ValueT BasicInterpolator<TimeValueT, ValueT, FloatT>::EvaluatePoint( TimeValueT t ) const
{
    assert( keys.size() > 0 && "No keys added to the interpolator" );

    TimeValueT tStart = keys.front().t;
    TimeValueT tEnd = keys.back().t;

    if ( t < tStart )
        t = CalcPreT( t );
    else
        if( t > tEnd )
            t = CalcPostT( t );

    SizeType maxKeyIdx = keys.size() - 1;

    for ( SizeType i = 0; i < keys.size(); ++i )
    {
        if( t >= keys[ i ].t )
        {
            auto i0 = i;
            auto i1 = std::min( maxKeyIdx, i + 1 );

            auto k1 = keys[ i1 ];

            if(t <= k1.t)
            {
                return bv::EvaluatePoint( keys[ i0 ], keys[ i1 ], t );
            }
        }
    }

    return bv::EvaluatePoint( keys[ maxKeyIdx ], keys[ maxKeyIdx ], t );
}

// *************************************
//
template<class TimeValueT, class ValueT, class FloatT >
TimeValueT BasicInterpolator<TimeValueT, ValueT, FloatT>::CalcPreT( TimeValueT t ) const
{
    TimeValueT tStart = keys.front().t;
    TimeValueT tEnd = keys.back().t;

    auto interval = tEnd - tStart;
    if( interval <= tolerance )
        return tStart;

    t = t - tStart;

    if( wrapPre == WrapMethod::clamp )
        return tStart;
    else if( wrapPost == WrapMethod::repeat )
    {
        TimeValueT q = interval;
        TimeValueT r = std::modf( t, &q );
        return tStart + r;
    }
    else if ( wrapPost == WrapMethod::pingPong )
    {
        TimeValueT q = interval;
        TimeValueT r = std::modf( t, &q );

        if( round( q ) % 2 == 0 )
        {
            return tStart + r;
        }
        else
        {
            return tStart + interval - r;
        }
    }

    return t;
}

template<class TimeValueT, class ValueT, class FloatT >
TimeValueT BasicInterpolator<TimeValueT, ValueT, FloatT>::CalcPostT( TimeValueT t ) const
{
    TimeValueT tStart = keys.front().t;
    TimeValueT tEnd = keys.back().t;

    auto interval = tEnd - tStart;
    if( interval <= tolerance )
        return tEnd;

    t = t - tStart;

    if( wrapPost == WrapMethod::clamp )
        return tEnd;
    else if( wrapPost == WrapMethod::repeat )
    {
        TimeValueT q = interval;
        TimeValueT r = divmod( t, &q );
        return tStart + r;
    }
    else if( wrapPost == WrapMethod::pingPong )
    {
        TimeValueT q = interval;
        TimeValueT r = divmod(t, &q);

        if( round( q ) % 2 == 0 )
        {
            return tStart + r;
        }
        else
        {
            return tStart + interval - r;
        }
    }

    return t;
}

// *************************************
//
template<class TimeValueT, class ValueT, class FloatT >
void BasicInterpolator<TimeValueT, ValueT, FloatT>::SetWrapPostMethod( WrapMethod wm )
{
    wrapPost = wm;
}

// *************************************
//
template<class TimeValueT, class ValueT, class FloatT >
void BasicInterpolator<TimeValueT, ValueT, FloatT>::SetWrapPreMethod (WrapMethod wm )
{
    wrapPre= wm;
}

// *************************************
//
template<class TimeValueT, class ValueT, class FloatT >
WrapMethod BasicInterpolator<TimeValueT, ValueT, FloatT>::GetWrapPostMethod() const
{
    return wrapPost;
}

// *************************************
//
template<class TimeValueT, class ValueT, class FloatT >
WrapMethod BasicInterpolator<TimeValueT, ValueT, FloatT>::GetWrapPreMethod() const
{
    return wrapPre;
}

// *************************************
//
template<class TimeValueT, class ValueT, class FloatT >
void BasicInterpolator<TimeValueT, ValueT, FloatT>::SetWrapMethod( WrapMethod pre, WrapMethod post )
{
    SetWrapPostMethod( post );
    SetWrapPreMethod( pre );
}

// *************************************
//
template<class TimeValueT, class ValueT, class FloatT >
int BasicInterpolator<TimeValueT, ValueT, FloatT>::EvalToCBuffer( TimeValueT time, char * buf ) const
{
    ValueT val = Evaluate( time );
    memcpy( buf, &val, value_size );

    return value_size;
}

// *************************************
//
template<class TimeValueT, class ValueT, class FloatT>
const typename BasicInterpolator<TimeValueT, ValueT, FloatT>::KeyType &     BasicInterpolator<TimeValueT, ValueT, FloatT>::FirstKey    () const
{
    return keys[ 0 ];
}

// *************************************
//
template<class TimeValueT, class ValueT, class FloatT>
const typename BasicInterpolator<TimeValueT, ValueT, FloatT>::KeyType &     BasicInterpolator<TimeValueT, ValueT, FloatT>::LastKey     () const
{
    return keys.back();
}

} // bv

#define INSTANTIATE(TYPE) \
template bv::BasicInterpolator<TYPE,TYPE>;

INSTANTIATE(float)
INSTANTIATE(double)
INSTANTIATE(bv::TimeType)

template bv::BasicInterpolator<bv::TimeType, bool>;
template bv::BasicInterpolator<bv::TimeType, int>;
template bv::BasicInterpolator<bv::TimeType, float>;
template bv::BasicInterpolator<bv::TimeType, double>;
template bv::BasicInterpolator<bv::TimeType, glm::vec2>;
template bv::BasicInterpolator<bv::TimeType, glm::vec3>;
template bv::BasicInterpolator<bv::TimeType, glm::vec4>;

#undef INSTANTIATE
