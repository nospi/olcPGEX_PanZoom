/*
        olcPGEX_PanZoom.h
        +-------------------------------------------------------------+
        |         OneLoneCoder Pixel Game Engine Extension            |
        |                 Simple Pan/Zoom 1.0                         |
        +-------------------------------------------------------------+
        What is this?
        ~~~~~~~~~~~~~
        Simple pan/zoom extenson for OLC PixelGameEngine. 
        Adapted from the panning & zooming tutorial: [Link](https://www.youtube.com/watch?v=ZQ8qtAizis4)


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
        Author      https://github.com/nospi
        ~~~~~~
        David Barr, aka javidx9, ©OneLoneCoder 2019, 2020


        Example program, adapted from javidx9's pan/zoom tutorial:
        ~~~~~~
        #define OLC_PGE_APPLICATION
        #include "olcPixelGameEngine.h"
        #include "olcPGEX_PanZoom.h"

        class PanZoomDemo : public olc::PixelGameEngine
        {
        private:
            olc::panzoom pz;

        public:
            PanZoomDemo()
            {
                sAppName = "Pan/Zoom PGEX Demo";
            }

            bool OnUserCreate() override
            {
                pz.Create(this);    // pass instance of PGE
                return true;
            }

            bool OnUserUpdate(float fElapsedTime)
            {
                Clear(0);

                // pass input to panzoom
                if (GetMouse(0).bPressed)
                    pz.StartPan();
                if (GetMouse(0).bReleased)
                    pz.StopPan();
                if (GetKey(olc::Q).bHeld)
                    pz.ZoomIn(1.001f);  // optional fScaleMultiplier here -- not clamped, can allow ZoomIn to actually zoom out..
                if (GetKey(olc::A).bHeld)
                    pz.ZoomOut();
                
                // call pz update method
                pz.OnUpdate(fElapsedTime);

                // draw vertical lines
                for (float y = 0.0f; y <= 10.0f; y++)
                {
                    olc::vf2d w_start, w_end;
                    olc::vi2d s_start, s_end;
                    w_start = { 0.0f, y };
                    w_end = { 10.0f, y };
                    pz.WorldToScreen(w_start, s_start);
                    pz.WorldToScreen(w_end, s_end);
                    DrawLine(s_start, s_end);
                }

                // draw horizontal lines
                for (float x = 0.0f; x <= 10.0f; x++)
                {
                    olc::vf2d w_start, w_end;
                    olc::vi2d s_start, s_end;
                    w_start = { x, 0.0f };
                    w_end = { x, 10.0f };
                    pz.WorldToScreen(w_start, s_start);
                    pz.WorldToScreen(w_end, s_end);
                    DrawLine(s_start, s_end);
                }

                return true;
            }
        };

        int main()
        {
            PanZoomDemo demo;
            demo.Construct(320, 200, 4, 4);
            demo.Start();
            return 0;
        }
        ~~~~~~

*/

#ifndef OLC_PGEX_PANZOOM_H
#define OLC_PGEX_PANZOOM_H

namespace olc {
    class panzoom : olc::PGEX
    {
    private:
        olc::vf2d offset { 0.0f, 0.0f };
        olc::vf2d startPan { 0.0f, 0.0f };
        olc::vf2d scale { 1.0f, 1.0f };
        olc::vf2d mouse;
        bool bPanning = false;

        olc::vf2d GetMouseVec()
        {
            return olc::vf2d((float)pge->GetMouseX(), (float)pge->GetMouseY());
        }

    public:
        void WorldToScreen(olc::vf2d world, olc::vi2d& screen)
        {
            screen.x = (int)((world.x - offset.x) * scale.x);
            screen.y = (int)((world.y - offset.y) * scale.y);
        }

        void ScreenToWorld(olc::vi2d screen, olc::vf2d& world)
        {
            world.x = (float)(screen.x) / scale.x + offset.x;
            world.y = (float)(screen.y) / scale.y + offset.y;
        }

        bool Create(olc::PixelGameEngine *game)
        {
            pge = game;
            offset.x = -(float)pge->ScreenWidth() / 2.0f;
            offset.y = -(float)pge->ScreenHeight() / 2.0f;
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

        void ZoomIn(float fScaleMultiplier = 1.001f)
        {
            olc::vf2d mwPreZoom, mwPostZoom;    // mouse world pos pre and post zoom
            ScreenToWorld(GetMouseVec(), mwPreZoom);
            scale *= fScaleMultiplier;
            ScreenToWorld(GetMouseVec(), mwPostZoom);
            offset += (mwPreZoom - mwPostZoom);
        }

        void ZoomOut(float fScaleMultiplier = 0.999f)
        {
            olc::vf2d mwPreZoom, mwPostZoom;    // mouse world pos pre and post zoom
            ScreenToWorld(GetMouseVec(), mwPreZoom);
            scale *= fScaleMultiplier;
            ScreenToWorld(GetMouseVec(), mwPostZoom);
            offset += (mwPreZoom - mwPostZoom);
        }

        bool OnUpdate(float _fElapsedTime)
        {
            if (pge == nullptr) return false;

            // handle panning
            if (bPanning)
            {
                offset -= (GetMouseVec() - startPan) / scale;
                startPan = GetMouseVec();
            }

            return true;
        }
        
    };
}

#endif /* ifndef OLC_PGEX_PANZOOM_H */