#pragma once


namespace bv { 
    
class Renderer;    

namespace nrl {

class RenderResult;
class Preview;
class VideoOutput;
class NRenderContext;

class OutputLogic
{
private:

    Preview *       m_preview;
    VideoOutput *   m_videoOutput;

public:

                    OutputLogic         ();
                    ~OutputLogic        ();

    void            ProcessFrameData    ( NRenderContext * ctx, const RenderResult * data, unsigned int numScenes );
    
    Preview *       GetPreview          ();
    VideoOutput *   GetVideoOutput      ();

};

} //nrl
} //bv
