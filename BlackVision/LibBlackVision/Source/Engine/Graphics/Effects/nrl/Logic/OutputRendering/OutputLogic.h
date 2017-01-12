#pragma once


namespace bv { 
    
class Renderer;    
class SharedMemoryVideoBuffer;

namespace nrl {

class RenderResult;
class Preview;
class VideoOutput;
class NRenderContext;

class OutputLogic
{
private:

    // FIXME: nrl - any additional flags?
    bool            m_videoOutputEnabled;
    bool            m_useSharedMemory;

    Preview *                   m_preview;
    SharedMemoryVideoBuffer *   m_sharedMemoryVideoBuffer;
    VideoOutput *               m_videoOutput;

public:

                    OutputLogic         ( unsigned int width, unsigned int height, unsigned int sharedMemScaleFactor );
                    ~OutputLogic        ();

    void            ProcessFrameData    ( NRenderContext * ctx, const RenderResult * data, unsigned int numScenes );
    
    Preview *       GetPreview          ();
    VideoOutput *   GetVideoOutput      ();

};

} //nrl
} //bv
