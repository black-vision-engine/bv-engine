#pragma once

#include <list>
#include <vector>
#include<string>
#include "XMLPlugin.h"
using namespace std;

enum BLEND_MODES{ADD, MULTIPLY, NORMAL};

class BlackNode{
public:
    BlackNode(){blend_mode=BLEND_MODES::NORMAL;visible=true;};
	
	string name;
    bool visible;
    BLEND_MODES blend_mode;

	vector<BlackNode> children;
	vector<XMLPlugin*> plugins;

	
};