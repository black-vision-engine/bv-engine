namespace bv {

// *********************************
//
inline const Transform &   TransformableEntity::LocalTransform                 () const
{
    return m_localTransform;
}

// *********************************
//
inline void                TransformableEntity::SetLocalTransform              ( const Transform & t )
{
    m_localTransform = t;
}

// *********************************
//
inline const std::vector< Transform > &  TransformableEntity::WorldTransforms  () const
{
    return m_worldTransforms;
}

    
// *********************************
//
inline void  TransformableEntity::SetWorldTransforms                           ( const std::vector< Transform > & transforms )
{
    m_worldTransforms = transforms;
}

// *********************************
//
inline void   TransformableEntity::ResetLocalTransform             ()
{
    m_localTransform.Reset();
}

// *********************************
//
inline void   TransformableEntity::ResetWorldTransforms            ()
{
    for( auto t : m_worldTransforms )
    {
        t.Reset();
    }
}

} //bv
