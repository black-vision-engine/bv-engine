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

//**************************************
//
void VideoCardBase::SetReferenceOffsetValue(int H, int V)
{
    m_refH = H; 
    m_refV = V;

}
}
}