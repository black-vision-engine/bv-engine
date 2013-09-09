#pragma once

#include <glm/glm.hpp>
#include <hash_map>

#include "Texture.h"

#include "WGLRendererInput.h"
#include "WGLRendererData.h"

namespace bv {

class Camera;
class RenderableEntity;
class PixelShader;
class VertexShader;
class GeometryShader;
class PdrShader;
class VertexBuffer;
class PdrVertexBuffer;
class IndexBuffer;
class PdrIndexBuffer;
class VertexDescriptor;
class PdrVertexArrayObject;
class RenderablePass;
class PdrVertexDescriptor;
class TransformableEntity;
enum class FaceKind : int;

class Renderer
{
private:

    int					m_Width;
    int					m_Height;

    Texture::TFormat    m_ColorFormat;
    
    glm::vec4			m_ClearColor;
    float				m_ClearDepth;

    Camera *			m_Camera;

    RendererData *		m_RendererData;

    typedef std::hash_map<RenderablePass*, PdrShader*>                  PdrShaderMapType;
    typedef std::hash_map<VertexBuffer*, PdrVertexBuffer*>              PdrVertexBufferMapType;
    typedef std::hash_map<IndexBuffer*, PdrIndexBuffer*>                PdrIndexBufferMapType;
    typedef std::hash_map<VertexDescriptor*, PdrVertexDescriptor*>      PdrVertexDescriptorType;
    typedef std::hash_map<VertexDescriptor*, PdrVertexArrayObject*>     PdrVertexArrayObjectMapType;

    PdrShaderMapType                m_PdrShaderMap;
    PdrVertexBufferMapType          m_PdrVertexBufferMap;
    PdrIndexBufferMapType           m_PdrIndexBufferMap;
    PdrVertexDescriptorType         m_PdrVertexDescriptorMap;
    PdrVertexArrayObjectMapType     m_PdrVertexArrayObjectMap;


public:

    void	Initialize			( int w, int h, Texture::TFormat colorFormat );
    void	Terminate			();

            Renderer			( RendererInput & ri, int w, int h, Texture::TFormat colorFormat = Texture::TFormat::F_A8R8G8B8 );
            ~Renderer			();

    void	ClearBuffers		();
    void	SetClearColor		( const glm::vec4 & col );
    void	Resize				( int w, int h );
    bool    PreDraw             ();
    bool    Draw                (RenderableEntity* ent);
    bool    PostDraw            ();

    void	DisplayColorBuffer	();
    void    SetFaceCulling      ( FaceKind face );
    void    DisableFaceCulling  ();
    void    EnableFaceCulling   ();
    void    SetCamera           (Camera* cam);

	int		VertexCount;

private:
    void    Enable              ( RenderablePass* pass, TransformableEntity* transform );
    void    Enable              ( VertexBuffer* vb );
    void    Enable              ( IndexBuffer* ib );
    void    Enable              ( VertexBuffer* vb, VertexDescriptor* vd );

    PdrVertexDescriptor*        GetPdrVertexDescriptor  ( VertexDescriptor* vd  );

    bool                        DrawRenderable          ( RenderableEntity* ent );
};
}
