#pragma once

#define ANNOUNCE_AS_IUNKNOWN(aClass)                            \
public:                                                         \
    const std::string GetClass() override { return #aClass; }   \
private:                                                        

namespace LUL
{
    class LUL_DLL IUnknown
    {
    public:

        IUnknown() = default;

        ~IUnknown() = default;

    public:

        virtual const std::string GetClass() { return "IUnknown"; }

    };
}