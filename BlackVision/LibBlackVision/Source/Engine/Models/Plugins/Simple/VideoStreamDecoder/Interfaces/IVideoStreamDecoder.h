#pragma once

#include "CoreDEF.h"

namespace bv
{

class IVideoStreamDecoder
{
public:
	
	virtual UInt32				GetWidth				() const = 0;
	virtual UInt32				GetHeight				() const = 0;
	virtual UInt32				GetFrameRate			() const = 0;

	//FIXME
	//virtual bool				DecodePacket			( void * packet, void * frame ) = 0;

	virtual Int32				GetStreamIdx			() const = 0;
	virtual UInt32				GetCurrentFrameId		() const = 0;	
};

DEFINE_PTR_TYPE( IVideoStreamDecoder )
DEFINE_CONST_PTR_TYPE( IVideoStreamDecoder )

} //bv
