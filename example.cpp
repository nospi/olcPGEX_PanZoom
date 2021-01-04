#define OLC_PGE_APPLICATION
#define OLC_PGEX_PANZOOM
#include <olcPixelGameEngine.h>
#include <olcPGEX_PanZoom.h>

class olcPanZoomTest : public olc::PixelGameEngine
{
public:
    olcPanZoomTest()
    {
        sAppName = "PanZoom Fun!";
    }

    bool OnUserCreate() override
    {
        pz.Create(this);
        pz.SetOffset({ (float)-ScreenWidth() * 0.5f, (float)-ScreenHeight() * 0.5f });
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // mouse pan
        if (GetMouse(2).bPressed)
            pz.StartPan();
        if (GetMouse(2).bReleased)
            pz.StopPan();
        
        // mousewheel zoom - zooms to mouse
        if (GetMouseWheel() > 0)
            pz.ZoomIn(1.1f);
        if (GetMouseWheel() < 0)
            pz.ZoomOut(0.9f);

        // keyboard pan
        float fPanFactor = (float)pz.ToScreenScale(50.0f);
        if (GetKey(olc::UP).bHeld)
            pz.NudgeUp(fPanFactor * fElapsedTime);
        if (GetKey(olc::DOWN).bHeld)
            pz.NudgeDown(fPanFactor * fElapsedTime);
        if (GetKey(olc::LEFT).bHeld)
            pz.NudgeLeft(fPanFactor * fElapsedTime);
        if (GetKey(olc::RIGHT).bHeld)
            pz.NudgeRight(fPanFactor * fElapsedTime);
        
        // keyboard zoom - zooms to screen center by passing false to 2nd arg of zoom functions
        if (GetKey(olc::Q).bHeld)
            pz.ZoomIn(1.01f, false);
        if (GetKey(olc::A).bHeld)
            pz.ZoomOut(0.99f, false);
        
        // don't forget to call the Update method on panzoom
        pz.Update(fElapsedTime);

        // get position in world space
        olc::vf2d topLeft, bottomRight;
        pz.ScreenToWorld({ 0, 0 }, topLeft);
        pz.ScreenToWorld({ ScreenWidth(), ScreenHeight() }, bottomRight);

        Clear(0);

        // draw axes
        olc::vi2d start, end;
        pz.WorldToScreen({ (float)topLeft.x, 0.0f }, start);
        pz.WorldToScreen({ (float)bottomRight.x, 0.0f }, end);
        DrawLine(start, end, olc::GREY, 0xF0F0F0F0);
        pz.WorldToScreen({ 0.0f, (float)topLeft.y }, start);
        pz.WorldToScreen({ 0.0f, (float)bottomRight.y }, end);
        DrawLine(start, end, olc::GREY, 0xF0F0F0F0);
        
        // accumulate time for circle movement
        fTime += fElapsedTime;

        // draw circle, rotating around the origin
        olc::vf2d worldPos { cos(fTime) * 30.0f, sin(fTime) * 30.0f };
        olc::vi2d screenPos;
        pz.WorldToScreen(worldPos, screenPos);  // translate from world pos to screen space
        int radius = pz.ToScreenScale(30.0f);   // scale a world distance to screen space distance
        DrawCircle(screenPos, radius);

        return !(GetKey(olc::ESCAPE).bPressed);
    }

private:
    olc::panzoom pz;
    float fTime = 0.0f;
};

int main()
{
    olcPanZoomTest app;
    app.Construct(1280, 720, 1, 1);
    app.Start();
    return 0;
}