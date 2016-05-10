#pragma once

#include "CoreDEF.h"
#include "../NodeLogicBase.h"           // Widgets/NodeLogicBase.h doesn't work

#include "ParameterBinding.h"

#include <vector>

namespace bv
{

namespace model {

class BasicNode;
DEFINE_PTR_TYPE( BasicNode )

} // model

namespace nodelogic
{

class SmoothValueSetter;

DEFINE_PTR_TYPE( SmoothValueSetter )
DEFINE_CONST_PTR_TYPE( SmoothValueSetter )




class SmoothValueSetter : public model::NodeLogicBase, public std::enable_shared_from_this< SmoothValueSetter >
{
private:
    static const std::string            m_type;

    struct ACTION 
    {
        static const std::string    ADD_PARAMETER_BINDING;
        static const std::string    REMOVE_PARAMETER_BINDING;
        static const std::string    SET_PARAMETER;
    };

    struct PARAMETERS
    {
        static const std::string    SMOOTH_TIME;
    };

private:

    std::string                         m_sceneName;
    bv::model::BasicNodePtr	            m_parentNode;

    std::vector< ParameterBinding >     m_paramBindings;

    model::ITimeEvaluatorPtr            m_timeEval;

    // Parameters
    ValueFloatPtr                       m_smoothTime;
    
public:
    explicit    SmoothValueSetter   ( bv::model::BasicNodePtr parent, bv::model::ITimeEvaluatorPtr timeEvaluator );
                ~SmoothValueSetter  ();

	virtual void                        Initialize		()				override {}
	virtual void                        Update			( TimeType t )	override;
	virtual void                        Deinitialize	()				override {}


    virtual const std::string &         GetType         () const override;
    static const std::string &          Type            ();

    virtual void                        Serialize       ( ISerializer & ser ) const override;
    static SmoothValueSetterPtr         Create          ( const IDeserializer & deser, bv::model::BasicNodePtr parentNode );

    virtual bool                        HandleEvent     ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor ) override;

public:

    bool                                AddBinding      ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * editor );
    bool                                RemoveBinding   ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * editor );
    bool                                SetParameter    ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * editor );

private:

    const ParameterBinding *            FindSource              ( const std::string & bindingSource );
    ParameterBinding                    TargetBindingData       ( IDeserializer & eventDeser, ISerializer & response );
    IValuePtr                           CreateSrcParameter      ( ModelParamType type, const std::string & name, const std::string & initValue );

    void                                UpdateParameter         ( IValuePtr paramSource, model::IParameterPtr boundParam );

    template< typename InterpolatorType, typename Type, ModelParamType type >
    bool                                SetSmoothParam          ( std::shared_ptr< model::SimpleParameterImpl< InterpolatorType, Type, type > > & param, ISerializer & response, const std::string & srcParamName, const std::string & paramValue );
};



}   // nodelogic
}	// bv

