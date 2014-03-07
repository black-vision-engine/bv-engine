namespace bv {

// *********************************
//
inline int              RenderableEffect::NumPasses () const
{
    return m_passes.size();
}

// *********************************
//
inline RenderablePass * RenderableEffect::GetPass   ( int index )
{
    assert( index >= 0 );
    assert( index < NumPasses() );

    return m_passes[ index ];
}

} //bv
