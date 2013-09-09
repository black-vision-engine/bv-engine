#include "BasicInterpolator.h"
#include <assert.h>
#include <iostream>

#include <glm/glm.hpp>

namespace bv
{
namespace
{
    template<class TimeValueT, class ValueT>
    ValueT evaluateLinear(const Key<TimeValueT, ValueT>& k0, const Key<TimeValueT, ValueT>& k1, TimeValueT t)
    {
        if (!(k0.t <= k1.t && k0.t <= t && t <= k1.t))
        {
            std::cerr << "Invalid interval ("<< k0.t <<", " << k1.t << ") or param " << t;
        }

        assert(k0.t <= k1.t && k0.t <= t && t <= k1.t);

        if( k0.t == k1.t)
            return k0.val;

        auto scl = (TimeValueT)1.0 / (k1.t - k0.t);
        auto w0 = scl * (k1.t - t);
        auto w1 = (TimeValueT)1.0 - w0;


        ValueT v0 = k0.val;
        ValueT v1 = k1.val;

        v0 *= w0;
        v1 *= w1;

        return ValueT(v0 + v1);
    }

    template<class TimeValueT, class ValueT>
    ValueT evaluatePoint(const Key<TimeValueT, ValueT>& k0, const Key<TimeValueT, ValueT>& k1, TimeValueT t)
    {
        if (!(k0.t <= k1.t && k0.t <= t && t <= k1.t))
        {
            std::cerr << "Invalid interval ("<< k0.t <<", " << k1.t << ") or param " << t;
        }

        assert(k0.t <= k1.t && k0.t <= t && t <= k1.t);

        if ( k1.t == t )
            return k1.val;
    
        return k0.val;
    }

}

template<class TimeValueT, class ValueT>
Key<TimeValueT, ValueT>::Key(TimeValueT t, ValueT val)
    : t(t), val(val)
{}


template<class TimeValueT, class ValueT>
BasicInterpolator<TimeValueT, ValueT>::BasicInterpolator(TimeValueT tolerance)
    : tolerance(tolerance), wrapPost(WrapMethod::clamp), wrapPre(WrapMethod::clamp)
{
    assert(tolerance > static_cast<TimeValueT>(0.));
}

template<class TimeValueT, class ValueT>
void BasicInterpolator<TimeValueT, ValueT>::addKey(TimeValueT t, ValueT v)
{
    addKey(Key<TimeValueT, ValueT>(t,v));
}

template<class TimeValueT, class ValueT>
void BasicInterpolator<TimeValueT, ValueT>::addKey(const Key<TimeValueT, ValueT>& key)
{
    if(keys.size() == 0)
    {
        keys.push_back(key);
        return;
    }
    
    TimeValueT t = key.t;
    ValueT v = key.val;

    TimeValueT tStart = keys.front().t;
    TimeValueT tEnd = keys.back().t;

    if (t < tStart)
    {
        if (tStart - t > tolerance)
        {
            keys.insert(keys.begin(),key);
        }
        else
        {
            keys[0].val = v;
        }

        return;
    }

    if (t > tEnd)
    {
        if (t - tEnd > tolerance)
        {
            keys.push_back(key);
        }
        else
        {
            (*keys.end()).val = v;
        }

        return;
    }

    for(auto it = keys.begin(); it != keys.end(); ++it)
    {
        if (std::fabs( (*it).t - t ) <= tolerance)
        {
            (*it).val = v;
            return;
        }

        auto next = it + 1;

        if (    t > (*it).t 
            &&  t < (*next).t
            &&  std::fabs( t - (*next).t) > tolerance)
        {
            keys.insert( next, key );
            return;
        }

    }

}

template<class TimeValueT, class ValueT>
ValueT BasicInterpolator<TimeValueT, ValueT>::evaluate(TimeValueT t) const
{
    assert(keys.size() > 0 && "No keys added to the interpolator");

    TimeValueT tStart = keys.front().t;
    TimeValueT tEnd = keys.back().t;

    if (t < tStart)
        t = calcPreT( t );
    else
        if(t > tEnd)
            t = calcPostT( t );

    auto maxKeyIdx = keys.size() - 1;

    for (unsigned int i = 0; i < keys.size(); ++i)
    {
        if(t >= keys[i].t)
        {
            auto i0 = i;
            auto i1 = std::min( maxKeyIdx, i + 1 );

            auto k1 = keys[ i1 ];

            if(t <= k1.t)
            {
                return evaluateLinear(keys[i0], keys[ i1 ], t);
            }
        }
    }

    return evaluateLinear(keys[maxKeyIdx], keys[maxKeyIdx], t);
}

template<class TimeValueT, class ValueT>
ValueT BasicInterpolator<TimeValueT, ValueT>::evaluatePoint(TimeValueT t) const
{
    assert(keys.size() > 0 && "No keys added to the interpolator");

    TimeValueT tStart = keys.front().t;
    TimeValueT tEnd = keys.back().t;

    if (t < tStart)
        t = calcPreT( t );
    else
        if(t > tEnd)
            t = calcPostT( t );

    auto maxKeyIdx = keys.size() - 1;

    for (unsigned int i = 0; i < keys.size(); ++i)
    {
        if(t >= keys[i].t)
        {
            auto i0 = i;
            auto i1 = std::min( maxKeyIdx, i + 1 );

            auto k1 = keys[ i1 ];

            if(t <= k1.t)
            {
                return bv::evaluatePoint(keys[i0], keys[ i1 ], t);
            }
        }
    }

    return bv::evaluatePoint(keys[maxKeyIdx], keys[maxKeyIdx], t);
}

template<class TimeValueT, class ValueT>
TimeValueT BasicInterpolator<TimeValueT, ValueT>::calcPreT(TimeValueT t) const
{
    TimeValueT tStart = keys.front().t;
    TimeValueT tEnd = keys.back().t;

    auto interval = tEnd - tStart;
    if(interval <= tolerance)
        return tStart;

    t = t - tStart;

    if(wrapPre == WrapMethod::clamp)
        return tStart;
    else if(wrapPost == WrapMethod::repeat)
    {
        TimeValueT q = interval;
        TimeValueT r = std::modf(t, &q);
        return tStart + r;
    }
    else if (wrapPost == WrapMethod::pingPong)
    {
        TimeValueT q = interval;
        TimeValueT r = std::modf(t, &q);

        if(int( q ) % 2 == 0)
            return tStart + r;
        else
            return tStart + interval - r;
    }
    return t;
}

namespace 
{
    double divmod(double t, double* i)
    {
        double ret = std::fmod(t, *i);
        *i = (t - ret) / *i;

        return ret;
    }

    float divmod(float t, float* i)
    {
        float ret = std::fmod(t, *i);
        *i = (t - ret) / *i;

        return ret;
    }
}

template<class TimeValueT, class ValueT>
TimeValueT BasicInterpolator<TimeValueT, ValueT>::calcPostT(TimeValueT t) const
{
    TimeValueT tStart = keys.front().t;
    TimeValueT tEnd = keys.back().t;

    auto interval = tEnd - tStart;
    if(interval <= tolerance)
        return tEnd;

    t = t - tStart;

    if(wrapPost == WrapMethod::clamp)
        return tEnd;
    else if(wrapPost == WrapMethod::repeat)
    {
        TimeValueT q = interval;
        TimeValueT r = divmod(t, &q);
        return tStart + r;
    }
    else if(wrapPost == WrapMethod::pingPong)
    {
        TimeValueT q = interval;
        TimeValueT r = divmod(t, &q);

        if(int( q ) % 2 == 0)
            return tStart + r;
        else
            return tStart + interval - r;
    }
    return t;
}

template<class TimeValueT, class ValueT>
void BasicInterpolator<TimeValueT, ValueT>::setWrapPostMethod(WrapMethod wm)
{
    wrapPost = wm;
}

template<class TimeValueT, class ValueT>
void BasicInterpolator<TimeValueT, ValueT>::setWrapPreMethod(WrapMethod wm)
{
    wrapPre= wm;
}

template<class TimeValueT, class ValueT>
WrapMethod BasicInterpolator<TimeValueT, ValueT>::getWrapPostMethod() const
{
    return wrapPost;
}

template<class TimeValueT, class ValueT>
WrapMethod BasicInterpolator<TimeValueT, ValueT>::getWrapPreMethod() const
{
    return wrapPre;
}

template<class TimeValueT, class ValueT>
void BasicInterpolator<TimeValueT, ValueT>::setWrapMethod(WrapMethod pre, WrapMethod post)
{
    setWrapPostMethod(post);
    setWrapPreMethod(pre);
}

template<class TimeValueT, class ValueT>
int BasicInterpolator<TimeValueT, ValueT>::evalToCBuffer(TimeValueT time, char* buf) const
{
    ValueT val = evaluate(time);
    memcpy(buf, &val, value_size);

    return value_size;
}

} // bv

#define INSTANTIATE(TYPE) \
template bv::BasicInterpolator<TYPE,TYPE>;

INSTANTIATE(float)
INSTANTIATE(double)

template bv::BasicInterpolator<float, glm::vec3>;
template bv::BasicInterpolator<float, glm::vec4>;

#undef INSTANTIATE