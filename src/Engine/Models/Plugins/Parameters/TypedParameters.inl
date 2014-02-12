namespace bv { namespace model {

// *******************************
//
template< typename InterpolatorType, typename ValueType >
inline ValueType ParamImpl< InterpolatorType, ValueType >::Evaluate ( TimeType t ) const
{
    t = BaseParameter::GetEvaluationTime( t );
    BaseParameter::SetLastEvaluatedTime( t );
    return m_interpolator.Evaluate( t );
}

// *******************************
//
template< typename InterpolatorType, typename ValueType >
inline  void        ParamImpl< InterpolatorType, ValueType >::SetVal    ( const ValueType & val, TimeType t )
{
    m_interpolator.AddKey( t, val );
}

// *******************************
//
inline  void        ParamMat2::SetVal    ( const glm::mat2 & val, TimeType t )
{
    m_interpolator.AddKey( t, glm::vec4( val[ 0 ][ 0 ], val[ 1 ][ 0 ], val[ 1 ][ 0 ], val[ 1 ][ 1 ] ) );
}

// *******************************
//
inline   void       ParamTransform::SetVal          ( const glm::mat4 & val, TimeType t )
{
    assert( false );
}

// *******************************
//
inline   void       ParamTransform::SetRotation     ( const glm::vec3 & rotAxis, float angle, TimeType t )
{
    for( unsigned int i = 0; i < m_interpolator.Size(); ++i )
    {
        if ( m_interpolator[ i ]->KindKurwaMac() == TransformKind::rotation )
        {
            RotationF * rotInterpolator = static_cast< RotationF * >( m_interpolator[ i ] );
            rotInterpolator->SetRotation( t, rotAxis, angle );
        }
    }
}

// *******************************
//
inline   void       ParamTransform::SetScale        ( const glm::vec3 & scale, TimeType t )
{
    for( unsigned int i = 0; i < m_interpolator.Size(); ++i )
    {
        if ( m_interpolator[ i ]->KindKurwaMac() == TransformKind::scale )
        {
            m_interpolator[ i ]->SetValues( t, scale[ 0 ], scale[ 1 ], scale[ 2 ] );        
        }
    }
}

// *******************************
//
inline   void       ParamTransform::SetTranslation  ( const glm::vec3 & translation, TimeType t )
{
    for( unsigned int i = 0; i < m_interpolator.Size(); ++i )
    {
        if ( m_interpolator[ i ]->KindKurwaMac() == TransformKind::translation )
        {
            m_interpolator[ i ]->SetValues( t, translation[ 0 ], translation[ 1 ], translation[ 2 ] );        
        }
    }
}

// *******************************
//
inline TransformF & ParamTransform::TransformRef()
{
    return m_interpolator;
}

// *******************************
//
inline  glm::mat2   ParamMat2::Evaluate    ( TimeType t ) const
{
    glm::vec4 v = Parent::Evaluate( t );

    return glm::mat2( v[ 0 ], v[ 1 ], v[ 2 ], v [ 3 ] );
}

} //model
} //bv
