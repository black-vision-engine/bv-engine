#pragma once

#include "CoreDEF.h"


namespace bv { namespace videocards {

class IVideoCard
{
public:

    virtual                     ~IVideoCard             () {}

	//virtual IPlaybackControl*   GetPlaybackControl      () const = 0;
	//virtual void                EnableVideoOutput       () = 0;
	//virtual void                DisableVideoOutput      () = 0;
	//virtual bool                SetupDevice             (unsigned int deviceIndex) = 0;
	//virtual bool                ReleaseDevice           () = 0;
	//virtual const TCHAR*        GetFormatDescription    () const = 0;
	//virtual bool                SetVideoFormat          (const tstring& strDesiredFrameFormat)
	//{
	//	LOG << TEXT("SetVideoFormat is no supported");
	//	return false; 
 //   }
};

DEFINE_PTR_TYPE( IVideoCard )
DEFINE_CONST_PTR_TYPE( IVideoCard )

} //videocards
} //bv
