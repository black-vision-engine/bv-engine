#pragma once

#include "CoreDEF.h"
#include "Engine/Models/NodeLogics/NodeLogicBase.h"
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

class MaxSize;

DEFINE_PTR_TYPE( MaxSize );
DEFINE_CONST_PTR_TYPE( MaxSize );



/**@brief Limits maximal size of node bounding box. Scales node if it exceeds this max size.
@ingroup LogicsList*/
class MaxSize : public model::NodeLogicBase, public std::enable_shared_from_this< MaxSize >
{
private:
    static const std::string            m_type;

    struct ACTION
    {
        //static const std::string    ACTION_NAME;
    };

    struct PARAMETERS
    {
        static const std::string    MAX_HEIGHT;
        static const std::string    MAX_WIDTH;
        static const std::string    MAX_DEPTH;
        static const std::string    IS_PROPORTIONAL;
    };

private:
    bv::model::BasicNodeWeakPtr			m_parentNode;

    ValueFloatPtr                       m_maxWidth;
    ValueFloatPtr                       m_maxHeight;
    ValueFloatPtr                       m_maxDepth;

    ValueBoolPtr                        m_isProportional;

public:
    explicit                            MaxSize			( bv::model::BasicNodeWeakPtr parent, bv::model::ITimeEvaluatorPtr timeEvaluator );
                                        ~MaxSize		();

    virtual void                        PostChildrenUpdate  ( TimeType )    override;

    virtual const std::string &         GetType         () const override;
    static const std::string &          Type            ();

    virtual void                        Serialize       ( ISerializer & ser ) const override;
    static MaxSizePtr			        Create          ( const IDeserializer & deser, bv::model::BasicNodeWeakPtr parentNode );

    virtual bool                        HandleEvent     ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * editor ) override;

};


}   // nodelogic
}	// bv
