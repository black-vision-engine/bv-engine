#pragma once

#include "VideoOutputsManager.h"
#include "Models/BlueFish/CFrame.h"
#include <memory>

//#include <..\dep\vld\include\vld.h>


namespace bv
{

namespace videocards{
using namespace std;

enum VideoCard_Modes{SD,HD};
enum VideoCard_RAM_GPU{RAM,GPU};


#define NUMBER_OGL_TEXTURES 2
#define NUMBER_BF_CARD_BUFFERS 3
#define NUMBER_CHUNKS 1

class VideoCardBase
{
protected:
    string                          Brand;
    string                          Name;
    VideoCard_RAM_GPU		        transferMode;
    long                            m_referenceMode;
    unsigned int                    device_id;
    bool                            enabled;
    int                             m_refH;
    int                             m_refV;

public:
    VideoCard_Modes         mode;
    bool                    SuperMagic;
    VideoOutputsManager*    outputsManager; 
    bool                    isKilled;

    VideoCardBase   	                        (void);
    virtual ~VideoCardBase	                    ();
    int                     GetTransferMode     ()                                                      {return transferMode;};
    long                    GetReferenceMode    ()                                                      {return m_referenceMode;};
    int                     GetReferencH        ()                                                      {return m_refH;};
    int                     GetReferencV        ()                                                      {return m_refV;};
    void                    SetTransferMode     (VideoCard_RAM_GPU mode)                                {this->transferMode = mode;};
    void                    SetDeviceID         (unsigned int id)                                       {this->device_id = id;};
    bool                    IsActive            ()                                                      {return enabled;};
    void                    Enable              ()                                                      {this->enabled = true;};
    void                    Disable             ()                                                      {this->enabled = false;};
    string				    GetName			    ()			                                            {return Name;};
    string				    GetBrand			()			                                            {return Brand;};
	virtual bool			InitVideoCard	    ( const std::vector<int> & hackBuffersUids )			{ {hackBuffersUids;}  return false; };
    virtual bool			DetectVideoCard     ()			                                            {return false;};
    virtual void            DeliverFrameFromRAM (unsigned char * )                                      {};
	virtual void            DeliverFrameFromRAM (std::shared_ptr<CFrame> Frame )                        {};
    virtual bool            ActivateVideoCard   ()                                                      {return false;};
    virtual bool            DeactivateVideoCard ()                                                      {return false;};
    virtual void            Black               ()                                                      {return ;};
    virtual bool            InitOutputChannels  ()                                                      {return false;};
    virtual bool            InitHardwareOutputs ()                                                      {return false;};
	virtual bool            UpdateReferenceOffset  (std::string ChannelName/*A,B,C,D,E,F*/, int refH, int refV)													{{refH;}{refV;}return false;};
    virtual bool            UpdateReferenceMode    (std::string ChannelName/*A,B,C,D,E,F*/, std::string ReferenceModeName/*FREERUN,IN_A,IN_B,ANALOG,GENLOCK*/)	{{ChannelName;}{ReferenceModeName;}return false;};
    virtual unsigned int    DetectInputs        ()                                                      {return 0;}
    virtual unsigned int    DetectOutputs       ()                                                      {return 0;}
	virtual void			AddChannel			()														{};
    virtual void			StartVideoCardProccessing                   ()								{};
    //virtual void			StopVideoCardProccessing                    ()								{};
    virtual void			SuspendVideoCardProccessing                 ()								{};
    virtual void			ResumeVideoCardProccessing                  ()								{};
	virtual unsigned char *	GetCaptureBufferForShaderProccessing		(std::string ChannelName/*A,B,C,D,E,F*/){return 0;};
    virtual size_t	        CheckIfNewFrameArrived                      (std::string ChannelName/*A,B,C,D,E,F*/){return false;};
    virtual void	        UnblockCaptureQueue                         (std::string ChannelName/*A,B,C,D,E,F*/){};

    ///GPUDirect
    virtual bool			InitVideoCardGPUDirect    ()                                                {return false;};
    virtual bool            DirectGPUPreRender        ()                                                {return false;};
    virtual bool            DirectGPUPostRender       ()                                                {return false;};
	virtual void            DeliverFrameFromGPU       (unsigned int buffer)				                { {buffer;}};



};

}
}