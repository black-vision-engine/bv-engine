namespace bv {

// *********************************
//
inline const Transform &   TransformableEntity::LocalTransform      () const
{
    return m_localTransform;
}

// *********************************
//
inline void                TransformableEntity::SetLocalTransform   ( const Transform & t )
{
    m_localTransform = t;
}

// *********************************
//
inline void                TransformableEntity::SetLocalTransform   ( Transform && t )
{
	m_localTransform = t;
}

// *********************************
//
inline const Transform &  TransformableEntity::WorldTransform       () const
{
    return m_worldTransform;
}

    
// *********************************
//
inline void  TransformableEntity::SetWorldTransform                 ( const Transform & transform )
{
    m_worldTransform = transform;
}

// *********************************
//
inline void   TransformableEntity::ResetLocalTransform              ()
{
    m_localTransform.Reset();
}

// *********************************
//
inline void   TransformableEntity::ResetWorldTransform              ()
{
    m_worldTransform.Reset();
}

} //bv
