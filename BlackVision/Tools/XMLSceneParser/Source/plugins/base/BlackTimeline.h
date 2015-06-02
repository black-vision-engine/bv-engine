#pragma once

#include <list>
#include <vector>
#include<string>
#include "BlackKeyFrame.h"

using namespace std;

class BlackTimeline{
public:
	BlackTimeline(){ type="normal";duration="0.5";autoplay="false";};
	
	string name;
    string type;
    string duration;
    string autoplay;
	vector<BlackKeyframe*> keyframes;

	
};