#pragma once

#include "CoreDEF.h"
#include "../NodeLogicBase.h"           // Widgets/NodeLogicBase.h doesn't work
#include "Engine/Models/BasicNode.h"

#include "Engine/Models/Plugins/Parameters/AbstractModelParameter.h"
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

    bv::model::BasicNodePtr	            m_parentNode;
    model::ITimeEvaluatorPtr            m_timeEval;

    std::vector< ParameterBinding >     m_paramBindings;
    
public:
    explicit    SmoothValueSetter   ( bv::model::BasicNodePtr parent, bv::model::ITimeEvaluatorPtr timeEvaluator );
                ~SmoothValueSetter  ();

	virtual void                        Initialize		()				override {}
	virtual void                        Update			( TimeType t )	override;
	virtual void                        Deinitialize	()				override {}


    virtual const std::string &         GetType         () const override;
    static const std::string &          Type            ();

    virtual void                        Serialize       ( ISerializer & ser ) const override;
    virtual void                        Deserialize     ( const IDeserializer & deser );
    static SmoothValueSetterPtr         Create          ( const IDeserializer & deser, bv::model::BasicNodePtr parentNode );

    virtual bool                        HandleEvent     ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor ) override;

public:

    bool                                AddBinding      ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * editor );
    bool                                RemoveBinding   ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * editor );
    bool                                SetParameter    ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * editor );

private:

    const ParameterBinding *    FindSource              ( const std::string & bindingSource );
    ParameterBinding            FillTargetData          ( const std::string & nodeName, const std::string & pluginName, const std::string & paramName, TransformKind transformKind, ParameterBinding::VectorComponent component );
    IValuePtr                   CreateSrcParameter      ( ModelParamType type, const std::string & name );
    bool                        CreateAndAddSourceData  ( ParameterBinding & srcBindingData, const std::string & sourceName, ModelParamType type );

    void                        UpdateParameter         ( IValuePtr & paramSource, model::IParameterPtr & boundParam, TransformKind kind, ParameterBinding::VectorComponent component );

    void                        SerializeBinding        ( ISerializer & ser, const ParameterBinding & binding ) const;
    bool                        DeserializeBinding      ( const IDeserializer & deser );

    template< typename InterpolatorType, typename Type, ModelParamType type >
    bool                        SetSmoothParam          ( std::shared_ptr< model::SimpleParameterImpl< InterpolatorType, Type, type > > & param, float deltaTime, ISerializer & response, const std::string & srcParamName, const std::string & paramValue );
};



}   // nodelogic
}	// bv

