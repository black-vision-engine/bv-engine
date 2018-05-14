#pragma once

#include "CoreDEF.h"
#include "Engine/Interfaces/IUpdatable.h"
#include "Serialization/ISerializable.h"
#include "Engine/Models/Plugins/Interfaces/IParameter.h"

#include <vector>


/**@defgroup LogicsList
@ingroup Model*/


/**@page NodeLogics Logics

@section NodeLogics_General General

Logics add posibility to change normal Nodes update behavior. Moreover they can handle generic commands (defined by logic implementation)
coming from API. This is main difference between @ref Plugins "Plugins" and Logics. The only way to modify Plugin's state, is changing it's @ref Parameters "Parameters"
or loading asset. Logics can implement own commands and make operations on @ref Nodes "Nodes" in their subtree.

@subsection NodeLogics_Update Node update logic

Logics can implement custom functions to handle update. There're 3 functions which are called in different parts of @ref Nodes "Node's" update
function. From user perspective it could be useful to know order of updates in model part of Engine. Here I show a little bit simplified view:

- Visiblity check - If Node is invisible, no further processing is made in this frame.
- Call to Logic's @ref bv::model::INodeLogic::PreNodeUpdate
- @ref NodeEffects "Effect" update
- Node's Plugins list update
- Call to Logic's @ref bv::model::INodeLogic::Update
- Recursive update of Node's children
- Bounding box recomputation
- Call to Logic's @ref bv::model::INodeLogic::PostChildrenUpdate

Understanding update order could help to understand issues and restrictions related to Logics. 
@ref ExtendingEngine_CreatingLogics_UpdateHooks "More about update hooks functions."

@subsection NodeLogics_CustomCommands Custom commands

Logics can handle custom commands from API. Events from API are redirected to @ref bv::model::INodeLogic::HandleEvent function.
Content of command is defined by Logic's implementation. HandleEvent function can send response with content defined by implementation.
Commands executed in events update phase of engine, that means that this happens outside @ref Scenes "Scene" tree update.

@subsection NodeLogics_Parameters Parameters

Each Logic defines own set of parameters, which can be used like in @ref Plugins "Plugins".
Check how to set parameters through API @ref API_Parameters "here".

@subsection NodeLogics_APIFunctionalities Logics API commands

@copydoc API_LogicsManipulation

@see LogicsList, bv::model::INodeLogic
*/


namespace bv
{

class BVProjectEditor;
    
namespace model
{



/**@brief Node logic interface.
@ingroup Model*/
class INodeLogic : public IUpdatable, public ISerializable
{
public:

	virtual void					Initialize		()				= 0;
	virtual void					Deinitialize	()				= 0;

    virtual void					Update			    ( TimeType t )  = 0;
    virtual void                    PreNodeUpdate       ( TimeType t )  = 0;
    virtual void                    PostChildrenUpdate  ( TimeType t )  = 0;

    virtual IParameterPtr                           GetParameter        ( const std::string & name ) const = 0;
    virtual const std::vector< IParameterPtr > &    GetParameters       () const = 0;

    virtual const std::string &     GetType         () const        = 0;
    virtual bool                    HandleEvent     ( IDeserializer & eventStr, ISerializer & response, BVProjectEditor * editor ) = 0;

};

DEFINE_PTR_TYPE(INodeLogic)
DEFINE_CONST_PTR_TYPE(INodeLogic)

} // model
} // bv
