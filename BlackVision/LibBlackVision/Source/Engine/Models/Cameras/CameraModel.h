#pragma once

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Interfaces/IUpdatable.h"
#include "Engine/Models/Plugins/Interfaces/IParamSet.h"
#include "Engine/Interfaces/IValueSet.h"
#include "Serialization/ISerializable.h"



namespace bv { namespace model {



/**@brief Camera model.

@section Camera_Parameters Cameras parameters

Key                                     | Values        | Default Value         | Description
--------------------------------------- | ------------- | --------------------- | -----------
IsPerspective                           | bool          | true                  | Perspective or orthogonal camera.
ViewportSize                            | float         | 2.0f                  | Size of viewport. (Orthogonal camera only)
Direction                               | vec3          | 0.0f, 0.0f, -1.0f     | Camera view direction.
Position                                | vec3          | 0.0f, 0.0f, 5.0f      | Camera position.
FOV                                     | float         | 45.0f                 | Camera angle in y-axis in degrees.
FarClippingPlane                        | float         | 100.0f                | Far camera clipping plane.
NearClippingPlane                       | float         | 0.1f                  | Near camera clipping plane.
UpVector                                | vec3          | 0.0f, 1.0f, 0.0f      | Camera top direction.


@ingroup Model*/
class CameraModel : public IUpdatable, public ISerializable, public IParamSet, public IValueSet
{
public:

    struct PARAMETERS
    {
        static const std::string    IS_PERSPECTIVE;
        static const std::string    POSITION;
        static const std::string    DIRECTION;
        static const std::string    UP_VECTOR;
        static const std::string    FOV;
        static const std::string    NEAR_CLIPPING_PLANE;
        static const std::string    FAR_CLIPPING_PLANE;
        static const std::string    VIEWPORT_SIZE;
    };

private:
protected:

    DefaultParamValModelPtr     m_paramModel;

public:
    explicit                    CameraModel         ( ITimeEvaluatorPtr timeEvaluator );
                                ~CameraModel        ();


    virtual void                Serialize           ( ISerializer & ser ) const override;
    static ISerializablePtr		Create              ( const IDeserializer & deser );

    virtual void				Update			    ( TimeType t )	override;
    bool                        ParameterChanged    ( const std::string & name );
    bool                        StateChanged        ();

    virtual std::vector< IParameterPtr > &          GetParameters   () override;
    virtual IParameterPtr                           GetParameter    ( const std::string & name ) override;

    virtual const std::vector< IValueConstPtr > &   GetValues       () const override;
    virtual IValueConstPtr                          GetValue        ( const std::string & name ) const override;

};

DEFINE_PTR_TYPE(CameraModel)
DEFINE_CONST_PTR_TYPE(CameraModel)


} // model
} //bv
