// Implementation of string and memory functions

#include <string.h>

int strcmp(const char* str1, const char* str2) 
{
	
	int cmp;
	
	while (*str1 == *str2) 
	{
	str1++;
	str2++;
	}
	
	if (*str1 > *str2) 
	{
		cmp = *str1;
	} 
	else if (*str2 > *str1) 
	{
		cmp = *str2;
	}
	
	return cmp;
}

// Copies the string pointed to, by src to dest.
char * strcpy(char * destination, const char * source) 
{
	char *temp = destination;
	
	while ((*temp++ = *source++) != '\0') 
	{
			
	}
	return temp;
}

errno_t strcpy_s(char *destination, size_t numberOfElements, const char *source) 
{
	if(destination == NULL || source == NULL)
	{
		return EINVAL;
	}


	if (numberOfElements < strlen(source)) 
	{
		*destination = '\0';
		return ERANGE;
	}
	
	char *temp = destination;
	
	while ((*temp++ = *source++) != '\0') 
	{
			
	}
	return *temp;
}

size_t strlen(const char* source) 
{
		
	int i = 0;
		
	while (*source != NULL) 
	{ 
		i++;
		source++;
	}
	
	return i;	
}

void * memcpy(void * destination, const void * source, size_t count) 
{
	
	// Create temp variables
	char *temp = destination;
	const char *s = source;

	// Move the value pointed to by S to the location pointed to by TEMP.
	while (count--) 
	{
		*temp++ = *s++;
	}	
	return temp;
}

errno_t memcpy_s(void *destination, size_t destinationSize, const void *source, size_t count) 
{
	if(destination == NULL)
	{
		return EINVAL;
	}

	if(source == NULL)
	{
		memset((void*)destination, '\0', destinationSize);
		return EINVAL;
	}


	if (destinationSize < count) 
	{
		destination = 0;
		return ERANGE;
	}
	
	char *temp = destination;
	const char *s = source;

	while (count--) 
	{
		*temp++ = *s++;
	}	
	return *temp;

}

void * memset(void *destination, char val, size_t count) 
{
	
	char *dest = destination;
	
	while (count--) 
	{
		*dest++ = val;
	}
	return dest;
}

unsigned short * memsetw(unsigned short * destination, unsigned short val, size_t count) 
{
	
	unsigned short *dest = destination; 		// Create temp destination pointer
	
	while (count--) 
	{		  		// Replace the CHAR at the location pointed to and then increament to point to the next value
		*dest++ = val;		  		 
	}
	return dest;
}








