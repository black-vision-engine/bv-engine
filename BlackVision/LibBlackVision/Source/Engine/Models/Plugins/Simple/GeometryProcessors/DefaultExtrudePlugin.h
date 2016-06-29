#pragma once

#include "Engine/Models/Plugins/Simple/GeometryProcessors/DefaultGeometryProcessorBase.h"
#include "Engine/Models/Plugins/IndexedGeometry.h"



namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultExtrudePluginDesc : public DefaultGeometryProcessorDescBase
{
public:

    DefaultExtrudePluginDesc                ();


    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();

};

// ***************************** PLUGIN ********************************** 
class DefaultExtrudePlugin : public DefaultGeometryProcessorBase
{
public:

    struct PARAMS
    {
        static const std::string        EXTRUDE_VECTOR;
        static const std::string        SMOOTH_THRESHOLD_ANGLE;
    };


private:

    int         m_numUniqueExtrudedVerticies;
    int         m_numExtrudedVerticies;

public:

    explicit            DefaultExtrudePlugin   (    const std::string & name,
                                                                            const std::string & uid,
                                                                            IPluginPtr prev,
                                                                            DefaultPluginParamValModelPtr model );
                        ~DefaultExtrudePlugin  ();

    virtual void        ProcessVertexAttributesChannel  ();
    virtual void        ProcessConnectedComponent       (   model::ConnectedComponentPtr & currComponent,
                                                            std::vector< IConnectedComponentPtr > & allComponents,
                                                            PrimitiveType topology ) override;

private:

    void                            AddSymetricalPlane      ( IndexedGeometry & mesh, glm::vec3 translate );
    void                            AddSidePlanes           ( IndexedGeometry & mesh, const std::vector< INDEX_TYPE > & edges );
    void                            FillWithNormals         ( IndexedGeometry & mesh, IndexedGeometry & normals, glm::vec3 translate );
    
    std::vector< INDEX_TYPE >       ExtractEdges            ( IndexedGeometry & mesh );
    std::vector< INDEX_TYPE >       ExtractCorners          ( IndexedGeometry & mesh, const std::vector< INDEX_TYPE > & edges, float angleThreshold );

    int                             FindEdge                ( const std::vector< INDEX_TYPE > & indicies, INDEX_TYPE idx1, INDEX_TYPE idx2 );
    void                            AddOrRemoveEdge         ( std::vector< INDEX_TYPE > & edges, INDEX_TYPE idx1, INDEX_TYPE idx2 );
};


} // model
} // bv


