namespace bv { namespace model {

// ****************************************************************************************************************************


// *******************************
// FIXME: reimplement with SQT paramter model
inline   void       ParamTransform::SetRotation     ( const glm::vec3 & rotAxis, float angle, TimeType t )
{
    m_transformModel.SetRotation( rotAxis, angle, t );
}

// *******************************
// FIXME: reimplement with SQT paramter model
inline   void       ParamTransform::SetScale        ( const glm::vec3 & scale, TimeType t )
{
    m_transformModel.SetScale( scale, t );
}

// *******************************
// FIXME: reimplement with SQT paramter model
inline   void       ParamTransform::SetTranslation  ( const glm::vec3 & translation, TimeType t )
{
    m_transformModel.SetTranslation( translation, t );
}

// *******************************
// FIXME: reimplement with SQT paramter model
inline  void        ParamTransform::SetCenter       ( const glm::vec3 & center, TimeType t )
{
    m_transformModel.SetCenter( center, t );
}

// ***********************
//
inline  void ParamTransform::RemoveRotation      ( TimeType t )
{
    m_transformModel.RemoveRotation( t );
}

// ***********************
//
inline  void ParamTransform::RemoveScale         ( TimeType t )
{
    m_transformModel.RemoveScale( t );
}

// ***********************
//
inline  void ParamTransform::RemoveTranslation   ( TimeType t )
{
    m_transformModel.RemoveTranslation( t );
}

// ***********************
//
inline  void ParamTransform::RemoveCenter        ( TimeType t )
{
    m_transformModel.RemoveCenter( t );
}

// *******************************
// FIXME: reimplement with SQT paramter model
inline  CompositeTransform & ParamTransform::Transform       ()
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

} //model
} //bv
