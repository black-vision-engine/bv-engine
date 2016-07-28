#pragma once

#include <hash_map>




namespace bv
{

class RenderableEntity;
class PdrTimeQuery;
class SceneNode;
class Scene;


class RendererPerformance
{
private:
    typedef std::hash_map< const RenderableEntity *, PdrTimeQuery * >       PdrTimeQueryMap;

    PdrTimeQueryMap         m_timeQueryMap;
    unsigned short          m_windowSize;           // Number of samples which will be averaged.
    unsigned short          m_samplesAccumulated;   // Samples counter.

public:
    explicit    RendererPerformance();


    void        FreeAllPDResources          ( RenderableEntity * renderable );

    void        BeginGPUQuery               ( SceneNode * sceneNode );
    void        EndGPUQuery                 ( SceneNode * sceneNode );

    double      BeginCPURenderMessure       ();
    void        EndCPURenderMessure         ( SceneNode * sceneNode, double timeStart );

    double      BeginCPUQueueingMessure     ();
    void        EndCPUQueueingMessure       ( SceneNode * sceneNode, double timeStart );

    void        QueryPreviousGPUResult      ( SceneNode * sceneNode );

    void        AverageScenePerformanceData ( Scene * scene );
    void        AverageTreePerformanceData  ( SceneNode * node );

private:

    PdrTimeQuery *      GetTimeQuery        ( RenderableEntity * renderable );
};

#define BEGIN_CPU_RENDER_MESSURE( renderer, sceneNode )     \
double __CPURenderStartTime = renderer->Performance().BeginCPURenderMessure();

#define END_CPU_RENDER_MESSURE( renderer, sceneNode )       \
renderer->Performance().EndCPURenderMessure( sceneNode, __CPURenderStartTime );


#define BEGIN_CPU_QUEUEING_MESSURE( renderer, sceneNode )   \
double __CPUQueueingStartTime = renderer->Performance().BeginCPUQueueingMessure();

#define END_CPU_QUEUEING_MESSURE( renderer, sceneNode )     \
renderer->Performance().EndCPUQueueingMessure( sceneNode, __CPUQueueingStartTime );


}	// bv


