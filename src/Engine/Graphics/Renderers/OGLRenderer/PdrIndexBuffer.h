#pragma once

namespace bv
{

class Renderer;
class IndexBuffer;

class PdrIndexBuffer
{
public:
    PdrIndexBuffer(Renderer*, IndexBuffer*){}

    void Enable(Renderer*) {}
};

} // bv