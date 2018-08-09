#ifndef _VGA_H
#define _VGA_H
#include <stdint.h>
#include "Vector2.h"



void SetPixel(Vector2* pos);

void ClearScreen();

void DrawLine(Vector2* firstPos, Vector2* secondPos, uint16_t thickness);

void DrawRectangle(Vector2* originPoint, uint16_t width, uint16_t height);

void DrawFilledRectangle(Vector2 *originPoint, uint16_t width, uint16_t height);

void DrawCircle(Vector2* origin, uint16_t radius, bool filled);

uint16_t abs(uint16_t value);

uint16_t sgn(uint16_t value);

#endif