namespace bv
{

class CompositeBezierInterpolator
{
    typedef float TimeValueT;
    typedef float ValueT;

    BasicInterpolator< float, float > i;
public:
    typedef TimeValueT  TimeType;
    typedef ValueT      ValueType;

    typedef TimeValueT                  TimeT;
    typedef ValueT                      ValT;
public:
    void AddKey             ( TimeValueT t, const ValueT & v ) { i.AddKey( t, v ); }
    ValueT Evaluate         ( TimeValueT t ) const { return i.Evaluate( t ); }

// FIXME: below is to remove
    const std::vector<Key<TimeValueT, ValueT>> & AccessKeys() const { static std::vector<Key<TimeValueT, ValueT>> ret; return ret; };
    void                    SetInterpolationMethod ( model::IParameter::InterpolationMethod /*method*/ ) { /*m_method = method;*/ }
    model::IParameter::InterpolationMethod     GetInterpolationMethod () const { return model::IParameter::InterpolationMethod::LINEAR; } // WTF
    void SetWrapPostMethod  ( WrapMethod ) {}
    void SetWrapPreMethod   ( WrapMethod ) {}
};

}