#pragma once

#include "CoreDEF.h"
#include "../NodeLogicBase.h"           // Widgets/NodeLogicBase.h doesn't work
#include "Engine/Types/Values/TypedValues.h"


namespace bv
{

namespace model
{

class BasicNode;
DEFINE_PTR_TYPE( BasicNode );

} // model




namespace nodelogic
{

class Arrange;

DEFINE_PTR_TYPE( Arrange );
DEFINE_CONST_PTR_TYPE( Arrange );



class Arrange : public model::NodeLogicBase, public std::enable_shared_from_this< Arrange >
{
public:

    struct CircleArrangeParams
    {
        glm::vec3       Center;
        float           Radius;
        glm::vec3       Rotation;

    public:
        void                            Serialize       ( ISerializer & ser ) const;
        static CircleArrangeParams      Create          ( const IDeserializer & deser );
    };

    struct SphereArrangeParams
    {
        glm::vec3       Center;
        float           Radius;
        int             Columns;
        int             Rows;
        glm::vec3       Rotation;
        bool            Uniform;

    public:
        void                            Serialize       ( ISerializer & ser ) const;
        static SphereArrangeParams      Create          ( const IDeserializer & deser );
    };

    struct LineArrangeParams
    {
        glm::vec3       StartPoint;
        glm::vec3       EndPoint;

    public:
        void                            Serialize       ( ISerializer & ser ) const;
        static LineArrangeParams        Create          ( const IDeserializer & deser );
    };

    struct Grid2DArrangeParams
    {
        int             Rows;
        int             Columns;
        glm::vec3       Center;
        glm::vec3       Rotation;
        glm::vec2       Interspaces;
        bool            Uniform;

    public:
        void                            Serialize       ( ISerializer & ser ) const;
        static Grid2DArrangeParams      Create          ( const IDeserializer & deser );
    };

    struct Grid3DArrangeParams
    {
        int             Rows;
        int             Columns;
        int             Layers;
        glm::vec3       Center;
        glm::vec3       Rotation;
        glm::vec3       Interspaces;
        bool            Uniform;

    public:
        void                            Serialize       ( ISerializer & ser ) const;
        static Grid3DArrangeParams      Create          ( const IDeserializer & deser );
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
    };

    struct PARAMETERS {};

private:
    bv::model::BasicNodePtr	            m_parentNode;


public:
    explicit                            Arrange			( bv::model::BasicNodePtr parent, bv::model::ITimeEvaluatorPtr timeEvaluator );
                                        ~Arrange		();

    virtual void                        Initialize		()				override {}
    virtual void                        Update			( TimeType t )	override;
    virtual void                        Deinitialize	()				override {}


    virtual const std::string &         GetType         () const override;
    static const std::string &          Type            ();

    virtual void                        Serialize       ( ISerializer & ser ) const override;
    static ArrangePtr			        Create          ( const IDeserializer & deser, bv::model::BasicNodePtr parentNode );

    virtual bool                        HandleEvent     ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * editor ) override;

private:

    void            CircleArrange       ( std::vector< bv::model::BasicNodePtr > & nodes, const CircleArrangeParams & params );
    void            LineArrange         ( std::vector< bv::model::BasicNodePtr > & nodes, const LineArrangeParams & params );
    void            Grid2DArrange       ( std::vector< bv::model::BasicNodePtr > & nodes, const Grid2DArrangeParams & params );
    void            Grid3DArrange       ( std::vector< bv::model::BasicNodePtr > & nodes, const Grid3DArrangeParams & params );
    void            SphereArrange       ( std::vector< bv::model::BasicNodePtr > & nodes, const SphereArrangeParams & params );

    std::vector< bv::model::BasicNodePtr >  GetNodesToArrange   ();
    void                                    SetNodePosition     ( bv::model::BasicNodePtr node, glm::vec3 position, TimeType keyTime );
};


}   // nodelogic
}	// bv
