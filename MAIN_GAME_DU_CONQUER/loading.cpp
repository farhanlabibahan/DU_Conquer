#include "raylib.h"
#include <iostream>
#include "loading.h"
#include "global.h"
#include <string.h>


void loading_screen(std::string text)
{
    double t = GetTime()-time_loading_start;
    // std::cout<<"time :"<<t<<std::endl;
    float alpha = 1.0f;

    if(t <= 5)
    {
        if (t <= 2.0)
        {
            alpha = 1.0f;
        }
        else if (t > 2.0 && t <= 4.0)
        {
            alpha = 1.0f - ((t - 2.0f) / 2.0f);  // Fade out
        }
        else alpha = 0.0f;

        Color bgColor;
        if (t <= 2.0)
        {
            bgColor = Fade(WHITE, 1.0f);
        }
        else if (t > 2.0 && t <= 4.0)
        {
            float bgAlpha = (4.0f - t)/2.0f;
            bgColor = Fade(WHITE, bgAlpha);
        }
        else bgColor = Fade(WHITE, 0.0f); 

        Color fadeColor = Fade(BLACK, alpha);
        DrawRectangle(0, 0, screenWidth, screenHeight, fadeColor);
        DrawText(text.c_str(), 20*scale, screenHeight -160*scale, 30*scale, bgColor);
    }


    
}