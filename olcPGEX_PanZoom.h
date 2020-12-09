/*
        olcPGEX_PanZoom.h
        +-------------------------------------------------------------+
        |         OneLoneCoder Pixel Game Engine Extension            |
        |                 Simple Pan/Zoom 1.0                         |
        +-------------------------------------------------------------+
        What is this?
        ~~~~~~~~~~~~~
        Simple pan/zoom extenson for OLC PixelGameEngine. 
        Adapted from the panning & zooming tutorial.
        https://www.youtube.com/watch?v=ZQ8qtAizis4


        License (OLC-3)
        ~~~~~~~~~~~~~~~
        Copyright 2018 - 2020 OneLoneCoder.com
        Redistribution and use in source and binary forms, with or without
        modification, are permitted provided that the following conditions
        are met:
        1. Redistributions or derivations of source code must retain the above
        copyright notice, this list of conditions and the following disclaimer.
        2. Redistributions or derivative works in binary form must reproduce
        the above copyright notice. This list of conditions and the following
        disclaimer must be reproduced in the documentation and/or other
        materials provided with the distribution.
        3. Neither the name of the copyright holder nor the names of its
        contributors may be used to endorse or promote products derived
        from this software without specific prior written permission.
        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
        "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
        LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
        HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
        SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
        LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
        DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
        THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
        Links
        ~~~~~
        YouTube:	https://www.youtube.com/javidx9
        Discord:	https://discord.gg/WhwHUMV
        Twitter:	https://www.twitter.com/javidx9
        Twitch:		https://www.twitch.tv/javidx9
        GitHub:		https://www.github.com/onelonecoder
        Homepage:	https://www.onelonecoder.com
        Author: 	David Barr, aka javidx9, ©OneLoneCoder 2019, 2020

*/

#ifndef OLC_PGEX_PANZOOM_H
#define OLC_PGEX_PANZOOM_H

namespace olc {
    class panzoom : public olc::PGEX
    {
    private:
        olc::vd2d offset { 0.0, 0.0 };
        olc::vd2d startPan { 0.0, 0.0 };
        olc::vd2d scale { 1.0, 1.0 };
        olc::vd2d mouse;
        bool bPanning = false;

        olc::vd2d GetMouseVec()
        {
            return olc::vd2d((double)pge->GetMouseX(), (double)pge->GetMouseY());
        }

        void zoom(double dScaleMultiplier)
        {
            olc::vd2d mwPreZoom, mwPostZoom;
            ScreenToWorld(GetMouseVec(), mwPreZoom);
            scale *= dScaleMultiplier;
            ScreenToWorld(GetMouseVec(), mwPostZoom);
            offset += (mwPreZoom - mwPostZoom);
        }

    public:
        void WorldToScreen(const olc::vf2d& world, olc::vi2d& screen)
        {
            screen.x = (int)((world.x - (float)offset.x) * (float)scale.x);
            screen.y = (int)((world.y - (float)offset.y) * (float)scale.y);
        }

        void WorldToScreen(const olc::vd2d& world, olc::vi2d& screen)
        {
            screen.x = (int)((world.x - (double)offset.x) * (double)scale.x);
            screen.y = (int)((world.y - (double)offset.y) * (double)scale.y);
        }

        void ScreenToWorld(const olc::vi2d& screen, olc::vf2d& world)
        {
            world.x = (float)(screen.x) / (float)scale.x + (float)offset.x;
            world.y = (float)(screen.y) / (float)scale.y + (float)offset.y;
        }

        void ScreenToWorld(const olc::vi2d& screen, olc::vd2d& world)
        {
            world.x = (double)(screen.x) / (double)scale.x + (double)offset.x;
            world.y = (double)(screen.y) / (double)scale.y + (double)offset.y;
        }

        void SetOffset(const olc::vd2d& offset)
        {
            this->offset = offset;
        }

        bool Create(olc::PixelGameEngine *game)
        {
            pge = game;
            return true;
        }

        bool Update(float _fElapsedTime)
        {
            if (pge == nullptr) return false;
            if (bPanning)
            {
                offset -= (GetMouseVec() - startPan) / scale;
                startPan = GetMouseVec();
            }
            return true;
        }

        void StartPan()
        {
            startPan = GetMouseVec();
            bPanning = true;
        }

        void StopPan()
        {
            bPanning = false;
        }

        void ZoomIn(double dScaleMultiplier = 1.001)
        {
            if (dScaleMultiplier <= 1.0)
                dScaleMultiplier = 1.001;
            zoom(dScaleMultiplier);
        }

        void ZoomOut(double dScaleMultiplier = 0.999)
        {
            if (dScaleMultiplier >= 1.0)
                dScaleMultiplier = 0.999;
            zoom(dScaleMultiplier);
        }

        void NudgeLeft(double amount)
        {
            offset.x -= amount / scale.x;
        }

        void NudgeRight(double amount)
        {
            offset.x += amount / scale.x;
        }

        void NudgeUp(double amount)
        {
            offset.y -= amount / scale.y;
        }

        void NudgeDown(double amount)
        {
            offset.y += amount / scale.y;
        }
    };
}

#endif /* ifndef OLC_PGEX_PANZOOM_H */
