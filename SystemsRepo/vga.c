#include "include/vga.h"
#include "include/Vector2.h"
#include <stdint.h>


uint16_t *memAddress = (uint16_t *)0xA0000;

uint16_t screenWidth = 320;
uint16_t screenHeight = 200;

void SetPixel(Vector2 *pos)
{
    uint16_t y = pos->y;
    uint16_t x = pos->x;
    uint8_t colour = pos->colour;
    // Set a location on screen in memory to a specific colour
    memAddress[screenWidth * y+x] = colour;
}

void ClearScreen()
{
    uint16_t x = 0;
    uint16_t y = 0;

   // Go through all pixels on screen and set colour to black
    for (y = 0; y < screenHeight; y++)
    {
        for(x = 0; x < screenWidth; x++)
        {
            memAddress[screenWidth*y+x] = 0x0;            
        }
    }
}


void DrawRectangle(Vector2* origin, uint16_t width, uint16_t height)
{
    // Draw an unfilled rectangle of specified width and height
    // by drawing 4 lines 

    // scale each point by adding the x and y values from the origin
    width += origin->x;
    height += origin->y;

    if (origin->x > width) 
    {
        uint16_t temp = origin->x; origin->x = width; width = temp;
    }
    if (origin->y > height) 
    {
        uint16_t temp = origin->y; origin->y = height; height = temp;
    } 

    // set each corner
    Vector2 p1 = {origin->x, origin->y, origin->colour};
    Vector2 p2 = {width, p1.y, origin->colour};
    Vector2 p3 = {width, height, origin->colour};
    Vector2 p4 = {p1.x, height, origin->colour};

    //ref variables
    Vector2 *point1 = &p1;
    Vector2 *point2 = &p2;
    Vector2 *point3 = &p3;
    Vector2 *point4 = &p4;

    Vector2 t = {p1.x, p2.y, p1.colour};
    Vector2 *temp = &t;
    SetPixel(temp);   // set a pixel here because the first pixel would not be drawn
    DrawLine(point1, point2, 0); // top left to top right
    DrawLine(point2, point3, 0); // top left to bottom right
    DrawLine(point4, point3, 0); // bottom right to bottom left
    DrawLine(point1, point4, 0); // bottom left to top left
}


void DrawFilledRectangle(Vector2 *origin, uint16_t width, uint16_t height)
{
    // duplicated code here, I could change it so draw rectangle takes a 
    // struct of points that would be passed here instead of drawing a seperate
    // shape.

    // Draw an filled rectangle of specified width and height
    // by drawing 4 lines 
    // scale each point by adding the x and y values from the origin
    width += origin->x;
    height += origin->y;

    if (origin->x > width) 
    {
        uint16_t temp = origin->x; origin->x = width; width = temp;
    }
    if (origin->y > height) 
    {
        uint16_t temp = origin->y; origin->y = height; height = temp;
    } 

    Vector2 p1 = {origin->x, origin->y, origin->colour};
    Vector2 p2 = {width, p1.y, origin->colour};
    Vector2 p3 = {width, height, origin->colour};
    Vector2 p4 = {p1.x, height, origin->colour};

    //ref variables
    Vector2 *point1 = &p1;
    Vector2 *point2 = &p2;
    Vector2 *point3 = &p3;
    Vector2 *point4 = &p4;

    Vector2 t = {p1.x, p2.y, p1.colour};
    Vector2 *temp = &t;
    SetPixel(temp);   // set a pixel here because the first pixel would not be drawn
    DrawLine(point1, point2, 0); // top left to top right
    DrawLine(point2, point3, 0); // top left to bottom right
    DrawLine(point4, point3, 0); // bottom right to bottom left
    DrawLine(point1, point4, 0); // bottom left to top left

    // fill the rectangle by drawing a line from each point
    // in between the top left and bottom left points
    uint16_t test = p4.y - p1.y;

    for(uint16_t i = 0; i < test; i++){
        Vector2 q = {p1.x,p1.y + i,p1.colour};
        Vector2 *qq = &q;
        
        Vector2 w = {p2.x, p1.y + i, p1.colour};
        Vector2 *ww = &w;
        DrawLine(qq, ww, 0);
    }
}


void DrawLine(Vector2* initialPos, Vector2* finalPos, uint16_t thickness)
{
    // draw a line using Bresenhams Line Algorithm

    // Get distance and absolute distance between each point
    uint16_t dx = finalPos->x - initialPos->x; 
    uint16_t dy = finalPos->y - initialPos->y;
    uint16_t dxabs = abs(dx);
    uint16_t dyabs = abs(dy);
    uint16_t x = dyabs / 2;
    uint16_t y = dxabs / 2;
    uint16_t px = initialPos->x;
    uint16_t py = initialPos->y;

    // if line slope is more horizontal
    if(dxabs >= dyabs)
    {
        // go through each point
        for(uint16_t i = 0; i < dxabs; i++)
        {
            y += dyabs;

            if(y >= dxabs)
            {
                y -= dxabs;
                py += sgn(dy);
            }

            px += sgn(dx);
            Vector2 t = {px,py, initialPos->colour};
            Vector2 *temp = &t;
            SetPixel(temp);

            // draw the line of specified thickness
            uint16_t count = 1;
            uint16_t thickLevel = thickness;

            for(uint16_t i = thickLevel; i > 1; i)
            {
                //while thick level is above 1
                // draw above the line
                t.y += count;
                Vector2 *temp = &t;
                SetPixel(temp);
                i--;
                count++;

                if(i)
                {
                    // draw below the line
                    t.y -= count;
                    Vector2 *temp = &t;
                    SetPixel(temp);
                }
            }
        }
    }
    else // line slope is more vertical
    {
        for(uint16_t j = 0; j < dyabs; j++)
        {
            x += dxabs;

            if(x >= dyabs)
            {
                x -= dyabs;
                px += sgn(dx);
            }

            py += sgn(dy);
            Vector2 t = {px,py, initialPos->colour};
            Vector2 *temp = &t;
            SetPixel(temp);

            uint16_t count = 1;
            uint16_t thickLevel = thickness;

            for(uint16_t i = thickLevel; i > 1; i)
            {
                t.x += count;
                Vector2 *temp = &t;
                SetPixel(temp);
                i--;
                count++;

                if(i)
                {
                    t.x -= count;
                    Vector2 *temp = &t;
                    SetPixel(temp);
                }
            }
        }
    } 
}


void DrawCircle(Vector2* origin, uint16_t radius, bool filled)
{
    // Draw a 'circle' using Midpoint Algorithm
    uint16_t originX = origin->x;
    uint16_t originY = origin->y;
    uint16_t x = radius;
    uint16_t y = 0;
    uint16_t error = 0;

    uint16_t colour = origin->colour;

    while(x >= y)
    {
        // Get our circle points now to prevent duplicated code
        Vector2  t1 = {originX + x, originY + y, colour};
        Vector2 *temp1 = &t1;
        Vector2  t2 = {originX + y, originY + x, colour};
        Vector2 *temp2 = &t2;
        Vector2  t3 = {originX - y, originY + x, colour};
        Vector2 *temp3 = &t3;
        Vector2  t4 = {originX - x, originY + y, colour};
        Vector2 *temp4 = &t4;
        Vector2  t5 = {originX - x, originY - y, colour};
        Vector2 *temp5 = &t5;
        Vector2  t6 = {originX - y, originY - x, colour};
        Vector2 *temp6 = &t6;
        Vector2  t7 = {originX + y, originY - x, colour};
        Vector2 *temp7 = &t7;
        Vector2  t8 = {originX + x, originY - y, colour};
        Vector2 *temp8 = &t8;
       
        // draw unfilled shape
        if(!filled)
        {
            SetPixel(temp1);
            SetPixel(temp2);
            SetPixel(temp3);
            SetPixel(temp4);
            SetPixel(temp5);
            SetPixel(temp6);
            SetPixel(temp7);
            SetPixel(temp8);
        }
        else
        {
        // Fill the circle by drawing from between each reflected point
        Vector2 f1 = {t4.x, t4.y, t1.colour};
        Vector2 f2 = {t1.x, t1.y, t1.colour};
        Vector2 *test1 = &f1;
        Vector2 *test2 = &f2;
        DrawLine(test1, test2, 0);

        Vector2 f3 = {t5.x, t5.y, t5.colour};
        Vector2 f4 = {t8.x, t8.y, t8.colour};
        Vector2 *test3 = &f3;
        Vector2 *test4 = &f4;
        DrawLine(test3, test4, 0);     
        
        Vector2 f5 = {t2.x, t2.y, t2.colour};
        Vector2 f6 = {t3.x, t3.y, t3.colour};
        Vector2 *test5 = &f5;
        Vector2 *test6 = &f6;
        DrawLine(test6, test5, 0);

        Vector2 f7 = {t6.x, t6.y, t6.colour};
        Vector2 f8 = {t7.x, t7.y, t7.colour};
        Vector2 *test7 = &f7;
        Vector2 *test8 = &f8;
        DrawLine(test7, test8, 0);

        }

       
        y++;
        error += 1 + 2 * y;
        if(2 * (error - x) + 1 > 0)
        {
            x -= 1;
            error += 1 - 2 * x;
        }
    }
}


// Math functions for Bresenhams line algorithm
uint16_t abs(uint16_t value)
{
	if(value < 0)
	{
		value = value * -1;
		return value;
	}
	else
	{
		return value;
	}
}

uint16_t sgn(uint16_t value)
{
    if(value < 0)
    {
        value = -1;
        return value;
    }
    else
    {
        value = 1;
        return value;
    }
}


/*  */