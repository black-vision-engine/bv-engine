#include "BasicInterpolator.h"

#include <cassert>
#include <iostream>

#include <glm/glm.hpp>

#include "System/BasicTypes.h"
#include "Mathematics/Core/mathfuncs.h"


namespace bv {

namespace {

// *************************************
//
template<class TimeValueT, class ValueT>
ValueT EvaluateLinear( const Key<TimeValueT, ValueT> & k0, const Key<TimeValueT, ValueT> & k1, TimeValueT t )
{
    if ( !( k0.t <= k1.t && k0.t <= t && t <= k1.t ) )
    {
        std::cerr << "Invalid interval ("<< k0.t <<", " << k1.t << ") or param " << t;
    }

    assert( k0.t <= k1.t && k0.t <= t && t <= k1.t );

    if( k0.t == k1.t )
        return k0.val;

    ValueT scl =ValueT( ( TimeValueT )1.0 / ( k1.t - k0.t ) );
    ValueT w0 = ValueT( scl * ValueT( k1.t - t ) );
    ValueT w1 = ValueT( ( ValueT )1.0 - w0 );

    ValueT v0 = k0.val;
    ValueT v1 = k1.val;

    v0 *= w0;
    v1 *= w1;

    return ValueT( v0 + v1 );
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

template<class TimeValueT, class ValueT>
Key<TimeValueT, ValueT>::Key(TimeValueT t, ValueT val)
    : t(t), val(val)
{
}

// *************************************
//
template<class TimeValueT, class ValueT>
BasicInterpolator<TimeValueT, ValueT>::BasicInterpolator(TimeValueT tolerance)
    : tolerance( tolerance )
    , wrapPost( WrapMethod::clamp )
    , wrapPre( WrapMethod::clamp )
{
    assert( tolerance > static_cast<TimeValueT>(0.) );
}

// *************************************
//
template<class TimeValueT, class ValueT>
void BasicInterpolator<TimeValueT, ValueT>::AddKey( TimeValueT t, const ValueT & v )
{
    AddKey( Key<TimeValueT, ValueT>( t, v ) );
}

// *************************************
//
template<class TimeValueT, class ValueT>
void BasicInterpolator<TimeValueT, ValueT>::AddKey( const Key<TimeValueT, ValueT> & key )
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
template<class TimeValueT, class ValueT>
ValueT BasicInterpolator<TimeValueT, ValueT>::Evaluate( TimeValueT t ) const
{
    assert( keys.size() > 0 && "No keys added to the interpolator" );

    TimeValueT tStart = keys.front().t;
    TimeValueT tEnd = keys.back().t;

    if ( t < tStart )
        t = CalcPreT( t );
    else
        if( t > tEnd )
            t = CalcPostT( t );

    unsigned int maxKeyIdx = keys.size() - 1;

    for ( unsigned int i = 0; i < keys.size(); ++i )
    {
        if( t >= keys[ i ].t )
        {
            auto i0 = i;
            auto i1 = std::min( maxKeyIdx, i + 1 );

            auto k1 = keys[ i1 ];

            if( t <= k1.t )
            {
                return EvaluateLinear( keys[ i0 ], keys[ i1 ], t );
            }
        }
    }

    return EvaluateLinear( keys[ maxKeyIdx ], keys[ maxKeyIdx ], t );
}

// *************************************
//
template<class TimeValueT, class ValueT>
ValueT BasicInterpolator<TimeValueT, ValueT>::EvaluatePoint( TimeValueT t ) const
{
    assert( keys.size() > 0 && "No keys added to the interpolator" );

    TimeValueT tStart = keys.front().t;
    TimeValueT tEnd = keys.back().t;

    if ( t < tStart )
        t = CalcPreT( t );
    else
        if( t > tEnd )
            t = CalcPostT( t );

    unsigned int maxKeyIdx = keys.size() - 1;

    for ( unsigned int i = 0; i < keys.size(); ++i )
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
template<class TimeValueT, class ValueT>
TimeValueT BasicInterpolator<TimeValueT, ValueT>::CalcPreT( TimeValueT t ) const
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

template<class TimeValueT, class ValueT>
TimeValueT BasicInterpolator<TimeValueT, ValueT>::CalcPostT( TimeValueT t ) const
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
template<class TimeValueT, class ValueT>
void BasicInterpolator<TimeValueT, ValueT>::SetWrapPostMethod( WrapMethod wm )
{
    wrapPost = wm;
}

// *************************************
//
template<class TimeValueT, class ValueT>
void BasicInterpolator<TimeValueT, ValueT>::SetWrapPreMethod (WrapMethod wm )
{
    wrapPre= wm;
}

// *************************************
//
template<class TimeValueT, class ValueT>
WrapMethod BasicInterpolator<TimeValueT, ValueT>::GetWrapPostMethod() const
{
    return wrapPost;
}

// *************************************
//
template<class TimeValueT, class ValueT>
WrapMethod BasicInterpolator<TimeValueT, ValueT>::GetWrapPreMethod() const
{
    return wrapPre;
}

// *************************************
//
template<class TimeValueT, class ValueT>
void BasicInterpolator<TimeValueT, ValueT>::SetWrapMethod( WrapMethod pre, WrapMethod post )
{
    SetWrapPostMethod( post );
    SetWrapPreMethod( pre );
}

// *************************************
//
template<class TimeValueT, class ValueT>
int BasicInterpolator<TimeValueT, ValueT>::EvalToCBuffer( TimeValueT time, char * buf ) const
{
    ValueT val = Evaluate( time );
    memcpy( buf, &val, value_size );

    return value_size;
}

// *************************************
//
template<class TimeValueT, class ValueT>
const typename BasicInterpolator<TimeValueT, ValueT>::KeyType &     BasicInterpolator<TimeValueT, ValueT>::FirstKey    () const
{
    return keys[ 0 ];
}

// *************************************
//
template<class TimeValueT, class ValueT>
const typename BasicInterpolator<TimeValueT, ValueT>::KeyType &     BasicInterpolator<TimeValueT, ValueT>::LastKey     () const
{
    return keys.back();
}

} // bv

#define INSTANTIATE(TYPE) \
template bv::BasicInterpolator<TYPE,TYPE>;

INSTANTIATE(float)
INSTANTIATE(double)
INSTANTIATE(bv::TimeType)

template bv::BasicInterpolator<bv::TimeType, float>;
template bv::BasicInterpolator<bv::TimeType, double>;
template bv::BasicInterpolator<bv::TimeType, glm::vec2>;
template bv::BasicInterpolator<bv::TimeType, glm::vec3>;
template bv::BasicInterpolator<bv::TimeType, glm::vec4>;

#undef INSTANTIATE
