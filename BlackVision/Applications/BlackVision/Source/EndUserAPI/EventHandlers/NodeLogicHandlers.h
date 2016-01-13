#pragma once

#include "Engine/Events/Events.h"

namespace bv
{
class BVAppLogic;


class NodeLogicHandlers
{
private:
    BVAppLogic*         m_appLogic;
public:
    NodeLogicHandlers( BVAppLogic* logic );
    ~NodeLogicHandlers();


    void        WidgetHandler       ( bv::IEventPtr evt );

	void        OnNodeAppearing     ( IEventPtr evt );
	void        OnNodeLeaving       ( IEventPtr evt );
	void        OnNoMoreNodes       ( IEventPtr evt );
};


} //bv
