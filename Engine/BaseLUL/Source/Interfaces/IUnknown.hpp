#pragma once

#define LUL_SET_IUNKNOWN(className)             \
public:                                         \
std::string Interface() { return #className; }  \
private:

namespace LUL
{
    class LUL_DLL IUnknown
    {
    public:

        IUnknown() = default;

        ~IUnknown() = default;

    public:
        
        std::string GetInterfaceName() { return Interface(); }

    protected:
        
        virtual std::string Interface() = 0;

    };
}