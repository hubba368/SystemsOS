#include <string.h>
#include <console.h>
#include <hal.h>
#include <exception.h>
#include "physicalmemorymanager.h"
#include "virtualmemorymanager.h"
#include "bootinfo.h"
#include <keyboard.h>
#include <sysapi.h>
#include <user.h>
#include <vgamodes.h>
#include <vga.h>

// This is a dummy __main.  For some reason, gcc puts in a call to 
// __main from main, so we just include a dummy.
 
BootInfo *	_bootInfo;
 
void __main() {}

void InitialisePhysicalMemory()
{
	// Initialise the physical memory manager
	// We place the memory bit map in the next available block of memory after the kernel.
	// Note that all memory addresses here are physical memory addresses since we are dealing
	// with management of physical memory

	uint32_t memoryMapAddress = 0x100000 + _bootInfo->KernelSize;
	if (memoryMapAddress % PMM_GetBlockSize() != 0)
	{
		// Make sure that the memory map is going to be aligned on a block boundary
		memoryMapAddress = (memoryMapAddress / PMM_GetBlockSize() + 1) * PMM_GetBlockSize();
	}
	uint32_t sizeOfMemoryMap = PMM_Initialise(_bootInfo, memoryMapAddress);

	// We now need to mark various regions as unavailable
	
	// Mark first page as unavailable (so that a null pointer is invalid)
	PMM_MarkRegionAsUnavailable(0x00, PMM_GetBlockSize());

	// Mark memory used by kernel as unavailable
	PMM_MarkRegionAsUnavailable(0x100000, _bootInfo->KernelSize);

	// Mark memory used by memory map as unavailable
	PMM_MarkRegionAsUnavailable(PMM_GetMemoryMap(), sizeOfMemoryMap);

	// Reserve two blocks for the stack and make unavailable (the stack is set at 0x90000 in boot loader)
	uint32_t stackSize = PMM_GetBlockSize() * 2;
	PMM_MarkRegionAsUnavailable(_bootInfo->StackTop - stackSize, stackSize);
	
	// Reserve two block for the kernel stack and make unavailable
	PMM_MarkRegionAsUnavailable(0x80000 - stackSize, stackSize);
}
void Initialise()
{
	ConsoleClearScreen(0x1F);
	ConsoleWriteString("UODOS 32-bit Kernel. Kernel size is ");
	ConsoleWriteInt(_bootInfo->KernelSize, 10);
	ConsoleWriteString(" bytes\n");
	ConsoleWriteString("");
	ConsoleWriteString("\t\t\t\t\tPress Space To View Ring 0 Tests.\n");
	ConsoleWriteString("\t\t\t\t\t\tPress Backspace to View Ring 3 Tests.\n");
	ConsoleWriteString("\t\t\t\t\t\tPress Any Key after viewing to reset.\n");
	HAL_Initialise();
	InitialisePhysicalMemory();
	VMM_Initialise();
	KeyboardInstall(33); 
	InitialiseSysCalls();
}

void RingZeroTests()
{
	// set dimensions
	VGA_SetGraphicsMode(320, 200, 1);
	
	//clear screen
	ClearScreen();

	// Set pixel tests
	Vector2 pT1 = {1,1, 0x0B};
	Vector2 *pixelTest1 = &pT1;
	SetPixel(pixelTest1);

	Vector2 pT2 = {50, 50, 0x0F};
	Vector2 *pixelTest2 = &pT2;
	SetPixel(pixelTest2);

	// Draw line test
	Vector2 lTp1 = {10, 10, 0x0F};
	Vector2 *lineTestp1 = &lTp1;
	Vector2 lTp2 = {70, 70, 0x0F};
	Vector2 *lineTestp2 = &lTp2;
	DrawLine(lineTestp1, lineTestp2, 2);

	// draw unfilled rectangle test
	Vector2 rT1 = {20, 20, 0x0B};
	Vector2 *rectangleTest1 = &rT1;
	DrawRectangle(rectangleTest1, 10, 20);

	// draw filled rectangle test
	Vector2 rT2 = {10, 60, 0x0F};
	Vector2 *rectangleTest2 = &rT2;
	DrawFilledRectangle(rectangleTest2, 30, 20);

	// draw unfilled 'circle' test
	Vector2 cT1 = {100, 40, 0x0D};
	Vector2 *circleTest1 = &cT1;
	DrawCircle(circleTest1, 5, false);

	// draw filled 'circle'
	Vector2 cT2 = {135, 40, 0x0E};
	Vector2 *circleOrigin2 = &cT2;
	DrawCircle(circleOrigin2, 10, true);
}


void RingThreeTests()
{
	// enter ring 3
	HAL_EnterUserMode();
	
	// width , height
	// 0,0 is in top left corner
	VGA_SetGraphicsMode(320, 200, 1);

	// clear screen to black
	User_ClearScreen();

	// user mode pixel test
	Vector2 t = {1, 1, 0x0F};
	Vector2 *test = &t;
	User_SetPixel(test);

	// user mode line draw of  specified thickness test
	Vector2 p1 = {10,10, 0x0B};
	Vector2 *point1 = &p1;
	Vector2 p2 = {70, 70, 0x0B};
	Vector2 *point2 = &p2;
	User_DrawLine(point1, point2, 3);

	// user mode unfilled rectangle test
	Vector2 r = {20, 20, 0x0C};
	Vector2 *originPoint = &r;
	User_DrawRectangle(originPoint, 20, 30);

	// user mode filled rectangle test
	Vector2 r2 = {70, 60, 0x04};
	Vector2 *originPoint2 = &r2;
	User_DrawFilledRectangle(originPoint2, 40, 20);

	// user mode unfilled circle test
	Vector2 cR = {100, 40, 0x0D};
	Vector2 *circleOrigin = &cR;
	User_DrawCircle(circleOrigin, 15, false);

	// user mode filled circle test
	Vector2 cR2 = {135, 40, 0x0E};
	Vector2 *circleOrigin2 = &cR2;
	User_DrawCircle(circleOrigin2, 15, true);
}

void main(BootInfo * bootInfo) 
{
	_bootInfo = bootInfo;
	Initialise();

	bool running = true;

	while(running)
	{
		// enable the keyboard
		// check key press
		KeyboardEnable();
		if(KeyboardGetCharacter() == KEY_SPACE)
		{
			RingZeroTests();
		}

		if(KeyboardGetCharacter() == KEY_BACKSPACE)
		{
			RingThreeTests();
		}

		if(KeyboardGetCharacter() == KEY_ESCAPE)
		{
			running = false;
		}
		// unsure why calling this causes the program to reset
		KeyboardReset();
	}
}


//WHAT NEEDS TO BE DONE//
/*


*/