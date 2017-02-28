#pragma once

#include <map>
#include <atomic>

#include "Interfaces/IVideoCard.h"
#include "Interfaces/IVideoCardDescriptor.h"
#include "VideoCardManagerUtils.h"

#include "Memory/MemoryChunk.h"
#include "DataTypes/CircularBufferConcurrent.h"
#include "Threading/Thread.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Video/Components/AVOutputsData.h"

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

class VCMInputData
{
	typedef std::map< UInt64, AVFrameConstPtr >			OutputsFramesMappingType;
	typedef OutputsFramesMappingType::const_iterator	const_iterator;

	OutputsFramesMappingType					m_outputsFramesMapping;

public:

	const_iterator	begin	() const	{ return m_outputsFramesMapping.begin(); };
	const_iterator	end		() const	{ return m_outputsFramesMapping.end(); };

	explicit		VCMInputData();

	void			SetAVFrame( UInt64 avOutputID, const AVFrameConstPtr & avFrame );
	AVFrameConstPtr	GetAVFrame( UInt64 avOutputID ) const;
};

DEFINE_CONST_PTR_TYPE( VCMInputData )

class VideoCardManager;


// ******************************
class VideoCardProcessingThread : public Thread
{
private:

    std::atomic< bool >     m_running;
    VideoCardManager *      m_vcm;

public:

    explicit            VideoCardProcessingThread   ( VideoCardManager * vcm );
    virtual				~VideoCardProcessingThread  ();
    
protected:

    virtual void		Run                         () override;

};

DEFINE_UPTR_TYPE( VideoCardProcessingThread )


// ******************************
class VideoCardManager
{
private:

    static VCMInputDataConstPtr											KILLER_FRAME;
	char* m_PreviousFrame;

    std::hash_map< std::string, const IVideoCardDesc * >                m_descMap;
    std::vector< const IVideoCardDesc * >                               m_descVec;

    std::vector< IVideoCardPtr >                                        m_videoCards;

	std::multimap< UInt64, IVideoCardPtr >								m_outputsToCardsMapping;

    /**@brief Circular blocking frames queue */
	CircularBufferConcurrent< VCMInputDataConstPtr, FRAME_BUFFER_SIZE >	m_inputDataBuffer;

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
    //static void                         FrameProcessingCompleted( UInt64 deviceID, bool success );

public:
    
    explicit                            VideoCardManager        ();
    virtual                             ~VideoCardManager       ();


    void                                ReadConfig              ( const IDeserializer & deser );
    void                                RegisterDescriptors     ( const std::vector< IVideoCardDesc * > & descriptors );

    bool                                IsRegistered            ( const std::string & uid ) const;

    /**@brief Enables/disabled videooutput. */
    void                                SetVideoOutput          ( bool enable );
    void                                SetKey                  ( bool active );

    void                                Start                   ();
    void                                Stop                    ();

	void								Display     			( const VCMInputDataConstPtr & outputs );
    
    /**@brief Runs in processing thread. Can be stopped by queueing KILLER_FRAME.
    @return Returns true if processed correct frame, false for KILLER_FRAME. */
    bool                                ProcessOutputsData      ();
   

    IVideoCardPtr                       GetVideoCard            ( UInt32 idx );


   // static VideoCardManager &           Instance                ();

};

} //videocards
} //bv
