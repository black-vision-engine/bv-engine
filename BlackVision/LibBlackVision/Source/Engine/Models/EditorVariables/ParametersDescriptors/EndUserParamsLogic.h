#pragma once

#include "EndUserParamDescriptor.h"
#include "Engine/Events/ParamAddress.h"

#include <map>



namespace bv
{


class ISerializer;
class IDeserializer;



class EndUserParamsLogic
{
private:

    std::map< ParameterAddress, EndUserParamDescriptor >    m_paramsDescsMap;

public:
    explicit                EndUserParamsLogic();
                            ~EndUserParamsLogic();

    bool                    AddDescriptor   ( ParameterAddress && param, EndUserParamDescriptor && descriptor );
    bool                    RemoveDescriptor( const ParameterAddress & param );

    void                    Serialize       ( ISerializer & ser ) const;
    void                    Deserialize     ( const IDeserializer & deser );

    const EndUserParamDescriptor *      GetDescriptor   ( const ParameterAddress & param );
};


}	// bv
