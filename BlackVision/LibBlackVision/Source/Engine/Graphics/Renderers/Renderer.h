#pragma once

#include <glm/glm.hpp>
#include <hash_map>

#include "Engine/Graphics/Resources/Texture2D.h"

#include "Engine/Graphics/Renderers/WGLRenderer/WGLRendererInput.h"
#include "Engine/Graphics/Renderers/OGLRenderer/GLRendererData.h"

#include "Engine/Graphics/State/RendererStateInstance.h"

namespace bv {

class Camera;

class RenderableEntity;
class TriangleStrip;

class PixelShader;
class VertexShader;
class GeometryShader;
class RenderablePass;
class RenderTarget;

class VertexBuffer;
class IndexBuffer;
class VertexDescriptor;
class VertexArray;
class VertexArraySingleVertexBuffer;

class PdrTexture2D;
class PdrVertexBuffer;
class PdrShader;
class PdrIndexBuffer;
class PdrVertexArrayObject;
class PdrVertexArrayObjectSingleVB;
class PdrVertexDescriptor;
class PdrVertexArrayObject;
class PdrRenderTarget;
class PdrPBOMemTransfer;

class TransformableEntity;

enum class FaceKind : int;

//FIXME: add disable methods so that current state can be cleared after frame is rendered
class Renderer
{
private:

    RendererStateInstance       m_currentStateInstance;
    RendererStateInstance       m_defaultStateInstance;

    int					m_Width;
    int					m_Height;

    TextureFormat       m_ColorFormat;

    glm::vec4			m_ClearColor;
    float				m_ClearDepth;

    Camera *			m_Camera;

    RendererData *		m_RendererData;

    typedef std::hash_map<const RenderablePass*, PdrShader*>                                    PdrShaderMapType;
    typedef std::hash_map<const VertexBuffer*, PdrVertexBuffer*>                                PdrVertexBufferMapType;
    typedef std::hash_map<const IndexBuffer*, PdrIndexBuffer*>                                  PdrIndexBufferMapType;
    typedef std::hash_map<const VertexDescriptor*, PdrVertexDescriptor*>                        PdrVertexDescriptorType;
    typedef std::hash_map<const VertexArray*, PdrVertexArrayObject*>                            PdrVertexArrayObjectMapType;
    typedef std::hash_map<const VertexArraySingleVertexBuffer*, PdrVertexArrayObjectSingleVB*>  PdrVertexArrayObjectSingleVBMapType;
    typedef std::hash_map<const Texture2D *, PdrTexture2D * >                                   PdrTexture2DMap;
    typedef std::hash_map<const RenderTarget *, PdrRenderTarget * >                             PdrRenderTargetMap;

    PdrShaderMapType                    m_PdrShaderMap;
    PdrVertexBufferMapType              m_PdrVertexBufferMap;
    PdrIndexBufferMapType               m_PdrIndexBufferMap;
    PdrVertexDescriptorType             m_PdrVertexDescriptorMap;
    PdrVertexArrayObjectMapType         m_PdrVertexArrayObjectMap;
    PdrTexture2DMap                     m_PdrTextures2DMap;
    PdrVertexArrayObjectSingleVBMapType m_PdrVertexArrayObjectSingleVBMap;
    PdrRenderTargetMap                  m_PdrRenderTargetMap;

    PdrPBOMemTransfer *                 m_PdrPBOMemTransferRT;

public:

    void	Initialize			( int w, int h, TextureFormat colorFormat );
    void	Terminate			();

            Renderer			( RendererInput & ri, int w, int h, TextureFormat colorFormat = TextureFormat::F_A8R8G8B8 );
            ~Renderer			();

    void	ClearBuffers		();
    void	SetClearColor		( const glm::vec4 & col );
    void	Resize				( unsigned int w, unsigned int h );
    bool    PreDraw             ();
    bool    Draw                ( RenderableEntity * ent );
    bool    PostDraw            ();

    int     GetWidth            () const;
    int     GetHeight           () const;

    void	DisplayColorBuffer	();

    void    NaiveReadback       ( char * buf, int w, int h );

    void    SetCamera           ( Camera * cam );

public:

    void    SetStateInstance    ( const RendererStateInstance & stateInstance );

public: //FIXME: private

    void    Enable              ( RenderablePass * pass, RenderableEntity * renderable );
    void    Enable              ( const VertexBuffer * vb );
    void    Enable              ( const IndexBuffer * ib );
    void    Enable              ( const VertexArray * vao );
    void    Enable              ( const VertexArraySingleVertexBuffer * vao );

    void    Disable             ( const VertexArraySingleVertexBuffer * vao );
    void    Disable             ( const VertexBuffer * vb );

    void    Update              ( const VertexBuffer * vb );
    void    Recreate            ( const VertexBuffer * vb );

    bool    IsRegistered        ( const Texture2D * texture );
    void    RegisterTexture2D   ( const Texture2D * texture, PdrTexture2D * pdrTexture );

public:

    void    Enable              ( const Texture2D * texture, int textureUnit );

    //FIXME: add disable methods so that current state can be cleared after frame is rendered
    void    Disable             ( const Texture2D * texture, int textureUnit );

    void    Enable              ( const RenderTarget * rt );
    void    Disable             ( const RenderTarget * rt );

    void    ReadColorTexture    ( unsigned int i, const RenderTarget * rt, Texture2DPtr & outputTex );

public:

    PdrVertexBuffer *               GetPdrVertexBuffer          ( const VertexBuffer * vb );
    PdrIndexBuffer *                GetPdrIndexBuffer           ( const IndexBuffer * ib );
    PdrVertexDescriptor *           GetPdrVertexDescriptor      ( const VertexDescriptor * vd );
    PdrVertexArrayObject *          GetPdrVertexArray           ( const VertexArray * vao );
    PdrVertexArrayObjectSingleVB *  GetPdrVertexArraySingleVB   ( const VertexArraySingleVertexBuffer * vao );
    PdrTexture2D *                  GetPdrTexture2D             ( const Texture2D * texture );
    PdrRenderTarget *               GetPdrRenderTarget          ( const RenderTarget * rt );

    bool                        DrawRenderable              ( RenderableEntity * ent );
    bool                        DrawTriangleStrips          ( TriangleStrip * strip );

private:

    void                        SetAlphaState               ( const AlphaState * as );
    void                        SetCullState                ( const CullState * cs );
    void                        SetDepthState               ( const DepthState * ds );
    void                        SetFillState                ( const FillState * fs );
    void                        SetOffsetState              ( const OffsetState * os );
    void                        SetStencilState             ( const StencilState * ss );

};

}