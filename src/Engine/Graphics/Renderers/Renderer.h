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

//FIXME: add disable methods so that current state can be cleared after frame is rendered
class Renderer
{
private:

    int					m_Width;
    int					m_Height;

    TextureFormat    m_ColorFormat;
    
    glm::vec4			m_ClearColor;
    float				m_ClearDepth;

    Camera *			m_Camera;

    RendererData *		m_RendererData;

    typedef std::hash_map<const RenderablePass*, PdrShader*>                PdrShaderMapType;
    typedef std::hash_map<const VertexBuffer*, PdrVertexBuffer*>            PdrVertexBufferMapType;
    typedef std::hash_map<const IndexBuffer*, PdrIndexBuffer*>              PdrIndexBufferMapType;
    typedef std::hash_map<const VertexDescriptor*, PdrVertexDescriptor*>    PdrVertexDescriptorType;
    typedef std::hash_map<const VertexArray*, PdrVertexArrayObject*>        PdrVertexArrayObjectMapType;
    typedef std::hash_map<const Texture2D *, PdrTexture2D * >               PdrTexture2DMap;

    PdrShaderMapType                m_PdrShaderMap;
    PdrVertexBufferMapType          m_PdrVertexBufferMap;
    PdrIndexBufferMapType           m_PdrIndexBufferMap;
    PdrVertexDescriptorType         m_PdrVertexDescriptorMap;
    PdrVertexArrayObjectMapType     m_PdrVertexArrayObjectMap;
    PdrTexture2DMap                 m_PdrTextures2DMap;

public:

    void	Initialize			( int w, int h, TextureFormat colorFormat );
    void	Terminate			();

            Renderer			( RendererInput & ri, int w, int h, TextureFormat colorFormat = TextureFormat::F_A8R8G8B8 );
            ~Renderer			();

    void	ClearBuffers		();
    void	SetClearColor		( const glm::vec4 & col );
    void	Resize				( int w, int h );
    bool    PreDraw             ();
    bool    Draw                ( RenderableEntity * ent );
    bool    PostDraw            ();

    void	DisplayColorBuffer	();
    void    SetFaceCulling      ( FaceKind face );
    void    DisableFaceCulling  ();
    void    EnableFaceCulling   ();
    void    SetCamera           (Camera* cam);

private:

    void    Enable              ( RenderablePass * pass, TransformableEntity * transform );
    void    Enable              ( const VertexBuffer * vb );
    void    Enable              ( const IndexBuffer * ib );
    void    Enable              ( const VertexArray * vao );

public:

    void    Enable              ( const Texture2D * texture, int textureUnit );

    //FIXME: add disable methods so that current state can be cleared after frame is rendered
    void    Disable             ( const Texture2D * texture, int textureUnit );

public:

    PdrVertexBuffer *           GetPdrVertexBuffer      ( const VertexBuffer * vb );
    PdrIndexBuffer *            GetPdrIndexBuffer       ( const IndexBuffer * ib );
    PdrVertexDescriptor *       GetPdrVertexDescriptor  ( const VertexDescriptor * vd );
    PdrVertexArrayObject *      GetPdrVertexArray       ( const VertexArray * vao );
    PdrTexture2D *              GetPdrTexture2D         ( const Texture2D * texture );

    bool                        DrawRenderable          ( RenderableEntity * ent );

};
}
