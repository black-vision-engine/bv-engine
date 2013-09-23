#include "Mathematics/Transform/MatTransform.h"

namespace bv
{

namespace model
{

Transform::Transform()
    : m_mat(glm::mat4x4(1.f))
{}

Transform::Transform(const glm::mat4x4& m)
    :   m_mat(m)
{}

Transform               Transform::operator*    (const Transform& m)    const
{
    return Transform(m_mat * m.m_mat);
}

void     Transform::SetMatrix   ( const glm::mat4x4& m )
{
    m_mat = m;
}

const glm::mat4x4&      Transform::GetMatrix   ()                       const
{
    return m_mat;
}

} // model

template<typename ParamT>
SimpleTransform<ParamT>::SimpleTransform(TransformKind kind, ParamT p0, ParamT p1, ParamT p2)
    : kind(kind), p0(p0), p1(p1), p2(p2)
{}

template<typename ParamT>
Rotation<ParamT>::Rotation(ParamT angle, ParamT p0, ParamT p1, ParamT p2)
    :   SimpleTransform(TransformKind::rotation,p0, p1, p2)
    ,   angle(angle)
{}

template<typename ParamT>
CompositeTransform<ParamT>::~CompositeTransform()
{
    for(unsigned int i = 0; i < transformations.size(); ++i)
    {
        delete transformations[i];
    }
}

template<typename ParamT>
int CompositeTransform<ParamT>::evalToCBuffer(typename ParamT::TimeT t,char * buf) const
{
    glm::mat4x4 val = evaluate(t);

    memcpy(buf, &val, value_size);

    return value_size;
}

template<typename ParamT>
void CompositeTransform<ParamT>::addTranslation(ParamT x0, ParamT x1, ParamT x2)
{
    transformations.push_back(new SimpleTransform<ParamT>(TransformKind::translation, x0, x1, x2));
}

template<typename ParamT>
void CompositeTransform<ParamT>::addScale(ParamT s0, ParamT s1, ParamT s2)
{
    transformations.push_back(new SimpleTransform<ParamT>(TransformKind::scale, s0, s1, s2));
}

template<typename ParamT>
void CompositeTransform<ParamT>::addRotation(ParamT angle, ParamT r0, ParamT r1, ParamT r2)
{
    transformations.push_back(new Rotation<ParamT>(angle, r0, r1, r2));
}

template<typename ParamT>
void CompositeTransform<ParamT>::addTransform(SimpleTransform<ParamT>* trans)
{
    transformations.push_back(trans);
}

template<typename ParamT>
CompositeTransform<ParamT>::CompositeTransform()
{}

template<typename ParamT>
unsigned int CompositeTransform<ParamT>::size() const
{
    return transformations.size();
}

template<typename ParamT>
SimpleTransform<ParamT>& CompositeTransform<ParamT>::operator[](unsigned int i)
{
    assert(transformations[i] != nullptr);
    return *transformations[i];
}

template<typename ParamT>
const SimpleTransform<ParamT>& CompositeTransform<ParamT>::operator[](unsigned int i) const
{
    assert(transformations[i] != nullptr);
    return *transformations[i];
}

template<typename ParamT>
glm::mat4x4 CompositeTransform<ParamT>::evaluate(typename ParamT::TimeT t) const
{
    glm::mat4x4 ret(1.0f);
    for(unsigned int i = 0; i < transformations.size(); ++i)
    {
        ret *= transformations[i]->evaluate(t);
    }

    return ret; 
}

template<typename ParamT>
glm::mat4x4 SimpleTransform<ParamT>::evaluate(typename ParamT::TimeT t) const
{
    switch(kind)
    {
    case TransformKind::translation:
        return glm::translate(glm::mat4(1.0f), glm::vec3(p0.evaluate(t), p1.evaluate(t), p2.evaluate(t)));
    case TransformKind::scale:
        return glm::scale(glm::mat4(1.0f), glm::vec3(p0.evaluate(t), p1.evaluate(t), p2.evaluate(t)));
    case TransformKind::rotation:
        assert(false);
        return glm::mat4(1.0f);
    default:
        assert(false);
        return glm::mat4(1.0f);
    }
}

}

template class bv::CompositeTransform<bv::FloatInterpolator>;
