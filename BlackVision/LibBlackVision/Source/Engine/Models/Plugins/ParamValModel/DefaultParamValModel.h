#pragma once

#include <vector>

#include "Engine/Models/Plugins/Interfaces/IParamValModel.h"
#include "Engine/Models/Plugins/Interfaces/IParameter.h"
#include "Engine/Models/Plugins/Interfaces/IParamValEvaluator.h"


namespace bv { namespace model {

//class IStateValue
//{
//public:
//    virtual bool StateChanged() const = 0;
//};
//
//DEFINE_PTR_TYPE(IStateValue)
//
//template< typename T >
//class StateValue : public IStateValue {
//    T prevVal, curVal;
//public:
//    Update( const T& val )
//    {
//        prevVal = curVal;
//        curVal = val;
//    }
//    virtual bool StateChanged() const override
//    {
//        return prevVal != curVal;
//    }
//    const T& GetValue() const
//    {
//        return curVal;
//    }
//};



class DefaultParamValModel : public IParamValModel
{
private:

    std::vector< IParameterPtr >            m_parameters;

    std::vector< IParamValEvaluatorPtr >    m_evaluators;
    std::vector< bv::IValueConstPtr >       m_values;

    //std::vector< IStateValueEvaluatorPtr >  m_SVevaluators;
    //std::vector< IStateValuePtr >           m_valuePairs;

    std::vector< bv::IValuePtr >            m_valuesNC;

public:

            DefaultParamValModel            ();
            ~DefaultParamValModel           ();
                                        
    virtual std::vector< IParameterPtr > &              GetParameters   () override;
    virtual const std::vector< bv::IValueConstPtr > &   GetValues       () const override;
    virtual std::vector< IParamValEvaluatorPtr > &      GetEvaluators   () override;

    virtual IParameterPtr                               GetParameter    ( const std::string & name ) override;
    virtual bv::IValueConstPtr                          GetValue        ( const std::string & name ) const override;
    
//  bool                                                HasStateValueChanged ( const std::string & name ) const;
//                                                      GetStateValue () const;                            

    virtual void                                        Update          () override;

    void                                                AddParameter    ( IParameterPtr param );
    void                                                AddValue        ( bv::IValuePtr val );
    void                                                AddValue        ( bv::IValueConstPtr val );
    void                                                AddEvaluator    ( IParamValEvaluatorPtr evaluator );
    void                                                RegisterAll     ( IParamValEvaluatorPtr evaluator );

    const std::vector< bv::IValuePtr > &                GetValuesNC     () const;

};

DEFINE_PTR_TYPE(DefaultParamValModel)

} //model
} //bv
