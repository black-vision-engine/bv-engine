#pragma once

#include <hash_map>
#include <vector>

#include "BVGL.h"

#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Core/MemoryChunk.h"


namespace bv {

class Renderer;
class Texture2D;

class PdrPBOMemTransfer;

class PdrRenderTarget
{
private:
    
    typedef std::hash_map< Texture2D * , MemoryChunkPtr >   TTex2DMCHashMap;
    typedef std::vector< TTex2DMCHashMap >                  TReadbackVec;

private:

    //bool          m_hasStencilBuffer; //FIXME: to be implemented

    unsigned int                    m_numTargets;

    unsigned int                    m_width;
    unsigned int                    m_height;

    bool                            m_hasDepthBuffer;

    std::vector< TextureFormat >    m_textureFormats;

    GLuint                          m_fboID;
    GLuint                          m_depthBufID;

    std::vector< GLuint >           m_textures;
    std::vector< GLenum >           m_drawBuffers;

    int                             m_prevViewportCoords[ 4 ];
    double                          m_prevDepthRange[ 2 ];

    TReadbackVec                    m_readbackBuffers;

public:

                    PdrRenderTarget     ( Renderer * renderer, const RenderTarget * rt );
                    ~PdrRenderTarget    ();
public:


    void            Enable              ( Renderer * renderer );
    void            Disable             ( Renderer * renderer );

    void            ReadColorTexture    ( unsigned int i, Renderer * renderer, PdrPBOMemTransfer * pboMem, Texture2DPtr & outputTex );

private:

    GLuint          GetPrevTexture      () const;

    void            AddColorAttachments ( Renderer * renderer, const RenderTarget * rt );
    void            AddDepthBuffer      ( Renderer * renderer, const RenderTarget * rt );
    bool            FramebuferStatusOK  () const;

};

} //bv
