#pragma once

namespace bv
{

struct FrameStats
{
public:

    float fps;
    float frameMillis;

public:

        FrameStats      ();
        ~FrameStats     ();

};

struct MovingAverageFrameStats
{
    unsigned int    numFramesInAverage;

    float           fpsExpected;
    float           frameMillisExpected;

    float           fpsVariance;
    float           frameMillisVariance;

    float           minFps;
    unsigned int    minFpsFrame;

    float           maxFps;
    unsigned int    maxFpsFrame;

    float           minFrameMillis;
    unsigned int    minFrameMillisFrame;

    float           maxFrameMillis;
    unsigned int    maxFrameMillisFrame;
};

} //bv
