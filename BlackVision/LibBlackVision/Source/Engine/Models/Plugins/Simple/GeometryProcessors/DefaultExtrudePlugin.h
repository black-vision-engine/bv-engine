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
        static const std::string        EXTRUDE_CURVE;
        static const std::string        EXTRUDE_TESSELATION;
    };

    enum ExtrudeCurveType
    {
        None,
        Sinus,

        Total
    };

    typedef float (*ExtrudeCurve)( float );

private:

    int         m_numUniqueExtrudedVerticies;
    int         m_numExtrudedVerticies;
    int         m_tesselation;

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
    void                            AddSidePlanes           ( IndexedGeometry & mesh, std::vector< INDEX_TYPE > & edges, std::vector< INDEX_TYPE > & corners );
    void                            FillWithNormals         ( IndexedGeometry & mesh, std::vector< glm::vec3 > & normals, glm::vec3 translate, bool fillDefaults );

    void                            ApplyFunction           (   ExtrudeCurve curve,
                                                                IndexedGeometry & mesh,
                                                                IndexedGeometry & normalsVec,
                                                                std::vector< INDEX_TYPE > & edges,
                                                                std::vector< INDEX_TYPE > & corners
                                                            );
    
    std::vector< INDEX_TYPE >       ExtractEdges            ( IndexedGeometry & mesh );
    std::vector< INDEX_TYPE >       ExtractCorners          ( IndexedGeometry & mesh, const std::vector< INDEX_TYPE > & edges, float angleThreshold );

    int                             FindEdge                ( const std::vector< INDEX_TYPE > & indicies, INDEX_TYPE idx1, INDEX_TYPE idx2 );
    void                            AddOrRemoveEdge         ( std::vector< INDEX_TYPE > & edges, INDEX_TYPE idx1, INDEX_TYPE idx2 );
};


// Nie patrzeæ w dó³!!! Brzydkie !!!!!!

template<>
inline bool SetParameter< DefaultExtrudePlugin::ExtrudeCurveType >( IParameterPtr param, TimeType t, const DefaultExtrudePlugin::ExtrudeCurveType & val )
{
    //return SetSimpleTypedParameter< ParamEnum<DefaultCirclePlugin::OpenAngleMode> >( param, t, val );
    typedef ParamEnum< DefaultExtrudePlugin::ExtrudeCurveType > ParamType;

    ParamType * typedParam = QueryTypedParam< std::shared_ptr< ParamType > >( param ).get();

    if( typedParam == nullptr )
    {
        return false;
    }

    typedParam->SetVal( val, t );

    return true;
}


} // model
} // bv


