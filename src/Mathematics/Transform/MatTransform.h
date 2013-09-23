#pragma once

#include <vector>

#include "Mathematics/Interpolators/Interpolators.h"
#include "glm/glm.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

namespace bv
{
namespace model
{

class Transform
{
private:
    glm::mat4x4 m_mat;

public:
    Transform               operator *  (const Transform& m)        const;

    void                    SetMatrix   ( const glm::mat4x4& m );
    const glm::mat4x4&      GetMatrix   ()                          const;

    explicit                Transform   (const glm::mat4x4& m);
    Transform();
};

}
// ******************* TransformInterpolator **************** //

enum class TransformKind : int
{
    rotation,
    scale,
    translation,
};

template<typename ParamT>
class SimpleTransform
{
protected:
    TransformKind kind;

    ParamT p0; 
    ParamT p1;
    ParamT p2;

public:
    explicit SimpleTransform(TransformKind kind, ParamT p0, ParamT p1, ParamT p2);

    virtual glm::mat4x4 evaluate(typename ParamT::TimeT t) const;
};

template<typename ParamT>
class Rotation : public SimpleTransform<ParamT>
{
private:
    ParamT angle;

public:
    explicit Rotation(ParamT angle, ParamT p0, ParamT p1, ParamT p2);

    virtual glm::mat4x4 evaluate(typename ParamT::TimeT t) const override;
};

template<typename ParamT>
class CompositeTransform : public Interpolator<typename ParamT::TimeT>
{
private:
    std::vector<SimpleTransform<ParamT>*> transformations;
public:

    static const int value_size = sizeof(glm::mat4x4);

    void addTranslation(ParamT x0, ParamT x1, ParamT x2);
    void addScale(ParamT s0, ParamT s1, ParamT s2);
    void addRotation(ParamT angle, ParamT r0, ParamT r1, ParamT r2);
    void addTransform(SimpleTransform<ParamT>* trans);

    unsigned int size() const;

    SimpleTransform<ParamT>&        operator[](unsigned int i);
    const SimpleTransform<ParamT>&  operator[](unsigned int i) const;

    glm::mat4x4 evaluate(typename ParamT::TimeT t) const;

    explicit CompositeTransform();
    virtual ~CompositeTransform();

    int evalToCBuffer(typename ParamT::TimeT,char *) const;
};

template<typename ParamT>
glm::mat4x4 Rotation<ParamT>::evaluate(typename ParamT::TimeT t) const
{
    return glm::rotate(glm::mat4(1.0f), (float)angle.evaluate(t), glm::vec3(p0.evaluate(t), p1.evaluate(t), p2.evaluate(t)));
}

typedef CompositeTransform<FloatInterpolator> TransformF;
typedef SimpleTransform<FloatInterpolator> SimpleTransformF;
typedef Rotation<FloatInterpolator> RotationF;
}
