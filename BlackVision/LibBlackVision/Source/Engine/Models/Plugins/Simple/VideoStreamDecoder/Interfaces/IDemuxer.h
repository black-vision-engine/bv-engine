#pragma once

#include "CoreDEF.h"

namespace bv
{

class IDemuxer
{
public:
	//FIXME: void*
	//virtual void *				GetPacket			( Int32 streamIdx ) = 0;

	virtual bool				IsOpened			() const = 0;

	virtual bool				HasVideoStream		() const = 0;
    virtual Int32				GetVideoStreamIndex	( UInt32 idx ) const = 0;

	virtual void				Seek				( Float32 time ) = 0;
};

DEFINE_PTR_TYPE( IDemuxer )
DEFINE_CONST_PTR_TYPE( IDemuxer )

} //bv
