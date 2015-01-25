namespace bv {

// *********************************
//
inline unsigned int     RenderableEffect::NumPasses () const
{
    return (unsigned int) m_passes.size();
}

// *********************************
//
inline RenderablePass * RenderableEffect::GetPass   ( unsigned int index )
{
    assert( index < NumPasses() );

    return m_passes[ index ];
}

} //bv
