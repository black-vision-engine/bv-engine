#pragma once

#include <hash_map>
#include <atomic>

#include "Interfaces/IVideoCard.h"
#include "Interfaces/IVideoCardDescriptor.h"
#include "VideoCardManagerUtils.h"

#include "Memory/MemoryChunk.h"
#include "DataTypes/CircularBufferConcurrent.h"
#include "Threading/Thread.h"
#include "AVFrame.h"

#include "Serialization/IDeserializer.h"



namespace bv { namespace videocards {


enum class DisplayMode : int
{
    SD,
	SD_4_3,
    HD

};


std::vector< IVideoCardDesc * >  DefaultVideoCardDescriptors  ();


struct InputConfig
{
    std::string type;
    bool playthrough;
};


struct OutputConfig
{   
    std::string type;
    std::string referenceMode;
    Int32 refH;
    Int32 refV;
    UInt32 resolution;
    UInt32 refresh;
    bool interlaced;
    bool flipped;
};


class VideoCardManager;


// ******************************
class VideoCardProcessingThread : public Thread
{
private:

    std::atomic< bool >     m_running;

public:

                        VideoCardProcessingThread   ();
    virtual				~VideoCardProcessingThread  ();
    
protected:

    virtual void		Run                         () override;

};

DEFINE_UPTR_TYPE( VideoCardProcessingThread )


// ******************************
class VideoCardManager
{
private:

    static AVFramePtr                  KILLER_FRAME;
	char* m_PreviousFrame;

    std::hash_map< std::string, const IVideoCardDesc * >                m_descMap;
    std::vector< const IVideoCardDesc * >                               m_descVec;

    std::vector< IVideoCardPtr >                                        m_videoCards;

    /**@brief Circular blocking frame queue */
    CircularBufferConcurrent< AVFramePtr, FRAME_BUFFER_SIZE >  m_frameBuffer;

	bool                                m_keyActive;
	bool                                m_InterlaceProducesFullFrames;

    //FIXME: probably not needed
    bool                                m_interlaceEnabled;

    DisplayMode                         m_dislpayMode;

    mutable std::atomic< UInt64 >       m_numReadyCards;
    mutable std::condition_variable     m_waitFramesProcessed;
    mutable std::mutex			        m_mutex;
	Int32                               m_currentFrameNumber;

    //FIXME: probably not needed
    /**@brief Frame processing thread (copies frames, interlacing, etc.) */
    VideoCardProcessingThreadUPtr       m_processThread;

private:
    static void                         FrameProcessingCompleted( UInt64 deviceID, bool success );


                                        VideoCardManager        ();
                                        ~VideoCardManager       ();

public:
    
    void                                ReadConfig              ( const IDeserializer & deser );
    void                                RegisterDescriptors     ( const std::vector< IVideoCardDesc * > & descriptors );

    bool                                IsRegistered            ( const std::string & uid ) const;

    /**@brief Enables/disabled videooutput. */
    void                                SetVideoOutput          ( bool enable );
    void                                SetKey                  ( bool active );

    void                                Start                   ();

    void                                QueueFrame              ( AVFramePtr data );
    
    /**@brief Runs in processing thread. Can be stopped by queueing KILLER_FRAME.
    @return Returns true if processed correct frame, false for KILLER_FRAME. */
    bool                                ProcessFrame            ();
    
    //FIXME: probably not needed
    /**@brief Copies and interlaces full frame.
    @return Returns interlaced copy of frame. */
	AVFramePtr                 InterlacedFrame(AVFramePtr data);
	AVFramePtr                 RetrieveFieldFromFrame(AVFramePtr data, int odd);

    IVideoCardPtr                       GetVideoCard            ( UInt32 idx );


    static VideoCardManager &           Instance                ();

};

} //videocards
} //bv