#pragma once

#include <hash_map>
#include <atomic>

#include "Interfaces/IVideoCard.h"
#include "Interfaces/IVideoCardDescriptor.h"
#include "VideoCardManagerUtils.h"

#include "Memory/MemoryChunk.h"
#include "DataTypes/CircularBufferConcurrent.h"
#include "Threading/Thread.h"

#include "Serialization/IDeserializer.h"



namespace bv { namespace videocards {


enum class DisplayMode : int
{
    SD,
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

	int	m_currentFrameNumber;
	MemoryChunkPtr m_currentFrameData;
    static MemoryChunkConstPtr                  KILLER_FRAME;

    std::hash_map< std::string, const IVideoCardDesc * >                m_descMap;
    std::vector< const IVideoCardDesc * >                               m_descVec;

    std::vector< IVideoCardPtr >                                        m_videoCards;

    /**@brief Circular blocking frame queue */
    CircularBufferConcurrent< MemoryChunkConstPtr, FRAME_BUFFER_SIZE >  m_frameBuffer;

    bool                                m_keyActive;

    //FIXME: probably not needed
    bool                                m_interlaceEnabled;

    DisplayMode                         m_dislpayMode;

    mutable std::mutex			        m_mutex;

    //FIXME: probably not needed
    /**@brief Frame processing thread (copies frames, interlacing, etc.) */
    VideoCardProcessingThreadUPtr       m_processThread;

private:

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

    void                                QueueFrame              ( MemoryChunkConstPtr data );
    
    /**@brief Runs in processing thread. Can be stopped by queueing KILLER_FRAME.
    @return Returns true if processed correct frame, false for KILLER_FRAME. */
    bool                                ProcessFrame            ();
    
    //FIXME: probably not needed
    /**@brief Copies and interlaces full frame.
    @return Returns interlaced copy of frame. */
    MemoryChunkConstPtr                 InterlacedFrame         ( MemoryChunkConstPtr data );

    IVideoCardPtr                       GetVideoCard            ( UInt32 idx );


    static VideoCardManager &           Instance                ();

};

} //videocards
} //bv