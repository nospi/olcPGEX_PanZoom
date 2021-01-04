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
#include <olcPixelGameEngine.h>

// definition
namespace olc
{
    class panzoom : public olc::PGEX
    {
    public:
        void WorldToScreen(const olc::vf2d& world, olc::vi2d& screen);
        void ScreenToWorld(const olc::vi2d& screen, olc::vf2d& world);
        bool Create(olc::PixelGameEngine*);
        bool Update(float);
        void ZoomIn(float fScaleFactor = 1.001f, bool zoomToMouse = true);
        void ZoomOut(float fScaleFactor = 0.999f, bool zoomToMouse = true);
        void NudgeLeft(float amount);
        void NudgeRight(float amount);
        void NudgeUp(float amount);
        void NudgeDown(float amount);
        void StartPan();
        void StopPan();
        int ToScreenScale(float w);
        float ToWorldScale(int s);
        void SetOffset(const olc::vf2d& offset);
        void SetScale(const olc::vf2d& scale);

    private:
        olc::PixelGameEngine* pge;
        olc::vf2d offset { 0.0f, 0.0f };
        olc::vf2d startPan { 0.0f, 0.0f };
        olc::vf2d scale { 1.0f, 1.0f };
        olc::vf2d mouse;
        bool bPanning = false;
        olc::vf2d GetMouseVec();
        void zoom(float fScaleFactor);
        void zoom_to_mouse(float fScaleFactor);
    };
};

// implementation
#ifdef OLC_PGEX_PANZOOM

olc::vf2d olc::panzoom::GetMouseVec()
{
    return (olc::vf2d)pge->GetMousePos();
}

void olc::panzoom::zoom(float fScaleFactor)
{
    olc::vf2d cPre, cPost;
    ScreenToWorld({ pge->ScreenWidth() >> 1, pge->ScreenHeight() >> 1 }, cPre);
    scale *= fScaleFactor;
    ScreenToWorld({ pge->ScreenWidth() >> 1, pge->ScreenHeight() >> 1 }, cPost);
    offset += cPre - cPost;
}

void olc::panzoom::zoom_to_mouse(float fScaleFactor)
{
    olc::vf2d mwPreZoom, mwPostZoom;
    ScreenToWorld(GetMouseVec(), mwPreZoom);
    scale *= fScaleFactor;
    ScreenToWorld(GetMouseVec(), mwPostZoom);
    offset += (mwPreZoom - mwPostZoom);
}

void olc::panzoom::WorldToScreen(const olc::vf2d& world, olc::vi2d& screen)
{
    screen.x = (int)((world.x - (float)offset.x) * (float)scale.x);
    screen.y = (int)((world.y - (float)offset.y) * (float)scale.y);
}

void olc::panzoom::ScreenToWorld(const olc::vi2d& screen, olc::vf2d& world)
{
    world.x = (float)(screen.x) / (float)scale.x + (float)offset.x;
    world.y = (float)(screen.y) / (float)scale.y + (float)offset.y;
}

void olc::panzoom::SetOffset(const olc::vf2d& offset)
{
    this->offset = offset;
}

void olc::panzoom::SetScale(const olc::vf2d& scale)
{
    this->scale = scale;
}

bool olc::panzoom::Create(olc::PixelGameEngine *game)
{
    pge = game;
    return true;
}

bool olc::panzoom::Update(float _fElapsedTime)
{
    if (pge == nullptr) return false;
    if (bPanning)
    {
        offset -= (GetMouseVec() - startPan) / scale;
        startPan = GetMouseVec();
    }
    return true;
}

void olc::panzoom::StartPan()
{
    startPan = GetMouseVec();
    bPanning = true;
}

void olc::panzoom::StopPan()
{
    bPanning = false;
}

void olc::panzoom::ZoomIn(float fScaleFactor, bool zoomToMouse)
{
    if (fScaleFactor <= 1.0)
        fScaleFactor += 0.001;
    zoomToMouse ? zoom_to_mouse(fScaleFactor) : zoom(fScaleFactor);
}

void olc::panzoom::ZoomOut(float fScaleFactor, bool zoomToMouse)
{
    if (fScaleFactor >= 1.0)
        fScaleFactor -= 0.001;
    zoomToMouse ? zoom_to_mouse(fScaleFactor) : zoom(fScaleFactor);
}

void olc::panzoom::NudgeLeft(float amount)
{
    offset.x -= amount / scale.x;
}

void olc::panzoom::NudgeRight(float amount)
{
    offset.x += amount / scale.x;
}

void olc::panzoom::NudgeUp(float amount)
{
    offset.y -= amount / scale.y;
}

void olc::panzoom::NudgeDown(float amount)
{
    offset.y += amount / scale.y;
}

int olc::panzoom::ToScreenScale(float w)
{
    return (int)(w * scale.x);
}

float olc::panzoom::ToWorldScale(int s)
{
    return (float)(s / scale.x);
}

#endif /* ifdef OLC_PGEX_PANZOOM */

#endif /* ifndef OLC_PGEX_PANZOOM_H */