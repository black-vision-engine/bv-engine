#pragma once

#include "Engine/Models/Interfaces/INodeLogic.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"


namespace bv { namespace model
{



class NodeLogicBase : public INodeLogic
{
private:

    NodeLogicBase();

protected:

    DefaultParamValModelPtr         m_paramValModel;

public:
    virtual ~NodeLogicBase();


    virtual IParameterPtr                           GetParameter        ( const std::string & name ) const;
    virtual const std::vector< IParameterPtr > &    GetParameters       () const;


    virtual void                Serialize       ( ISerializer & ser ) const;
    void                        Deserialize     ( const IDeserializer & ser );
};


}   // model
}	// bv
