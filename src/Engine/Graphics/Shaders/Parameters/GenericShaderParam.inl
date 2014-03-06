namespace bv {

// ****************************
//
template< typename ValType >
const ValType &             GenericShaderParam::GenericGetValue () const
{
    //FIXME: implement
    return ValType();   
}

// ****************************
//
inline ParamType            GenericShaderParam::Type            () const
{
    return m_paramType;
}

// ****************************
//
inline const std::string &  GenericShaderParam::Name            () const
{
    return m_paramName;
}

} //bv
