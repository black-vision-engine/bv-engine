#pragma once

#include "CoreDEF.h"
#include "Engine/Models/NodeLogics/NodeLogicBase.h"
#include "Engine/Types/Values/TypedValues.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

namespace bv
{

namespace model
{

class BasicNode;
DEFINE_PTR_TYPE( BasicNode );

} // model




namespace nodelogic
{

class Follow;

DEFINE_PTR_TYPE( Follow );
DEFINE_CONST_PTR_TYPE( Follow );



class Follow : public model::NodeLogicBase, public std::enable_shared_from_this< Follow >
{
public:

    enum FollowingMode
    {
        Previous        = 0,
        FirstInSubtree  = 1,
        Path            = 2
    };

    enum BBAlignementX
    {
        Left        = 0,
        CenterX     = 1,
        Right       = 2
    };

    enum BBAlignementY
    {
        Bottom      = 0,
        CenterY     = 1,
        Top         = 2
    };

    enum BBAlignementZ
    {
        Back        = 0,
        CenterZ     = 1,
        Front       = 2
    };

    typedef std::shared_ptr< bv::model::ParamEnum< FollowingMode > > FollowingModeParamPtr;
    typedef std::shared_ptr< bv::model::ParamEnum< BBAlignementX > > BBAlignementXparamPtr;
    typedef std::shared_ptr< bv::model::ParamEnum< BBAlignementY > > BBAlignementYParamPtr;
    typedef std::shared_ptr< bv::model::ParamEnum< BBAlignementZ > > BBAlignementZParamPtr;

private:
    static const std::string            m_type;

    struct ACTION
    {
        //static const std::string    ACTION_NAME;
    };

    struct PARAMETERS
    {
        static const std::string    OFFSET_X;
        static const std::string    OFFSET_Y;
        static const std::string    OFFSET_Z;

        static const std::string    ALIGN_X;
        static const std::string    ALIGN_Y;
        static const std::string    ALIGN_Z;

        static const std::string    FOLLOWING_MODE;
        static const std::string    FOLLOWING_NODE_PATH;
    };

private:
    bv::model::BasicNodeWeakPtr         m_parentNode;

    ValueFloatPtr           m_offsetX;
    ValueFloatPtr           m_offsetY;
    ValueFloatPtr           m_offsetZ;

    BBAlignementXparamPtr   m_alignX;
    BBAlignementYParamPtr   m_alignY;
    BBAlignementZParamPtr   m_alignZ;
    
    FollowingModeParamPtr   m_followingMode;
    ValueStringPtr          m_nodePath;

public:
    explicit    Follow			        ( bv::model::BasicNodeWeakPtr parent, bv::model::ITimeEvaluatorPtr timeEvaluator );
    ~Follow		();

    virtual void                        Initialize		()				override;
    virtual void                        Update			( TimeType t )	override;
    virtual void                        Deinitialize	()				override;


    virtual const std::string &         GetType         () const override;
    static const std::string &          Type            ();

    virtual void                        Serialize       ( ISerializer & ser ) const override;
    static FollowPtr			        Create          ( const IDeserializer & deser, bv::model::BasicNodeWeakPtr parentNode );

    virtual bool                        HandleEvent     ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * editor ) override;


private:
    void            NodeRemovedHandler      ( IEventPtr evt );


    model::BasicNodeConstPtr    GetObservedNode     ();
    glm::vec3                   GetBBPoint          ( model::BasicNodeConstPtr & node );
    glm::mat4                   GetBBTransform      ( model::BasicNodeConstPtr & node );
    void                        ApplyTranslation    ( glm::vec3 & transform );
};




}   // nodelogic
}	// bv
