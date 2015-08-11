#include "BasicInterpolator.h"

namespace bv
{

template< class TimeValueT, class ValueT >
class IInterpolator
{
public:
    virtual ValueT Evaluate( TimeValueT t ) const = 0;
};

class CompositeBezierInterpolator
{
    typedef float TimeValueT;
    typedef float ValueT;

    std::vector< Key<TimeValueT, ValueT> >    keys;
    std::vector< IInterpolator<TimeValueT, ValueT >* > interpolators; // FIXME: ptr-ize
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

private:
    float PreEvaluate( float t ) const;
    float PostEvaluate( float t ) const;
};

}