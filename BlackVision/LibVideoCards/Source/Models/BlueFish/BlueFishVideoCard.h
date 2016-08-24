#pragma once

#include "Interfaces/IVideoCard.h"
#include "Interfaces/IVideoCardDescriptor.h"

#include "Channel.h"


namespace bv { namespace videocards { namespace bluefish {

DEFINE_PTR_TYPE( CBlueVelvet4 )

// ***************************** DESCRIPTOR **********************************
//
class VideoCardDesc : public IVideoCardDesc
{
private:

    std::string     m_uid;

public:

                                            VideoCardDesc           ();

    virtual IVideoCardPtr                   CreateVideoCard         ( const IDeserializer & deser ) const override;

    virtual const std::string &             GetVideoCardUID         () const override;

};

class VideoCard : public IVideoCard
{
private:

    typedef std::map< ChannelName, ChannelOption > ChannelOptionMap;

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

	std::vector< Channel * > m_channels;

	static ChannelOptionMap ChannelOptions;

public:

	static UInt32           AvailableVideoCards;


public:
	                        VideoCard                   ( UInt32 deviceID );
	virtual                 ~VideoCard                  () override; 

	bool                    InitVideoCard               ();

    void                    SetVideoOutput              ( bool enable );

	void				    AddChannel					( Channel * channel );
	void                    RouteChannel                ( ULONG source, ULONG destination, ULONG linkType );
	Channel *			    GetChannelByName			( ChannelName channelName ) const;   

    virtual void            Start                       () override;

    virtual void            ProcessFrame                ( MemoryChunkConstPtr data ) override;

    static ChannelOptionMap CreateChannelOptionMap      ();
    static UInt32           EnumerateDevices            ();

	//void                BailOut                         (CBlueVelvet4* pSDK);
	//void                InitBuffer                      (BLUE_UINT8* pVideoBuffer, ULONG PixelsPerLine, ULONG VideoLines);
	//void                InitOutputChannel               (CBlueVelvet4* pSDK, ULONG DefaultOutputChannel, ULONG VideoMode, ULONG UpdateFormat, ULONG MemoryFormat, ULONG VideoEngine);
    //void                SetReferenceModeValue           (std::string refMode);
/*    void                UpdateReferenceOffset           ();
    void                UpdateReferenceMode             ();  */    
    //void                StartVideoCardProccessing                   ();
    //void              StopVideoCardProccessing                    ();
    //void                SuspendVideoCardProccessing                ();
    //void                ResumeVideoCardProccessing                  ();
	unsigned char *		GetCaptureBufferForShaderProccessing    ( ChannelName channelName );
    bool	            CheckIfNewFrameArrived                  ( ChannelName channelName );
    //void	            UnblockCaptureQueue                     ( ChannelName channelName );
	//virtual bool        UpdateReferenceOffset  (std::string ChannelName/*A,B,C,D,E,F*/, int refH, int refV)		override;
    //virtual bool        UpdateReferenceMode    (std::string ChannelName/*A,B,C,D,E,F*/, std::string ReferenceModeName/*FREERUN,IN_A,IN_B,ANALOG,GENLOCK*/)	override;

    //static UInt32       DetectInputs                   ();
    //static UInt32       DetectOutputs                  ();

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
	
};

DEFINE_PTR_TYPE( VideoCard )

} //bluefish
} //videocards
} //bv
