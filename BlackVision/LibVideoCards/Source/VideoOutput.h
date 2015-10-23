#pragma once
#include<vector>
#include<string>

namespace bv
{

namespace videocards{
enum VideoOutputType {FILL,KEY,FILL_KEY,FILL_AUTO_KEY};
enum VideoCardMode{HD_50_I,SD_50_I,HD_25_P,SD_25_P};
using namespace std;

struct OutputConfigg
{
	int id;
	string type;
	unsigned int renderer;
	unsigned int resolution;
	unsigned int refresh;
	bool interlaced;
};

class VideoOutputDescriptor{
public:
    VideoOutputDescriptor(VideoCardMode _displayMode,int id,VideoOutputType type)
    {
        displayMode = _displayMode;
        OutputId = id;
        OutputType = type;
    }
    VideoCardMode   displayMode;
    int             OutputId;
    VideoOutputType OutputType;
};

class VideoOutput
{
private:
    
        VideoOutputType type;
    public:
        vector<VideoOutputDescriptor>           Channels;
		OutputConfigg* m_outputConfig;
		
        
        
        int id;


                            VideoOutput         (int _id);
                            VideoOutput         (OutputConfigg* VideoConfig);
                            ~VideoOutput        ();
        void                SetVideoOutputType  (VideoOutputType t)     {type=t;};
        VideoOutputType     getVideoOutputType  ()                      {return type;}
        bool                AddChannel      (VideoOutputDescriptor outputDescriptor);               
        
        

       
        
};
}
}