namespace bv
{

class CompositeBezierInterpolator
{
    typedef float TimeValueT;
    typedef float ValueT;
public:
    typedef TimeValueT  TimeType;
    typedef ValueT      ValueType;

    typedef TimeValueT                  TimeT;
    typedef ValueT                      ValT;
public:
    //CompositeBezierInterpolator() {}

    void                    SetInterpolationMethod ( model::IParameter::InterpolationMethod /*method*/ ) { /*m_method = method;*/ }
    model::IParameter::InterpolationMethod     GetInterpolationMethod () const { return model::IParameter::InterpolationMethod::LINEAR; } // WTF
    void AddKey             ( TimeValueT /*t*/, const ValueT & /*v*/ ) {}
    ValueT Evaluate         ( TimeValueT /*t*/ ) const { return 0; }

    void SetWrapPostMethod  ( WrapMethod ) {}
    void SetWrapPreMethod   ( WrapMethod ) {}

    const std::vector<Key<TimeValueT, ValueT>> & AccessKeys() const { static std::vector<Key<TimeValueT, ValueT>> ret; return ret; };
};

}