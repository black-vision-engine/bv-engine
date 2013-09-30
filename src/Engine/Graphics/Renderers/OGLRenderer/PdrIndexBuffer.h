#pragma once

namespace bv
{

class Renderer;
class IndexBuffer;

class PdrIndexBuffer
{
public:

    PdrIndexBuffer( Renderer *, const IndexBuffer * ) {}

    void Enable(Renderer*) {}
};

} // bv