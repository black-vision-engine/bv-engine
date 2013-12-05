#pragma once

#include <glm/glm.hpp>
#include <hash_map>

#include "Engine/Graphics/Resources/Texture.h"

#include "Engine/Graphics/Renderers/WGLRenderer/WGLRendererInput.h"
#include "Engine/Graphics/Renderers/OGLRenderer/GLRendererData.h"

#include "Engine/Graphics/State/StateInstance.h"

namespace bv {

class Camera;

class RenderableEntity;
class TriangleStrip;

class PixelShader;
class VertexShader;
class GeometryShader;
class RenderablePass;
class Texture2D;
class TextureAnimatedSequence2D;

class VertexBuffer;
class IndexBuffer;
class VertexDescriptor;
class VertexArray;
class VertexArraySingleVertexBuffer;

class PdrTexture2D;
class PdrTextureAnimatedSequence2D;
class PdrVertexBuffer;
class PdrShader;
class PdrIndexBuffer;
class PdrVertexArrayObject;
class PdrVertexArrayObjectSingleVB;
class PdrVertexDescriptor;
class PdrVertexArrayObject;

class TransformableEntity;

enum class FaceKind : int;

//FIXME: add disable methods so that current state can be cleared after frame is rendered
class Renderer
{
private:

    StateInstance       m_currentStateInstance;
    StateInstance       m_defaultStateInstance;

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
    typedef std::hash_map<const TextureAnimatedSequence2D *, PdrTextureAnimatedSequence2D * >   PdrTextureAnimatedSequence2DMap;

    PdrShaderMapType                    m_PdrShaderMap;
    PdrVertexBufferMapType              m_PdrVertexBufferMap;
    PdrIndexBufferMapType               m_PdrIndexBufferMap;
    PdrVertexDescriptorType             m_PdrVertexDescriptorMap;
    PdrVertexArrayObjectMapType         m_PdrVertexArrayObjectMap;
    PdrTexture2DMap                     m_PdrTextures2DMap;
    PdrTextureAnimatedSequence2DMap     m_PdrTexturesAnimatedSequence2DMap;
    PdrVertexArrayObjectSingleVBMapType m_PdrVertexArrayObjectSingleVBMap;

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

    void    NaiveReadback       ( char * buf, int w, int h );

    void    SetCamera           ( Camera * cam );

public:

    void    SetStateInstance    ( const StateInstance & stateInstance );

public: //FIXME: private

    void    Enable              ( RenderablePass * pass, TransformableEntity * transform );
    void    Enable              ( const VertexBuffer * vb );
    void    Enable              ( const IndexBuffer * ib );
    void    Enable              ( const VertexArray * vao );
    void    Enable              ( const VertexArraySingleVertexBuffer * vao );

    void    Disable             ( const VertexArraySingleVertexBuffer * vao );
    void    Disable             ( const VertexBuffer * vb );

    void    Update              ( const VertexBuffer * vb );
    void    Recreate            ( const VertexBuffer * vb );

public:

    void    Enable              ( const Texture2D * texture, int textureUnit );
    void    Enable              ( const TextureAnimatedSequence2D * texture, int textureUnit );

    //FIXME: add disable methods so that current state can be cleared after frame is rendered
    void    Disable             ( const Texture2D * texture, int textureUnit );
    void    Disable             ( const TextureAnimatedSequence2D * texture, int textureUnit );

public:

    PdrVertexBuffer *               GetPdrVertexBuffer          ( const VertexBuffer * vb );
    PdrIndexBuffer *                GetPdrIndexBuffer           ( const IndexBuffer * ib );
    PdrVertexDescriptor *           GetPdrVertexDescriptor      ( const VertexDescriptor * vd );
    PdrVertexArrayObject *          GetPdrVertexArray           ( const VertexArray * vao );
    PdrVertexArrayObjectSingleVB *  GetPdrVertexArraySingleVB   ( const VertexArraySingleVertexBuffer * vao );
    PdrTexture2D *                  GetPdrTexture2D             ( const Texture2D * texture );
    PdrTextureAnimatedSequence2D *  GetPdrTextureAnimSeq2D      ( const TextureAnimatedSequence2D * texture );

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
