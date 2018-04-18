#pragma once

#include <string>
#include <memory>
#include <assert.h>

#include "Engine/Types/Enums.h"

#include "Engine/Models/Interfaces/ITimeEvaluator.h"



/**@page Parameter

*/


namespace bv { namespace model {

    class IParameter;
    DEFINE_PTR_TYPE(IParameter)

    class IParameter;
    typedef std::shared_ptr< const IParameter > IParameterConstPtr;




    /**@brief Parameter interface.
    @ingroup Model*/
    class IParameter : public ISerializable
    {
    public:
        virtual void                    Serialize       ( ISerializer& ser ) const              = 0;
        virtual const std::string &     GetName         () const								= 0;
        virtual void                    SetName         ( const std::string & name )            = 0;
        virtual ModelParamType          GetType         () const								= 0;

        virtual ITimeEvaluatorPtr       GetTimeEvaluator() const    							= 0;
        virtual void                    SetTimeEvaluator( ITimeEvaluatorPtr timeEvaluator )		= 0;

        virtual VoidPtr                 QueryParamTyped ()										= 0;

        virtual std::string             EvaluateToString( TimeType t ) const                    = 0;

        virtual ~IParameter() {}

    };

    // *********************************
    //
    template< typename ParamTypePtr >
    ParamTypePtr  QueryTypedParam( IParameterPtr param )
    {
        assert( param );
        ModelParamType paramType = param->GetType();
        ModelParamType elementType = ParamTypePtr::element_type::Type();
        if( paramType != elementType)
        {
            return nullptr;
        }

        return std::static_pointer_cast< ParamTypePtr::element_type >( param->QueryParamTyped() );
    }

    // *********************************
    //
    template< typename ParamTypeConstPtr >
    ParamTypeConstPtr QueryTypedParam( IParameterConstPtr param )
    {
        if( param->GetType() != ParamTypeConstPtr::element_type::Type() )
        {
            return nullptr;
        }

        return std::static_pointer_cast< ParamTypeConstPtr::element_type >( std::const_pointer_cast< IParameter >( param )->QueryParamTyped() );
    }

    // *********************************
    //
    template< typename ParamType >
    ParamType * QueryTypedParam( IParameter * param )
    {
        if( param->GetType() != ParamType::Type() )
        {
            return nullptr;
        }

        return static_cast< ParamType * >( param->QueryParamTyped().get() );
    }

    // *********************************
    //
    template< typename ParamType >
    const ParamType * QueryTypedParam( const IParameter * param )
    {
        if( param->GetType() != ParamType::Type() )
        {
            return nullptr;
        }

        return static_cast< const ParamType * >( std::const_pointer_cast< IParameter * >( param )->QueryParamTyped().get() );
    }

    void    SetParamTimeline( IParameterPtr param, ITimeEvaluatorPtr timeline );

} //model
} //bv
