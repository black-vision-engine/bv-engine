#pragma once

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Interfaces/ITimeEvaluator.h"
#include "Engine/Models/Timeline/TimelineManager.h"


namespace bv {

model::BasicNodePtr CreateSingleTestNode        ( const std::string & nodeName, const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator, unsigned int numPlugins, ... );
model::BasicNodePtr CreateSingleTestNodeUidsOnly( const std::string & nodeName, const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator, unsigned int numPlugins, ... );


model::BasicNodePtr SolidRect                   ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );
model::BasicNodePtr TexturedTestRandomRect      ( const std::string & name, const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );
model::BasicNodePtr TwoTexturedRectangles       ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );

} //bv
