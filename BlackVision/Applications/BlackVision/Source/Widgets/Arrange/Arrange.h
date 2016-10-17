#pragma once

#include "Widgets/NodeLogicBase.h"
#include "Engine/Types/Values/TypedValues.h"


namespace bv { 
    

namespace model {

class BasicNode;
DEFINE_PTR_TYPE( BasicNode )
DEFINE_WEAK_PTR_TYPE( BasicNode )

} // model


namespace nodelogic {


class Arrange;

DEFINE_PTR_TYPE( Arrange );
DEFINE_CONST_PTR_TYPE( Arrange );


class Arrange : public model::NodeLogicBase, public std::enable_shared_from_this< Arrange >
{
public:

    enum ArrangmentType
    {
        Circle,
        Sphere,
        Grid2D,
        Grid3D,
        Line,

        Total
    };

    struct ArrangeParamsBase
    {
        virtual void                    Serialize       ( ISerializer & ser ) const = 0;
    };

    struct CircleArrangeParams : public ArrangeParamsBase
    {
        glm::vec3       Center;
        float           Radius;
        glm::vec3       Rotation;

    public:
        void                                            Serialize       ( ISerializer & ser ) const;
        static std::unique_ptr< CircleArrangeParams >   Create          ( const IDeserializer & deser );
        static std::unique_ptr< CircleArrangeParams >   Create          ( model::DefaultParamValModelPtr & model );
    };

    struct SphereArrangeParams : public ArrangeParamsBase
    {
        glm::vec3       Center;
        float           Radius;
        int             Columns;
        int             Rows;
        glm::vec3       Rotation;
        bool            Uniform;

    public:
        void                                            Serialize       ( ISerializer & ser ) const;
        static std::unique_ptr< SphereArrangeParams >   Create          ( const IDeserializer & deser );
        static std::unique_ptr< SphereArrangeParams >   Create          ( model::DefaultParamValModelPtr & model );
    };

    struct LineArrangeParams : public ArrangeParamsBase
    {
        glm::vec3       StartPoint;
        glm::vec3       EndPoint;

    public:
        void                                            Serialize       ( ISerializer & ser ) const;
        static std::unique_ptr< LineArrangeParams >     Create          ( const IDeserializer & deser );
        static std::unique_ptr< LineArrangeParams >     Create          ( model::DefaultParamValModelPtr & model );
    };

    struct Grid2DArrangeParams : public ArrangeParamsBase
    {
        int             Rows;
        int             Columns;
        glm::vec3       Center;
        glm::vec3       Rotation;
        glm::vec2       Interspaces;
        bool            Uniform;

    public:
        void                                            Serialize       ( ISerializer & ser ) const;
        static std::unique_ptr< Grid2DArrangeParams >   Create          ( const IDeserializer & deser );
        static std::unique_ptr< Grid2DArrangeParams >   Create          ( model::DefaultParamValModelPtr & model );
    };

    struct Grid3DArrangeParams : public ArrangeParamsBase
    {
        int             Rows;
        int             Columns;
        int             Layers;
        glm::vec3       Center;
        glm::vec3       Rotation;
        glm::vec3       Interspaces;
        bool            Uniform;

    public:
        void                                            Serialize       ( ISerializer & ser ) const;
        static std::unique_ptr< Grid3DArrangeParams >   Create          ( const IDeserializer & deser );
        static std::unique_ptr< Grid3DArrangeParams >   Create          ( model::DefaultParamValModelPtr & model );
    };

private:

    static const std::string            m_type;

    struct ACTION
    {
        static const std::string    LINE_ARRANGE;
        static const std::string    CIRCLE_ARRANGE;
        static const std::string    SPHERE_ARRANGE;
        static const std::string    GRID2D_ARRANGE;
        static const std::string    GRID3D_ARRANGE;
        static const std::string    GET_PARAMETERS;
        static const std::string    ARRANGE_AFTER_LOAD;
    };

    struct PARAMETERS
    {
        static const std::string    LINE_START_POINT;
        static const std::string    LINE_END_POINT;
    
        static const std::string    CIRCLE_RADIUS;
        static const std::string    CIRCLE_ROTATION;
        static const std::string    CIRCLE_CENTER;

        static const std::string    SPHERE_RADIUS;
        static const std::string    SPHERE_ROTATION;
        static const std::string    SPHERE_CENTER;
        static const std::string    SPHERE_ROWS;
        static const std::string    SPHERE_COLUMNS;

        static const std::string    GRID2D_ROTATION;
        static const std::string    GRID2D_CENTER;
        static const std::string    GRID2D_ROWS;
        static const std::string    GRID2D_COLUMNS;
        static const std::string    GRID2D_INTERSPACES;

        static const std::string    GRID3D_ROTATION;
        static const std::string    GRID3D_CENTER;
        static const std::string    GRID3D_ROWS;
        static const std::string    GRID3D_COLUMNS;
        static const std::string    GRID3D_LAYERS;
        static const std::string    GRID3D_INTERSPACES;

        static const std::string    ARRANGE_AFTER_LOAD;
    };

private:

    model::BasicNodeWeakPtr                 m_parentNode;
    std::unique_ptr< ArrangeParamsBase >    m_lastArrangement;

    ArrangmentType                          m_lastArrangementType;

public:

    explicit                            Arrange			( model::BasicNodeWeakPtr parent, model::ITimeEvaluatorPtr timeEvaluator );
                                        ~Arrange		();

    virtual void	                    Update			( TimeType );

    virtual const std::string &         GetType         () const override;
    static const std::string &          Type            ();

    virtual void                        Serialize       ( ISerializer & ser ) const override;
    virtual void                        Deserialize     ( const IDeserializer & ser );
    static ArrangePtr			        Create          ( const IDeserializer & deser, model::BasicNodeWeakPtr parentNode );

    virtual bool                        HandleEvent     ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * editor ) override;

private:

    void            ArrangeChildren     ( ArrangmentType type, std::unique_ptr< ArrangeParamsBase > & params );

    void            CircleArrange       ( std::vector< model::BasicNodePtr > & nodes, const CircleArrangeParams & params );
    void            LineArrange         ( std::vector< model::BasicNodePtr > & nodes, const LineArrangeParams & params );
    void            Grid2DArrange       ( std::vector< model::BasicNodePtr > & nodes, const Grid2DArrangeParams & params );
    void            Grid3DArrange       ( std::vector< model::BasicNodePtr > & nodes, const Grid3DArrangeParams & params );
    void            SphereArrange       ( std::vector< model::BasicNodePtr > & nodes, const SphereArrangeParams & params );

    std::vector< model::BasicNodePtr >      GetNodesToArrange   ();
    void                                    SetNodePosition     ( model::BasicNodePtr node, glm::vec3 position, TimeType keyTime );

};

}   // nodelogic
}	// bv
