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
class RenderablePass;
class Texture2D;
class PdrTexture2D;

class VertexBuffer;
class IndexBuffer;
class VertexDescriptor;
class VertexArray;

class PdrVertexBuffer;
class PdrShader;
class PdrIndexBuffer;
class PdrVertexArrayObject;
class PdrVertexDescriptor;
class PdrVertexArrayObject;

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
    typedef std::hash_map<VertexArray*, PdrVertexArrayObject*>          PdrVertexArrayObjectMapType;
    typedef std::hash_map<Texture2D*, PdrTexture2D*>                    PdrTexture2DMap;

    PdrShaderMapType                m_PdrShaderMap;
    PdrVertexBufferMapType          m_PdrVertexBufferMap;
    PdrIndexBufferMapType           m_PdrIndexBufferMap;
    PdrVertexDescriptorType         m_PdrVertexDescriptorMap;
    PdrVertexArrayObjectMapType     m_PdrVertexArrayObjectMap;
    PdrTexture2DMap                 m_PdrTextures2DMap;

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

private:

    void    Enable              ( RenderablePass * pass, TransformableEntity * transform );
    void    Enable              ( VertexBuffer * vb );
    void    Enable              ( IndexBuffer * ib );
    void    Enable              ( VertexArray * vao );
    void    Enable              ( Texture2D * texture );

public:

    PdrVertexBuffer *           GetPdrVertexBuffer      ( VertexBuffer * vb );
    PdrIndexBuffer *            GetPdrIndexBuffer       ( IndexBuffer * ib );
    PdrVertexDescriptor *       GetPdrVertexDescriptor  ( VertexDescriptor * vd );
    PdrVertexArrayObject *      GetPdrVertexArray       ( VertexArray * vao );
    PdrTexture2D *              GetPdrTexture2D         ( Texture2D * texture );

    bool                        DrawRenderable          ( RenderableEntity* ent );
};
}
