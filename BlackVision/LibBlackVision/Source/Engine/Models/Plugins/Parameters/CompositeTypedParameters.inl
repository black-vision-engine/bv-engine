namespace bv { namespace model {

// ****************************************************************************************************************************


// *******************************
// FIXME: reimplement with SQT paramter model
inline   void       ParamTransform::SetRotation     ( const glm::vec3 & eulerAngles, TimeType t )
{
    m_transformModel.SetRotation( eulerAngles, t );
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

// ***********************
//
inline  bool ParamTransform::MoveRotation      ( TimeType t, TimeType newTime )
{
    return m_transformModel.MoveRotation( t, newTime );
}

// ***********************
//
inline  bool ParamTransform::MoveScale         ( TimeType t, TimeType newTime )
{
    return m_transformModel.MoveScale( t, newTime );
}

// ***********************
//
inline  bool ParamTransform::MoveTranslation   ( TimeType t, TimeType newTime )
{
    return m_transformModel.MoveTranslation( t, newTime );
}

// ***********************
//
inline  bool ParamTransform::MoveCenter        ( TimeType t, TimeType newTime )
{
    return m_transformModel.MoveCenter( t, newTime );
}

// *******************************
//
inline  CompositeTransform & ParamTransform::Transform       ()
{
    return m_transformModel;
}

// *******************************
//
inline  glm::mat4   ParamTransform::Evaluate        () const
{
    auto t = AbstractModelParameter::GetLocalEvaluationTime();

    return m_transformModel.Evaluate( t );
}

// *************************************
//
inline ModelParamType ParamTransform::Type          ()
{
    return ModelParamType::MPT_TRANSFORM;
}

} //model
} //bv
