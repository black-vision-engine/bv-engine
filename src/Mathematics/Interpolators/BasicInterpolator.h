#pragma once

#include <vector>


namespace bv
{

enum class WrapMethod : int
{
    clamp = 0,
    repeat = 1,
    pingPong = 2,
};

template<class TimeValueT, class ValueT>
class Key
{
public:
    TimeValueT  t;
    ValueT      val;

public: 
    explicit Key(TimeValueT t, ValueT val);
};

template<class TimeValueT>
class Interpolator
{
public:
    virtual int evalToCBuffer(TimeValueT time, char* buf) const = 0;
};

template<class TimeValueT, class ValueT>
class BasicInterpolator : public Interpolator<TimeValueT>
{
private:
    std::vector<Key<TimeValueT, ValueT>>    keys;
    TimeValueT                              tolerance;

    WrapMethod wrapPre;
    WrapMethod wrapPost;

public:
    typedef TimeValueT TimeT;
    typedef ValueT ValT;
    typedef Interpolator<TimeValueT> ParentType;

    static const int value_size = sizeof(ValueT);

    explicit BasicInterpolator(TimeValueT tolerance = 0.0001);
    virtual ~BasicInterpolator() {};

    void addKey(TimeValueT t, ValueT v);
    void addKey(const Key<TimeValueT, ValueT>& key);

    ValueT evaluate(TimeValueT t) const;
    ValueT evaluatePoint(TimeValueT t) const;

    TimeValueT calcPreT(TimeValueT t) const;
    TimeValueT calcPostT(TimeValueT t) const;

    void setWrapPostMethod(WrapMethod);
    void setWrapPreMethod(WrapMethod);

    WrapMethod getWrapPostMethod() const;
    WrapMethod getWrapPreMethod() const;

    void setWrapMethod(WrapMethod pre, WrapMethod post);

    virtual int evalToCBuffer(TimeValueT time, char* buf) const;
};

}