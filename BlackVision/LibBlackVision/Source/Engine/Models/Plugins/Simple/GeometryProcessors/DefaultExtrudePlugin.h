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
        static const std::string        CURVE_SCALE;
        static const std::string        COSINUS_CURVE_PERIOD;
    };

    enum ExtrudeCurveType
    {
        None,
        Parabola,
        Cosinus,
        Gauss,
        Circle,

        Total
    };
    
    typedef float (DefaultExtrudePlugin::* ExtrudeCurve)( float );

private:

    int         m_numUniqueExtrudedVerticies;
    int         m_numExtrudedVerticies;
    int         m_tesselation;
    float       m_curveScale;
    int         m_cosinusPeriod;

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

    // Curves
    float           ParabolaCurve           ( float param );
    float           PeriodicCosinusCurve    ( float param );
    float           GaussCurve              ( float param );
    float           CircleCurve             ( float param );

private:

    void            AddSymetricalPlane      ( IndexedGeometry & mesh, glm::vec3 translate );
    void            AddSidePlanes           ( IndexedGeometry & mesh, std::vector< IndexType > & edges, std::vector< IndexType > & corners );
    void            FillWithNormals         ( IndexedGeometry & mesh, std::vector< glm::vec3 > & normals );
    void            DefaultNormals          ( IndexedGeometry & mesh, std::vector< glm::vec3 > & normals, bool useExisting );
    void            ClampNormVecToDefaults  ( IndexedGeometry & normals );

    void            ApplyFunction           (   ExtrudeCurve curve,
                                                IndexedGeometry & mesh,
                                                IndexedGeometry & normalsVec,
                                                std::vector< IndexType > & edges,
                                                std::vector< IndexType > & corners
                                            );
   

    int             FindEdge                ( const std::vector< IndexType > & indicies, IndexType idx1, IndexType idx2 );
    void            AddOrRemoveEdge         ( std::vector< IndexType > & edges, IndexType idx1, IndexType idx2 );

    void            ConnectVerticies        ( std::vector< IndexType > & indicies, std::vector< IndexType > & edges, int offset1, int offset2 );


    std::vector< IndexType >       ExtractEdges            ( IndexedGeometry & mesh );
    std::vector< IndexType >       ExtractCorners          ( IndexedGeometry & mesh, const std::vector< IndexType > & edges, float angleThreshold );
};


// Nie patrze� w d�!!! Brzydkie !!!!!!

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


