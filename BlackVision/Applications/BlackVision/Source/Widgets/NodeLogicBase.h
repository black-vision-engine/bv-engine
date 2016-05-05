#pragma once

#include "Engine/Models/Interfaces/INodeLogic.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"


namespace bv { namespace model
{



class NodeLogicBase : public INodeLogic
{
private:
protected:

    DefaultParamValModelPtr         m_paramValModel;

public:
    NodeLogicBase();
    virtual ~NodeLogicBase();


    virtual IParameterPtr                           GetParameter        ( const std::string & name ) const;
    virtual const std::vector< IParameterPtr > &    GetParameters       () const;


	virtual void	            Initialize		()				override {}
    virtual void	            Update			( TimeType )	override;
	virtual void	            Deinitialize	()				override {}

    virtual void                Serialize       ( ISerializer & ser ) const;
    virtual void                Deserialize     ( const IDeserializer & ser );
};


}   // model
}	// bv
