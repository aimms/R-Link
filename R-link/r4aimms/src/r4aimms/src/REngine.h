/*  This file is part of R-Link.

    R-Link is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    R-Link is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with R-Link.  If not, see <http://www.gnu.org/licenses/>.

*/
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
        bool rIsAlreadyLoaded();
    private:
        struct RModule; RModule* m_RModule;
    };

};