#pragma once

#include "Interfaces/IVideoCard.h"
#include "Interfaces/IVideoCardDescriptor.h"
#include "AVFrame.h"
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

public:
	                        VideoCard                   ( UInt32 deviceID );
	virtual                 ~VideoCard                  () override; 

    bool                    AttachVideoCard             ();
	void                    InitVideoCard               ();

    virtual void            SetVideoOutput              ( bool enable ) override;

	void				    AddChannel					( Channel * channel );
	void                    RouteChannel                ( ULONG source, ULONG destination, ULONG linkType );
	Channel *			    GetChannelByName			( ChannelName channelName ) const;   

    virtual void            Start                       () override;

    virtual void            ProcessFrame                ( AVFramePtr data, int odd ) override;

    static UInt32           EnumerateDevices            ();

};

DEFINE_PTR_TYPE( VideoCard )

} //bluefish
} //videocards
} //bv
