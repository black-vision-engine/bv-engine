namespace bv { namespace model {

// ****************************************************************************************************************************

// *******************************
// FIXME: reimplement with SQT paramter model
inline   void       ParamTransform::SetRotation     ( const glm::vec3 & rotAxis, float angle, TimeType t )
{
    for( unsigned int i = 0; i < m_transformModel.Size(); ++i )
    {
        if( m_transformModel[ i ]->KindKurwaMac() == TransformKind::rotation )
        {
            static_cast< RotationF * >( m_transformModel[ i ] )->SetRotation( t, rotAxis, angle );

            return;
        }
    }
}

// *******************************
// FIXME: reimplement with SQT paramter model
inline   void       ParamTransform::SetScale        ( const glm::vec3 & scale, TimeType t )
{
    for( unsigned int i = 0; i < m_transformModel.Size(); ++i )
    {
        if( m_transformModel[ i ]->KindKurwaMac() == TransformKind::scale )
        {
            static_cast< SimpleTransformF * >( m_transformModel[ i ] )->SetVecVal( t, scale );

            return;
        }
    }
}

// *******************************
// FIXME: reimplement with SQT paramter model
inline   void       ParamTransform::SetTranslation  ( const glm::vec3 & translation, TimeType t )
{
    for( unsigned int i = 0; i < m_transformModel.Size(); ++i )
    {
        if( m_transformModel[ i ]->KindKurwaMac() == TransformKind::translation )
        {
            static_cast< SimpleTransformF * >( m_transformModel[ i ] )->SetVecVal( t, translation );

            return;
        }
    }
}

// *******************************
// FIXME: reimplement with SQT paramter model
inline  void        ParamTransform::SetCenter       ( const glm::vec3 & center, TimeType t )
{
    for( unsigned int i = 0; i < m_transformModel.Size(); ++i )
    {
        if( m_transformModel[ i ]->KindKurwaMac() == TransformKind::fwd_center )
        {
            static_cast< SimpleTransformF * >( m_transformModel[ i ] )->SetVecVal( t, center );
        }
        else if( m_transformModel[ i ]->KindKurwaMac() == TransformKind::inv_center )
        {
            static_cast< SimpleTransformF * >( m_transformModel[ i ] )->SetVecVal( t, -center );
        }
    }
}

// *******************************
// FIXME: reimplement with SQT paramter model
inline  TransformF& ParamTransform::Transform       ()
{
    return m_transformModel;
}

// *******************************
// FIXME: reimplement with SQT paramter model
inline  glm::mat4   ParamTransform::Evaluate        () const
{
    auto t = AbstractModelParameter::GetLocalEvaluationTime();

    return m_transformModel.Evaluate( t );
}

// ****************************************************************************************************************************

// *******************************
//
inline unsigned int  ParamTransformVec::NumTransforms   () const
{
    return (unsigned int) m_transformModelVec.size();
}

// *******************************
// FIXME: reimplement with SQT paramter model
inline  void        ParamTransformVec::SetRotation      ( unsigned int transformNum, const glm::vec3 & rotAxis, float angle, TimeType t )
{
    assert( transformNum < NumTransforms() );

    for( unsigned int i = 0; i < m_transformModelVec[ transformNum ].Size(); ++i )
    {
        if( m_transformModelVec[ transformNum ][ i ]->KindKurwaMac() == TransformKind::rotation )
        {
            static_cast< RotationF * >( m_transformModelVec[ transformNum ][ i ] )->SetRotation( t, rotAxis, angle );

            return;
        }
    }
}

// *******************************
// FIXME: reimplement with SQT paramter model
inline  void        ParamTransformVec::SetScale         ( unsigned int transformNum, const glm::vec3 & scale, TimeType t )
{
    assert( transformNum < NumTransforms() );

    for( unsigned int i = 0; i < m_transformModelVec[ transformNum ].Size(); ++i )
    {
        if( m_transformModelVec[ transformNum ][ i ]->KindKurwaMac() == TransformKind::scale )
        {
            static_cast< RotationF * >( m_transformModelVec[ transformNum ][ i ] )->SetVecVal( t, scale );

            return;
        }
    }
}

// *******************************
// FIXME: reimplement with SQT paramter model
inline  void        ParamTransformVec::SetTranslation   ( unsigned int transformNum, const glm::vec3 & translation, TimeType t )
{
    assert( transformNum < NumTransforms() );

    for( unsigned int i = 0; i < m_transformModelVec[ transformNum ].Size(); ++i )
    {
        if( m_transformModelVec[ transformNum ][ i ]->KindKurwaMac() == TransformKind::translation )
        {
            static_cast< RotationF * >( m_transformModelVec[ transformNum ][ i ] )->SetVecVal( t, translation );

            return;
        }
    }
}

// *******************************
// FIXME: reimplement with SQT paramter model
inline  void        ParamTransformVec::SetCenter   ( unsigned int transformNum, const glm::vec3 & center, TimeType t )
{
    assert( transformNum < NumTransforms() );

    for( unsigned int i = 0; i < m_transformModelVec[ transformNum ].Size(); ++i )
    {
        if( m_transformModelVec[ transformNum ][ i ]->KindKurwaMac() == TransformKind::fwd_center )
        {
            static_cast< RotationF * >( m_transformModelVec[ transformNum ][ i ] )->SetVecVal( t, center );
        }
        else if( m_transformModelVec[ transformNum ][ i ]->KindKurwaMac() == TransformKind::inv_center )
        {
            static_cast< RotationF * >( m_transformModelVec[ transformNum ][ i ] )->SetVecVal( t, -center );
        }
    }
}

// *******************************
// FIXME: reimplement with SQT paramter model
inline  TransformF& ParamTransformVec::Transform        ( unsigned int transformNum )
{
    assert( transformNum < NumTransforms() );

    return m_transformModelVec[ transformNum ];
}

// *******************************
// FIXME: reimplement with SQT paramter model
inline  glm::mat4   ParamTransformVec::Evaluate         ( unsigned int transformNum ) const
{
    assert( transformNum < NumTransforms() );

    auto t = AbstractModelParameter::GetLocalEvaluationTime();

    return m_transformModelVec[ transformNum ].Evaluate( t );
}

} //model
} //bv
