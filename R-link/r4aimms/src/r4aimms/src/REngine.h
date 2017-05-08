#pragma once

#include <vector>
#include <string>

namespace r4aimms{

    class REngine{
    public:
        REngine();
        virtual ~REngine();

        virtual void initialize(const wchar_t* szHomeFolderR);
        virtual void executeScript(const wchar_t* szScript);
        virtual void terminate();
    private:
        struct RModule; RModule* m_RModule;
    };

};