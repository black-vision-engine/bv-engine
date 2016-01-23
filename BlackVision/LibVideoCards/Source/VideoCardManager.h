#pragma once

#include "win_sock.h"
#include <vector>
#include "VideoCardBase.h"
#include "VideoMidgard.h"

#include "Engine/Events/Interfaces/IEventManager.h"
#include "Engine/Events/Events.h"
#include "Engine/Events/BaseEvent.h"


namespace bv
{

namespace videocards{
using namespace std;

struct InputConfig
{
    string type;
    bool playthrough;
};

struct OutputConfig
{   
    string type;
    string referenceMode;
    int refH;
    int refV;
    unsigned short resolution;
    unsigned short refresh;
    bool interlaced;
    bool flipped;
};

struct ChannelConfig
{
    string name;
    unsigned short renderer;
    bool playback;
    bool capture;
    OutputConfig m_outputConfig;
    InputConfig inputConfig;

    ChannelConfig()
    {
        m_outputConfig = OutputConfig();
        inputConfig = InputConfig();
    }
};
struct VideoCardConfig
{
    string name;
    string type;
    unsigned int channelCount;
    vector<ChannelConfig>	channelConfigVector;
};
struct VideoConfig
{
    bool ReadbackFlag;
    bool BlueFish;
    bool BlackMagic;
    bool superMagic;
    string resolutionOld;
    string transferMode;
    unsigned int blueFishCount;
    unsigned int blackMagicCount;
    vector<VideoCardConfig>	m_BlueFishConfig;
    vector<VideoCardConfig>	m_BlackMagicConfig;
};




class VideoCardManager
{
private:

    vector<VideoCardBase*>	m_VideoCards;
    VideoMidgard*			m_Midgard;
	HANDLE					m_midgardThreadHandle;
	unsigned int			m_midgardThreadID;
	bool					m_midgardThreadStopping;
	bool					m_Enabled;
	bool					m_key_active;

    
public:
	
    bool                    m_SuperMagic;
    VideoConfig             m_VideoCardConfig;
    VideoCard_Modes         m_CurrentDislpayMode;
    VideoCard_RAM_GPU       m_CurrentTransferMode;
    
    bool                    m_IsEnding;
	void					Black();
	void					SetKey(bool active){m_key_active=active;};

	                        VideoCardManager        (void);
	                        ~VideoCardManager       (void);

    bool                    InitVideoCardManager    (const std::vector<int> & hackBuffersUids);
    void                    DestroyVideoCardManager ();
    void                    ReadConfig              ();
    void                    Enable                  ();
    void                    Disable                 ();
    void                    StartVideoCards         ();
    void                    StopVideoCards			();
    void                    SuspendVideoCards		();
    void                    ResumeVideoCards		();
    VideoCardBase*          GetVideoCard			(int i);
    size_t		            GetVideoCardsSize		();
    void                    OnEventReceived         (bv::IEventPtr evt);
	void                    DeliverFrameFromRAM     (unsigned char * buffer);
	void                    DeliverFrameFromRAM     (std::shared_ptr<CFrame> buffer);
	VideoMidgard *          GetMidgard				();
	void					GetBufferFromRenderer	(Texture2DConstPtr buffer);
	unsigned char *			GetCaptureBufferForShaderProccessing    (unsigned int VideCardID, std::string ChannelName/*A,B,C,D,E,F*/);    
    bool	                CheckIfNewFrameArrived                  (unsigned int VideCardID, std::string ChannelName/*A,B,C,D,E,F*/);    
    void	                UnblockCaptureQueue                     (unsigned int VideCardID, std::string ChannelName/*A,B,C,D,E,F*/);
	bool					UpdateReferenceMode		(unsigned int VideoCardID, std::string ChannelName/*A,B,C,D,E,F*/, std::string ReferenceModeName/*FREERUN,IN_A,IN_B,ANALOG,GENLOCK*/ );
	bool					UpdateReferenceOffset	(unsigned int VideoCardID, std::string ChannelName/*A,B,C,D,E,F*/, int refH, int refV);
    bool                    IsEnabled               (){return m_Enabled;}

private:
	void                    DetectVideoCards        ();
	void                    DisableVideoCard        (int i);
    void                    DisableVideoCards       ();
	void                    EnableVideoCard         (int i);
    void                    EnableVideoCards        ();
    void                    SetupVideoChannels      ();
	bool                    InitVideoCard           ( int i, const std::vector<int> & hackBuffersUids );
    bool                    InitVideoCards          ( const std::vector<int> & hackBuffersUids );
	void                    RegisterVideoCards      ();
    void                    RegisterBlueFishCards   ();
    void                    RegisterBlackMagicCards ();    
    bool                    StopMidgardThread       ();
	unsigned int static __stdcall copy_buffer_thread      (void *args);
};


}
}