#pragma once



namespace bv
{
class RemoteCommandsConverter;

class IEventServer
{
private:
protected:
public:
    ~IEventServer() {}

    virtual bool            InitializeServer    ( RemoteCommandsConverter* converter ) = 0;

    /// For now I assume, there's only one one implementation and this function is enough.
    /// Maybe in future we should create a factory or something.
    static IEventServer*    CreateServerObject  ();
};



} //bv
