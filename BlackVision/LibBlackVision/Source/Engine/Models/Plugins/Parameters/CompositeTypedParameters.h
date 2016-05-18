#pragma once

#include <cassert>

#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Mathematics/Transform/MatTransform.h"


namespace bv { namespace model {

// *******************************************
class ParamTransform : public AbstractModelParameter
{
private:

    CompositeTransform          m_transformModel;

public:

    explicit                    ParamTransform          ( const std::string & name, const CompositeTransform & transform, ITimeEvaluatorPtr evaluator );
                                ~ParamTransform         ();

    virtual void                Serialize               ( ISerializer & ser ) const override;
    void                        Serialize               ( ISerializer & ser, TransformKind kind ) const;

    virtual void                SetAddedKeyCurveType    ( CurveType type ) override;
    virtual void                SetGlobalCurveType      ( CurveType type ) override;
    virtual CurveType           GetCurveType            () override;

    virtual void                SetWrapPostMethod       ( WrapMethod method ) override;
    virtual void                SetWrapPreMethod        ( WrapMethod method ) override;
    virtual WrapMethod          GetWrapPostMethod       () override;
    virtual WrapMethod          GetWrapPreMethod        () override;

    virtual int                 GetNumKeys              () override;

    inline  void                SetRotation             ( const glm::vec3 & eulerAngle, TimeType t );
    inline  void                SetScale                ( const glm::vec3 & scale, TimeType t );
    inline  void                SetTranslation          ( const glm::vec3 & translation, TimeType t );
    inline  void                SetCenter               ( const glm::vec3 & center, TimeType t );

    inline  void                RemoveRotation          ( TimeType t );
    inline  void                RemoveScale             ( TimeType t );
    inline  void                RemoveTranslation       ( TimeType t );
    inline  void                RemoveCenter            ( TimeType t );

    inline  bool                MoveRotation            ( TimeType t, TimeType newTime );
    inline  bool                MoveScale               ( TimeType t, TimeType newTime );
    inline  bool                MoveTranslation         ( TimeType t, TimeType newTime );
    inline  bool                MoveCenter              ( TimeType t, TimeType newTime );
    
    inline  CompositeTransform & Transform              ();

    inline  glm::mat4           Evaluate                () const;

    inline  std::string         EvaluateToString        ( TimeType /*t*/ ) const { assert( false ); return ""; }

    virtual VoidPtr             QueryParamTyped         () override;

    const CompositeTransform &  GetTransform            () const;

    inline static  ModelParamType   Type                    ();

};


DEFINE_PTR_TYPE( ParamTransform )

} //model
} //bv

#include "CompositeTypedParameters.inl"
