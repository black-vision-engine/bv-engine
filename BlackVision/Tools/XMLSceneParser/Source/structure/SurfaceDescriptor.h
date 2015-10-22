#pragma once
#include <string>

class ColorDescriptor
{

public:
	ColorDescriptor(){ R = 0.0f; G = 0.0f; B = 0.0f; A = 1.0f; }
	float R;
	float G;
	float B;
	float A;
};

class SurfaceDescriptor
{
public:
	std::string name;
	std::string path;

	ColorDescriptor color_main;

	SurfaceDescriptor();
	~SurfaceDescriptor();
};

