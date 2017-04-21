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

        static const std::string        BEVEL_HEIGHT;
        static const std::string        BEVEL_TESSELATION;
        static const std::string        BEVEL_DEPTH_FRONT;
        static const std::string        BEVEL_DEPTH_BACK;
        static const std::string        BEVEL_CURVE_FRONT;
        static const std::string        BEVEL_CURVE_BACK;
        static const std::string        SYMETRICAL_BEVEL;
    };

    enum class ExtrudeCurveType
    {
        None,
        Parabola,
        Cosinus,
        Gauss,
        Circle,

        Total
    };

    enum class BevelCurveType
    {
        Line,
        HalfSinus,
        InverseHalfSinus,
        Sinus,
        Circle,
        InverseCircle,

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

    // Curves (side face)
    float           ZeroLineCurve           ( float param );
    float           ParabolaCurve           ( float param );
    float           PeriodicCosinusCurve    ( float param );
    float           GaussCurve              ( float param );
    float           CircleCurve             ( float param );

    // Curves (bevel part)
    float           LineCurve               ( float param );
    float           HalfSinusCurve          ( float param );
    float           SinusCurve              ( float param );
    float           InverseHalfSinusCurve   ( float param );
    float           CircleBevelCurve        ( float param );
    float           InverseCircleBevelCurve ( float param );

private:

    void            AddSymetricalPlane      ( IndexedGeometry & mesh, glm::vec3 translate );
    void            AddSidePlanes           ( IndexedGeometry & mesh, std::vector< IndexType > & edges, std::vector< IndexType > & corners );
    void            CopyTranslate           ( IndexedGeometry & mesh, glm::vec3 translate, SizeType referenceOffset, SizeType numVerticies );


    void            FillWithNormals         ( IndexedGeometry & mesh, std::vector< glm::vec3 > & normals );
    void            DefaultNormals          ( IndexedGeometry & mesh, std::vector< glm::vec3 > & normals, bool useExisting );
    void            ClampNormVecToDefaults  ( IndexedGeometry & normals );

    void            DefaultUVs              ( IndexedGeometry & mesh, std::vector< glm::vec2 > & uvs, bool useExisting );
    void            CopyUVsOnSideFaces      ( std::vector< glm::vec2 > & uvs, std::vector< IndexType > & edges, std::vector< IndexType > & corners );
    void            FillRestUVs             ( IndexedGeometry & mesh, std::vector< glm::vec2 > & uvs );

    void            ApplyFunction           (   ExtrudeCurve curve,
                                                IndexedGeometry & mesh,
                                                IndexedGeometry & normalsVec,
                                                std::vector< IndexType > & edges,
                                                std::vector< IndexType > & corners,
                                                SizeType beginContourOffset,
                                                SizeType endContourOffset,
                                                int tesselation,
                                                float scaleCurve,
                                                float offsetCurve,
                                                bool mirrorFunction = false
                                            );

    void            GenerateSideFace        (   BevelCurveType curve,
                                                IndexedGeometry & mesh,
                                                IndexedGeometry & normalsVec,
                                                std::vector< IndexType > & edges,
                                                std::vector< IndexType > & corners,
                                                SizeType beginContourOffset,
                                                SizeType endContourOffset,
                                                int tesselation,
                                                float bevelHeight,
                                                bool backBevelFace
                                            );

    void            GenerateSideFace        (   ExtrudeCurveType curve,
                                                IndexedGeometry & mesh,
                                                IndexedGeometry & normalsVec,
                                                std::vector< IndexType > & edges,
                                                std::vector< IndexType > & corners,
                                                SizeType beginContourOffset,
                                                SizeType endContourOffset,
                                                int tesselation,
                                                float curveScale,
                                                float bevelHeight
                                            );


    int             FindEdge                ( const std::vector< IndexType > & indicies, IndexType idx1, IndexType idx2 );
    void            AddOrRemoveEdge         ( std::vector< IndexType > & edges, IndexType idx1, IndexType idx2 );

    void            ConnectVerticies        ( std::vector< IndexType > & indicies, std::vector< IndexType > & edges, int offset1, int offset2 );


    std::vector< IndexType >       ExtractEdges            ( IndexedGeometry & mesh );
    std::vector< IndexType >       ExtractCorners          ( IndexedGeometry & mesh, const std::vector< IndexType > & edges, float angleThreshold );

    void            DebugPrintToFile        ( const std::string & fileName, const std::vector< glm::vec3 > & verticies, const std::vector< IndexType > & edges, const std::vector< IndexType > & corners );
    void            DebugPrint              ( std::fstream & file, glm::vec3 vertex );

    Float3AttributeChannelPtr       CreateNormalsChannel    ( ConnectedComponentPtr & prevComponent, ConnectedComponentPtr & newComponent );
    Float3AttributeChannelPtr       CreatePositionsChannel  ( ConnectedComponentPtr & prevComponent, ConnectedComponentPtr & newComponent );
    Float2AttributeChannelPtr       CreateUVsChannel        ( ConnectedComponentPtr & prevComponent, ConnectedComponentPtr & newComponent );
};


DEFINE_ENUM_SET_PARAMETER( DefaultExtrudePlugin::ExtrudeCurveType );
DEFINE_ENUM_SET_PARAMETER( DefaultExtrudePlugin::BevelCurveType );

} // model
} // bv


