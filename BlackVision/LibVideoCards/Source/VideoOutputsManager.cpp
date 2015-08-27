#include "VideoOutputsManager.h"

namespace bv
{

namespace videocards{
VideoOutputsManager::VideoOutputsManager(int _count)
{
    ChannelsCount=0;
    Video_id = 1000;
    this->SetChannelCount(_count);
    DefaultVideoOutput = nullptr;
}


VideoOutputsManager::~VideoOutputsManager()
{
	delete DefaultVideoOutput;
} 



VideoOutput* VideoOutputsManager::CreateVideoOutput()
{
    VideoOutput* output = new VideoOutput(Video_id++);
    if(DefaultVideoOutput == nullptr)
    {
        DefaultVideoOutput = output;
    }
    return output;

}

void VideoOutputsManager::AddVideoOutput(OutputConfigg* config)
{
	Outputs.push_back(new VideoOutput(config));
}
}
}