#include "GeometryChannel.h"

namespace bv { namespace model
{

virtual std::vector< AttrType >                 GeometryChannel::GetType             () const;
virtual std::vector< AttrSemantic >             GeometryChannel::GetSemantic         () const;
virtual PrimitiveType                           GeometryChannel::GetPrimitiveType    () const;

virtual std::vector< IConnectedComponent* >&    GeometryChannel::GetComponents       () const;

virtual bool                                    GeometryChannel::IsTimeInvariant     () const;

virtual                                         GeometryChannel::~GeometryChannel   (){};

} // model
} // bv