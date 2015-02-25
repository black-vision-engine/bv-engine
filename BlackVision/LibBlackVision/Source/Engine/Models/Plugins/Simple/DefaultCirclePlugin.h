#include "DefaultGeometryPluginBase.h"

namespace bv { namespace model {

class DefaultCirclePluginDesc : public DefaultGeometryPluginDescBase
{
public:
    DefaultCirclePluginDesc() 
        : DefaultGeometryPluginDescBase( UID(), "circle" ) { }

    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();
};

class DefaultCirclePlugin : public DefaultGeometryPluginBase
{
    //virtual void										InitGeometry();

    virtual IGeometryAndUVsGenerator*           GetGenerator() override;

public:
    DefaultCirclePlugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model ) 
        : DefaultGeometryPluginBase( name, uid, prev, model ) { }
};

} }