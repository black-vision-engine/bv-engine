// *********************************************************** SHIMS ***********************************************************

namespace bv {

// *********************************
//
inline RenderableEffectPtr      GetEffect               ( RenderableEntity * renderable )
{
    return renderable->GetRenderableEffect();
}

// *********************************
//
inline RenderablePass *         GetPass                 ( RenderableEntity * renderable, unsigned int index )
{
    return GetPass( GetEffect( renderable ), index );
}


// *********************************
// FIXME: works for pass 0 only
inline PixelShader *            GetPixelShader          ( RenderableEntity * renderable )
{
    return GetPixelShader( GetEffect( renderable ) );
}

// *********************************
// FIXME: works for pass 0 only
inline VertexShader *           GetVertexShader         ( RenderableEntity * renderable )
{
    return GetVertexShader( GetEffect( renderable ) );
}

// *********************************
// FIXME: works for pass 0 only
inline GeometryShader *         GetGeometryShader       ( RenderableEntity * renderable )
{
    return GetGeometryShader( GetEffect( renderable ) );
}

// *********************************
// FIXME: works for pass 0 only
inline RendererStateInstance *  GetRenderStateInstance  ( RenderableEntity * renderable )
{
    return GetRenderStateInstance( GetEffect( renderable ) );
}

} //bv
