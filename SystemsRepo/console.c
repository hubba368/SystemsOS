//	Basic Console Output.

#include <stdint.h>
#include <string.h>
#include <console.h>

// Video memory
uint16_t *_videoMemory = (uint16_t *)0xB8000;						// Defines _videoMemory at address 0xB8000

#define CONSOLE_HEIGHT		25
#define CONSOLE_WIDTH		80

// Current cursor position
uint8_t _cursorX = 0;
uint8_t _cursorY = 0;

// Current color
uint8_t	_colour = 15;

char stringBuffer[32];
char hexChars[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

// Write byte to device through port mapped io
void  OutputByteToVideoController(unsigned short portid, unsigned char value) 
{
	asm volatile("movb	%1, %%al \n\t"
				 "movw	%0, %%dx \n\t"
				  "out	%%al, %%dx"
				 : : "m" (portid), "m" (value));
				 
}

// Update hardware cursor position
void UpdateCursorPosition(int x, int y) 
{
    uint16_t cursorLocation = y * 80 + x;

	// Send location to VGA controller to set cursor
	// Send the high byte
	OutputByteToVideoController(0x3D4, 14);
	OutputByteToVideoController(0x3D5, cursorLocation >> 8); 
	// Send the low byte
	OutputByteToVideoController(0x3D4, 15);
	OutputByteToVideoController(0x3D5, cursorLocation);      
}

// Displays a character
void ConsoleWriteCharacter(unsigned char c) 
{

	uint16_t attribute = _colour << 8;					// 111100000000 | 3840 | f00

	// special character handling
	if (c == 0x09)
	{
		//Tab
		_cursorX = (_cursorX + 8) & ~(8 - 1);
	}
	else if(c == '\r')
	{
		//carriage return
		_cursorX = 0;
	}
	else if(c >= 32)
	{
		//print all characters above space
		uint16_t* location = _videoMemory + (_cursorY * CONSOLE_WIDTH + _cursorX);		
	
		*location = c | attribute;													// | bitwise inclusive OR
		_cursorX++;
	}

	//print new line if at edge of the row
	if(_cursorX >= CONSOLE_WIDTH)
	{
		_cursorX = 0;
		_cursorY++;
	}	
	//scroll the entire screen if at end of last row
	if(_cursorY >= CONSOLE_HEIGHT)
	{
		ScrollScreen();
		_cursorY = CONSOLE_HEIGHT - 1;
	}

	UpdateCursorPosition(_cursorX, _cursorY);
}

// Display specified string
void ConsoleWriteString(char* str) 
{
	if (!str)
	{
		return;
	}
	while (*str) 
	{
		ConsoleWriteCharacter(*str++);
		
	}
}

void ConsoleWriteInt(unsigned int i, unsigned int base)
{
	int pos = 0;

	//check if we are not in binary or above hexadecimal
	if(i == 0 || base > 16)
	{
		ConsoleWriteCharacter('0');
	}
	else
	{
		while(i != 0)
		{
			stringBuffer[pos] = hexChars[i % base];
			pos++;
			i /= base;
		}
		while(--pos >= 0)
		{
			ConsoleWriteCharacter(stringBuffer[pos]);
		}
	}
}

unsigned int ConsoleSetColour(const uint8_t c)
{
	return _colour = c;
}

//Update the cursor position
void ConsoleGotoXY(unsigned int x, unsigned int y)
{
	//only do it as long as the cursor is in the bounds of the screen
	if(_cursorX <= CONSOLE_WIDTH - 1)
	{
		_cursorX = x;
	}
	if(_cursorY <= CONSOLE_HEIGHT - 1)
	{
		_cursorY = y;
	}

	UpdateCursorPosition(_cursorX, _cursorY);
}

void ConsoleGetXY(unsigned* x, unsigned* y)
{
	*x = _cursorX;
	*y = _cursorY;
}

int ConsoleGetWidth()
{
	return CONSOLE_WIDTH;
}

int ConsoleGetHeight()
{
	return CONSOLE_HEIGHT;
}

// Clear the display screen 
void ConsoleClearScreen(const uint8_t c)
{
	_colour = c;
	uint16_t blank = 32 | (c << 8);

	// go through the entire screen
	for(int i =0; i < 80 * 25; i++)
	{
		_videoMemory[i] = blank;
	}

	//reset the cursor to the top left
	ConsoleGotoXY(0,0);
}

//scroll the screen one row upwards if at the bottom
void ScrollScreen()
{
	if(_cursorY >= CONSOLE_HEIGHT)
	{
		uint16_t attribute = _colour << 8;

		//move the current display upwards one line
		int line25 = (CONSOLE_HEIGHT - 1) * CONSOLE_WIDTH;

		for(int i = 0; i < line25; i++)
		{
			_videoMemory[i] = _videoMemory[i + CONSOLE_WIDTH];
		}

		//clear the bottom line
		for(int j = 0; j < line25 + 80; j++)
		{
			_videoMemory[j] = attribute | 32;
		}

		_cursorY = 25;
	}
}