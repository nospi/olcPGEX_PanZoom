# olcPGEX_PanZoom
Basic Pan/Zoom extension for [olc::PixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine).
Adapted from javidx9's [panning & zooming tutorial](https://www.youtube.com/watch?v=ZQ8qtAizis4).

# Getting Started
To begin with you'll want to declare an `olc::panzoom` somewhere accessible in your application. A private member variable of your derived PGE class is suitable.

```c++
class MyApp : public olc::PixelGameEngine
{
private:
  olc::panzoom pz;
};
```

The extension requires a pointer to your derived PGE class. You can provide it with one within your `OnUserCreate` method. You can also optionally set the offset with an `olc::vf2d` here. To emulate a euclidean plane with (0,0) at the center, you can set the offset to `(-ScreenWidth() / 2, -ScreenHeight() / 2)`:

```c++
bool OnUserCreate() override
{
  pz.Create(this);
  pz.SetOffset({ (float)-ScreenWidth() * 0.5f, (float)-ScreenHeight() * 0.5f });
}
```

Input handling is left to the user and instead several methods are presented: `StartPan`, `StopPan`, `ZoomIn`, `ZoomOut`, `NudgeLeft`, `NudgeRight`, `NudgeUp`, `NudgeDown`. The extension also provides an `Update` method which must be called once per frame in order to process panning. **NOTE:** the Nudge methods nudge the 'Camera', not the world.

The following basic example implements panning with the middle mouse button, and zooming with the mouse wheel:

```c++
bool OnUserUpdate(float fElapsedTime)
{
  if (GetMouse(2).bPressed)
    pz.StartPan();
  if (GetMouse(2).bReleased)
    pz.StopPan();
  
  if (GetMouseWheel() > 0)
    pz.ZoomIn(1.1f);
  if (GetMouseWheel() < 0)
    pz.ZoomOut(0.9f);
  
  // call the panzoom Update method or panning won't work
  pz.Update(fElapsedTime);
}
```

The `ZoomIn` and `ZoomOut` methods have an optional `float fScaleFactor` argument should you wish to have finer control over the speed of the zoom. Depending on whether keyboard or mousewheel zoom is used you may want to adjust these - the mousewheel moves in discrete steps and requires a larger 'jump'; occurring in a single frame as opposed to keyboard zoom will also effect this.

```c++
if (GetKey(olc::Q).bHeld)
  pz.ZoomIn(1.01f);
if (GetMouseWheel() > 0)
  pz.ZoomIn(1.1f);
if (GetKey(olc::A).bHeld)
  pz.ZoomOut(0.99f);
if (GetMouseWheel() < 0)
  pz.ZoomOut(0.9f);
```

By default, zooming will occur around the mouse cursor. If you'd prefer to disable this and instead zoom around the screen center, there is a second optional `bool zoomToMouse` argument for the `ZoomIn` and `ZoomOut` methods.

```c++
// keyboard zoom on center (not mouse)
if (GetKey(olc::Q).bHeld)
  pz.ZoomIn(1.01f, false);
if (GetKey(olc::A).bHeld)
  pz.ZoomOut(0.99f, false);
```

None of this is of any use unless we use it to transform co-ordinates between world-space and screen-space. This can be performed with the `ScreenToWorld` and `WorldToScreen` methods. The following example converts the top-left and bottom-right of the screen into co-ordinates in world space, and plots the function `y = x` - taking the values from world space and drawing them in screen space.

```c++
olc::vf2d wldTopLeft, wldBottomRight;
pz.ScreenToWorld({ 0, 0 }, wldTopLeft);
pz.ScreenToWorld({ ScreenWidth(), ScreenHeight() }, wldBottomRight);

olc::vi2d previousPos;  // used to draw lines between co-ordinates
for (float x = wldTopLeft.x; x < wldBottomRight.x; x++)
{
  float y = x;                          // perform the basic function y = x
  olc::vi2d scrPos;                     // declare an olc::vi2d to hold our screen pos
  pz.WorldToScreen({ x, y }, scrPos);   // convert from world to screen space
  if (x != wldTopLeft.x)                // if not the first iteration of this loop:
    DrawLine(previousPos, scrPos);      // draw a line between previousPos and current scrPos
  previousPos = scrPos;                 // update previousPos with current scrPos
}
```

There are instances where you may also want to scale a scalar value based on the zoom factor of the panzoom instance - such as calculating the screen-space radius of a circle.

```c++
olc::vi2d scrCirclePos;
int scrRadius;

pz.WorldToScreen({ 0.0f, 0.0f }, scrCirclePos);   // center of circle at origin (0,0) from world space to screen
scrRadius = pz.ToScreenScale(30.0f);              // scale radius of 30.0f in world space to screen-space
DrawCircle(scrCirclePos, scrRadius); 
DrawLine(scrCirclePos, { scrCirclePos.x + scrRadius, scrCirclePos.y });

// and to calculate value at world scale
float wldRadius = pz.ToWorldScale(scrRadius);     // should be close to 30.0f depending on current zoom factor
```

# Example
Please see the `example.cpp` file for a full working demo. Don't forget to `#define OLC_PGEX_PANZOOM` alongside `OLC_PGE_APPLICATION`.

# License (OLC-3)
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
YouTube:	https://www.youtube.com/javidx9
Discord:	https://discord.gg/WhwHUMV
Twitter:	https://www.twitter.com/javidx9
Twitch:		https://www.twitch.tv/javidx9
GitHub:		https://www.github.com/onelonecoder
Homepage:	https://www.onelonecoder.com
Author:   David Barr, aka javidx9, ©OneLoneCoder 2019, 2020
~~~~~~~~~~~~~~~
