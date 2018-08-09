#include <console.h>
#include <vga.h>
#include <Vector2.h>
#include <keyboard.h>

void User_ConsoleWriteCharacter(unsigned char c)
{
	asm volatile("movl $1, %%eax\n\t"
				 "movzx %0, %%ebx\n\t"
				 "int $0x80\n"
				 : : "b"(c)
				);
}

void User_ConsoleWriteString(char* str)
{
	asm volatile("xor %%eax, %%eax\n\t"
				 "leal (%0), %%ebx\n\t"
				 "int $0x80\n"
				 : : "b"(str)
				);
}

void User_ConsoleWriteInt(unsigned int i, unsigned int base)
{
	asm volatile("movl $2, %%eax\n\t"
				 "movl %0, %%ecx\n\t"
				 "movl %1, %%ebx\n\t"
				 "int $0x80\n"
				 : : "c"(base), "b"(i)
				);
}

void User_ConsoleClearScreen(const uint8_t c)
{
	asm volatile("movl $3, %%eax\n\t"
				 "movzx %0, %%ebx\n\t"
				 "int $0x80\n"
				 : : "b"(c)
				);
}

void User_ConsoleGotoXY(unsigned int x, unsigned int y) 
{
	asm volatile("movl $4, %%eax\n\t"
				 "movl %0, %%ecx\n\t"
				 "movl %1, %%ebx\n\t"
				 "int $0x80\n"
				 : : "c"(y), "b"(x)
				);
}

// User defined user mode functions below
void User_ClearScreen() 
{
	asm volatile("movl $5, %%eax\n\t"
				 "int $0x81\n"
				 : : );
				 
}

void User_SetPixel(Vector2* pos)
{
	// pass in the function number into eax
	// then pass the function parameters
	// into edx, ecx or ebx
	asm volatile("movl $6, %%eax\n\t"
				 "leal (%0), %%ebx\n\t"
				 "int $0x81\n"
				 : : "b"(pos)
				);		 
}

void User_DrawLine(Vector2* firstPos, Vector2* secondPos, uint16_t thickness)
{
	asm volatile("movl $7, %%eax\n\t"
				 "leal (%0), %%ebx\n\t"
				 "leal (%1), %%ecx\n\t"
				 "movzx %2, %%edx\n\t"
				 "int $0x81\n"
				 : : "b"(firstPos), "c"(secondPos), "d"(thickness)
				);
}

void User_DrawRectangle(Vector2 *origin, uint16_t width, uint16_t height)
{
	asm volatile("movl $8, %%eax\n\t"
				 "leal (%0), %%ebx\n\t"
				 "movzx %1, %%ecx\n\t"
				 "movzx %2, %%edx\n\t"
				 "int $0x81\n"
				 : : "b"(origin), "c"(width), "d"(height)
				);
}

void User_DrawCircle(Vector2 *origin, uint16_t radius, bool filled)
{
	asm volatile("movl $9, %%eax\n\t"
				 "leal (%0), %%ebx\n\t"
				 "movzx %1, %%ecx\n\t"
				 "movzx %2, %%edx\n\t"
				 "int $0x81\n"
				 : : "b"(origin), "c"(radius), "d"(filled)
				);
}

void User_DrawFilledRectangle(Vector2 *origin, uint16_t width, uint16_t height)
{
	asm volatile("movl $10, %%eax\n\t"
				 "leal (%0), %%ebx\n\t"
				 "movzx %1, %%ecx\n\t"
				 "movzx %2, %%edx\n\t"
				 "int $0x81\n"
				 : : "b"(origin), "c"(width), "d"(height)
   );
}
