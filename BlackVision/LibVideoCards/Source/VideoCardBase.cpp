#include "VideoCardBase.h"
namespace bv
{

namespace videocards{
//**************************************
//
VideoCardBase::VideoCardBase(void)
{
    SuperMagic=false;
    enabled = false;
    //outputsManager = new VideoOutputsManager();
    isKilled = false;
}

//**************************************
//
VideoCardBase::~VideoCardBase()
{
}

}
}