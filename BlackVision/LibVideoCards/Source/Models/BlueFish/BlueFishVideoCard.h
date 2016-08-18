#pragma once

#include "Channel.h"
#include "BVGL.h"
#include <map>

//FIXME
#include "../../Interfaces/IVideoCard.h"
#include "../../Interfaces/IVideoCardDescriptor.h"


namespace bv { namespace videocards { namespace bluefish {

DEFINE_PTR_TYPE( CBlueVelvet4 )

// ***************************** DESCRIPTOR **********************************
//
class BlueFishVideoCardDesc : public IVideoCardDesc
{
private:

    std::string     m_uid;
    UInt32          m_deviceID;

public:

                                            BlueFishVideoCardDesc   ( UInt32 deviceID );

    virtual IVideoCardPtr                   CreateVideoCard         () const override;

    virtual const std::string &             GetVideoCardUID         () const override;

    void                                    ReadConfig              ();

};

class BlueFishVideoCard : IVideoCard
{
private:

	CBlueVelvet4Ptr         m_SDK;
	UInt32                  m_deviceID;

	UInt32                  m_updFmt;
	UInt32                  m_memFmt;
	UInt32                  m_engineMode;


	UInt32                  m_resolution;
	UInt32                  m_refresh;
    bool                    m_interlaced;
    bool                    m_flipped;


	VARIANT                 varVal;
    ULONG                   GoldenSize;
	ULONG					PixelsPerLine;
	ULONG					VideoLines;
    ULONG                   FieldCount;

	std::map< ChannelName, ChannelOption > m_channelOptions;

	std::vector< Channel* >	m_channels;

public:
	                    BlueFishVideoCard               ( UInt32 deviceID );
	virtual             ~BlueFishVideoCard              () override; 



    static Int32        EnumerateDevices                ();

	bool                InitVideoCard                   ();

    void                EnableVideoOutput               ();
    void                DisableVideoOutput              ();


    virtual unsigned int DetectInputs                   () override;      
    virtual unsigned int DetectOutputs                  () override;    
	void                BailOut                         (CBlueVelvet4* pSDK);
	void                RouteChannel                    (CBlueVelvet4* pSDK, ULONG Source, ULONG Destination, ULONG LinkType);
	void                InitBuffer                      (BLUE_UINT8* pVideoBuffer, ULONG PixelsPerLine, ULONG VideoLines);
	void                InitOutputChannel               (CBlueVelvet4* pSDK, ULONG DefaultOutputChannel, ULONG VideoMode, ULONG UpdateFormat, ULONG MemoryFormat, ULONG VideoEngine);
	int                 InitSDK                         ();
	bool                Init                            ();
    void                DeliverFrameFromRAM             (unsigned char * );
	void                DeliverFrameFromRAM             (std::shared_ptr<CFrame> Frame );
    bool                DeactivateVideoCard             ();
    void                Black                           ();
    void                SetReferenceModeValue           (std::string refMode);
    void                UpdateReferenceOffset           ();
    void                UpdateReferenceMode             ();      
    void                StartDuplexPlayback             ();
	int					InitDuplexPlayback		        ();
	void				AddChannel						( std::string name, std::string type, unsigned short renderer, unsigned short resolution, unsigned short refresh, bool interlaced, bool flipped, bool playback, bool capture, bool playthrough, std::string inputType, std::string referenceMode, int refH, int refV );
	Channel*			GetChannelByName				( std::string Name );   
    void                StartVideoCardProccessing                   ();
    //void              StopVideoCardProccessing                    ();
    void                SuspendVideoCardProccessing                ();
    void                ResumeVideoCardProccessing                  ();
	unsigned char *		GetCaptureBufferForShaderProccessing    (std::string ChannelName/*A,B,C,D,E,F*/);
    bool	            CheckIfNewFrameArrived                  (std::string ChannelName/*A,B,C,D,E,F*/);
    void	            UnblockCaptureQueue                     (std::string ChannelName/*A,B,C,D,E,F*/);
	virtual bool        UpdateReferenceOffset  (std::string ChannelName/*A,B,C,D,E,F*/, int refH, int refV)		override;
    virtual bool        UpdateReferenceMode    (std::string ChannelName/*A,B,C,D,E,F*/, std::string ReferenceModeName/*FREERUN,IN_A,IN_B,ANALOG,GENLOCK*/)	override;

//    ///GPUDirect
//private:    
//    GLuint                  frameBufferTest;
//    GLuint                  depth_rb;
//    BLUE_GPUDIRECT_HANDLE   pGpuDirectOut;
//    unsigned int*           g_pAudioScratch;
//    BLUE_UINT8*             g_pHancFrame;
//    BLUE_UINT8*             g_pVancData;
//    unsigned int            nCardType;
//	unsigned long           BufferSelect;
//	unsigned long           TextureSelect;
//	unsigned long           DMABufferIdVideo;
//	unsigned long           DMABufferIdHanc;
//	unsigned long           DMABufferIdVanc;
//	unsigned int            nOglTexID;
//	int                     nVancBufferSize;
//    BOOL                    bUseAudio;
//	BOOL                    bUseVanc;
//    GLuint                  glFrameTexOut;
//	BOOL                    bDoHanc;
//	void*                   pAddress;
//	unsigned long           ulUnderrun;
//	unsigned long           ulUniqueId;
//    int                     iFramesToBuffer;
//    bool                    wait;
//    std::vector<int>        glOutputBuffersHack;
//
//public:
//    void                processAudioOutput              (unsigned int nCardType, void* pAudioOut, unsigned int VideoMode);
//    void                Fill50                          (unsigned short* pAudio16, unsigned int Samples, unsigned int Channels);
//    void                AdjustProcessWorkingSet         ();
//	int					InitSDKGPUDirect				( const std::vector<int> & hackBuffersUids );
//	BOOL				IsInputChannel					(EBlueVideoChannel VideoChannel);
//	BOOL				IsOutputChannel					(EBlueVideoChannel VideoChannel);
//	void        		InitBluefish					(CBlueVelvet4* tempSDK, int CardId, EBlueVideoChannel VideoChannel, EVideoMode VideoMode, EUpdateMethod UpdFmt, EMemoryFormat MemoryFormat, EEngineMode VideoEngine);
//    bool                DirectGPUPreRender              ();
//    bool                DirectGPUPostRender             ();
//    void                DeliverFrameFromGPU             (unsigned int bufferPointer);
//    CBlueVelvet4*       GetBlueFishSDK                  ();
	
};

DEFINE_PTR_TYPE( BlueFishVideoCard )

} //bluefish
} //videocards
} //bv
