#include "RenderableEffect.h"
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

// *********************************************************** SHIMS ***********************************************************

// *********************************
//
inline RenderablePass *         GetPass                 ( RenderableEffectPtr effect, unsigned int index )
{
    return effect->GetPass( index );
}


// *********************************
// FIXME: works for pass 0 only
inline PixelShader *            GetPixelShader          ( RenderableEffectPtr effect )
{
    return GetPixelShader( GetPass( effect, 0 ) );
}

// *********************************
// FIXME: works for pass 0 only
inline VertexShader *           GetVertexShader         ( RenderableEffectPtr effect )
{
    return GetVertexShader( GetPass( effect, 0 ) );
}

// *********************************
// FIXME: works for pass 0 only
inline GeometryShader *         GetGeometryShader       ( RenderableEffectPtr effect )
{
    return GetGeometryShader( GetPass( effect, 0 ) );
}

// *********************************
// FIXME: works for pass 0 only
inline RendererStateInstance *  GetRenderStateInstance  ( RenderableEffectPtr effect )
{
    return GetRenderStateInstance( GetPass( effect, 0 ) );
}

} //bv
