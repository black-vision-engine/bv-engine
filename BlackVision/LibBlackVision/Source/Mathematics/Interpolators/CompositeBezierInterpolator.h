#include "BasicInterpolator.h"

namespace bv
{

class CompositeBezierInterpolator
{
    typedef float TimeValueT;
    typedef float ValueT;

//    BasicInterpolator< float, float > i;
    std::vector<Key<TimeValueT, ValueT>>    keys;
    TimeValueT                              tolerance;

public:
    typedef TimeValueT  TimeType;
    typedef ValueT      ValueType;

    typedef TimeValueT                  TimeT;
    typedef ValueT                      ValT;
public:
    void AddKey             ( TimeValueT t, const ValueT & v );
    ValueT Evaluate         ( TimeValueT t ) const;

// FIXME: below is to remove
    const std::vector<Key<TimeValueT, ValueT>> & AccessKeys() const { static std::vector<Key<TimeValueT, ValueT>> ret; return ret; };
    void                    SetInterpolationMethod ( model::IParameter::InterpolationMethod /*method*/ ) { /*m_method = method;*/ }
    model::IParameter::InterpolationMethod     GetInterpolationMethod () const { return model::IParameter::InterpolationMethod::LINEAR; } // WTF
    void SetWrapPostMethod  ( WrapMethod ) {}
    void SetWrapPreMethod   ( WrapMethod ) {}
};

}