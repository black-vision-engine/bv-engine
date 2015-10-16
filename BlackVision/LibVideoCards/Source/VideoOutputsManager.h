#pragma once
#include"VideoOutput.h"
namespace bv
{

namespace videocards{
using namespace std;
class VideoOutputsManager
{
private:
    int                     ChannelsCount;
    int                     Video_id;
    VideoOutput*            DefaultVideoOutput;
public:
                            VideoOutputsManager            (int _count);
                            ~VideoOutputsManager();
    //void                    AddVideoOutput          (VideoOutput output
    vector<VideoOutput*>    Outputs;
    
    VideoOutput*            CreateVideoOutput       ();         
    VideoOutput*            GetDefaultVideoOutput   ()							{return DefaultVideoOutput;};
	void		            AddVideoOutput			(OutputConfigg* VideoConfig);   
    void                    SetChannelCount         (int count)                 {ChannelsCount = count;};

};
}
}