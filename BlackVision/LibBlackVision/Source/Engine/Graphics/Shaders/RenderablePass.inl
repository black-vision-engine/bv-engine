// *********************************************************** SHIMS ***********************************************************

namespace bv {

// *********************************
// FIXME: works for pass 0 only
inline PixelShader *            GetPixelShader          ( RenderablePass * pass )
{
    return pass->GetPixelShader();
}

// *********************************
// FIXME: works for pass 0 only
inline VertexShader *           GetVertexShader         ( RenderablePass * pass )
{
    return pass->GetVertexShader();
}

// *********************************
// FIXME: works for pass 0 only
inline GeometryShader *         GetGeometryShader       ( RenderablePass * pass )
{
    return pass->GetGeometryShader();
}

// *********************************
// FIXME: works for pass 0 only
inline RendererStateInstance *  GetRenderStateInstance  ( RenderablePass * pass )
{
    return pass->GetStateInstance();
}

} //bv
