#pragma once

#include "win_sock.h"
#include <vector>
#include "VideoCardBase.h"
#include "VideoMidgard.h"

#include <hash_map>
#include <string>

#include "Interfaces/IVideoCard.h"
#include "Interfaces/IVideoCardDescriptor.h"
#include "Memory/MemoryChunk.h"
#include "DataTypes/QueueConcurrent.h"

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


//struct ChannelConfig
//{
//    string name;
//    unsigned short renderer;
//    bool playback;
//    bool capture;
//    OutputConfig m_outputConfig;
//    InputConfig inputConfig;
//
//    ChannelConfig()
//    {
//        m_outputConfig = OutputConfig();
//        inputConfig = InputConfig();
//    }
//};
//
//
//struct VideoCardConfig
//{
//    string name;
//    string type;
//    unsigned int channelCount;
//    vector<ChannelConfig>   channelConfigVector;
//};


//struct VideoConfig
//{
//    bool ReadbackFlag;
//    bool superMagic;
//    string resolutionOld;
//    string transferMode;
//    unsigned int blueFishCount;
//    unsigned int blackMagicCount;
//    vector<VideoCardConfig> m_BlueFishConfig;
//    vector<VideoCardConfig> m_BlackMagicConfig;
//};


class VideoCardManager
{
private:

    static const UInt32         FRAMES_COUNT;
    static const UInt32         FHD;
    static const UInt32         WIDTH_BYTES;

private:

    std::hash_map< std::string, const IVideoCardDesc * >    m_descMap;
    std::vector< const IVideoCardDesc * >                   m_descVec;

    std::vector< IVideoCardPtr >            m_videoCards;

    QueueConcurrent< MemoryChunkConstPtr >  m_dataQueue;

    //ProcessingThreadUPtr                    m_processingThread;

    bool                                    m_enabled;
    bool                                    m_enableInterlace;
    bool                                    m_keyActive;

    DisplayMode                             m_dislpayMode;

private:

                                        VideoCardManager        ();
                                        ~VideoCardManager       ();

public:
    
    void                                ReadConfig              ( const IDeserializer & deser );
    void                                RegisterDescriptors     ( const std::vector< IVideoCardDesc * > & descriptors );

    bool                                IsRegistered            ( const std::string & uid ) const;
    bool                                IsEnabled               () const;

    void                                SetKey                  ( bool active );


    void                                Start                   ();

    void                                ProcessBuffer           ();

    IVideoCardPtr                       GetVideoCard            ( UInt32 idx );


    static VideoCardManager &           Instance                ();

private:

    //vector< VideoCardBase * >   m_VideoCards;
    //VideoMidgard *                m_Midgard;
    //HANDLE                        m_midgardThreadHandle;
    //unsigned int              m_midgardThreadID;
    //bool                      m_midgardThreadStopping;

    
public:
    
    //bool                    m_SuperMagic;
    //VideoConfig             m_VideoCardConfig;
    //VideoCard_RAM_GPU       m_CurrentTransferMode;
    


    //void                    Enable                  ();
    //void                    Disable                 ();
    //void                    StopVideoCards            ();
    //void                    SuspendVideoCards     ();
    //void                    ResumeVideoCards      ();
    //size_t                  GetVideoCardsSize       ();
    
    void                    PushDataFromRenderer    ( MemoryChunkConstPtr data );

    unsigned char *         GetCaptureBufferForShaderProccessing    (unsigned int VideCardID, std::string ChannelName/*A,B,C,D,E,F*/);    
    bool                    CheckIfNewFrameArrived                  (unsigned int VideCardID, std::string ChannelName/*A,B,C,D,E,F*/);    
    //void                    UnblockCaptureQueue                     (unsigned int VideCardID, std::string ChannelName/*A,B,C,D,E,F*/);
    bool                    UpdateReferenceMode     (unsigned int VideoCardID, std::string ChannelName/*A,B,C,D,E,F*/, std::string ReferenceModeName/*FREERUN,IN_A,IN_B,ANALOG,GENLOCK*/ );
    bool                    UpdateReferenceOffset   (unsigned int VideoCardID, std::string ChannelName/*A,B,C,D,E,F*/, int refH, int refV);

private:

    //void                    DetectVideoCards        ();
    //void                    DisableVideoCard        (int i);
    //void                    DisableVideoCards       ();
    //void                    EnableVideoCard         (int i);
    //void                    EnableVideoCards        ();
    //void                    SetupVideoChannels      ();
    //bool                    InitVideoCard           ( int i, const std::vector<int> & hackBuffersUids );
    //bool                    InitVideoCards          ( const std::vector<int> & hackBuffersUids );
    //void                    RegisterVideoCards      ();
/*    void                    RegisterBlueFishCards   ();
    void                    RegisterBlackMagicCards ();   */ 
    //unsigned int static __stdcall copy_buffer_thread      (void *args);

};

} //videocards
} //bv