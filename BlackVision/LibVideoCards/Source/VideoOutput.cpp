#include "VideoOutput.h"
namespace bv
{

namespace videocards{

VideoOutput::VideoOutput(int _id)
{
    id=_id;
}


VideoOutput::VideoOutput(OutputConfigg* config)
{
    this->m_outputConfig = config;//new OutputConfig;
	//this->m_outputConfig = config;
}

VideoOutput::~VideoOutput(void)
{
	delete m_outputConfig;
}

bool VideoOutput::AddChannel      (VideoOutputDescriptor outputDescriptor)
{
    Channels.push_back(outputDescriptor);
    return true;
}
}
}