//////////////////////////////////////////////////////////////////////////////////////
//
//						The Bohge Engine License (BEL)
//
//	Copyright (c) 2011-2014 Peng Zhao
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in 
//	all copies or substantial portions of the Software. And the logo of 
//	Bohge Engine shall be displayed full screen for more than 3 seconds 
//	when the software is started. Copyright holders are allowed to develop 
//	game edit based on Bohge Engine, The edit must be released under the MIT 
//	open source license if it is going to be published. In no event shall 
//	copyright holders be prohibited from using any code of Bohge Engine 
//	to develop any other analogous game engines.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//
//////////////////////////////////////////////////////////////////////////////////////



#include "Log.h"
#ifdef WIN32
#include <Windows.h>
#endif

#ifdef PRINTLOG

#ifdef WIN32

Tracker::Console Tracker::info;



#define ONLINE_MAX_PACKET_SIZE 500

Tracker::Console::Console()
{
	AllocConsole();  
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle,_O_TEXT);
	FILE * hf = _fdopen( hCrt,"w" );  
	outPutFile = hf;
}
Tracker::Console::~Console()
{
	FreeConsole();
}
void XP_API_MEMSET( void * data, int value, int size )
{
	memset( data, value, size );
}
int XP_API_STRLEN( const char * string )
{
	if (string == NULL)
	{
		return 0 ;
	}
	return (int)strlen( string );
}
char * XP_API_ITOA( int num, char * str, int radix )
{
	const char table[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char *ptr = str;
	bool negative = false;

	if( num == 0 )
	{
		//num=0 
		*ptr++ = '0';
		*ptr = '\0';		// don`t forget the end of the string is '\0'!!!!!!!!!
		return str;
	}

	if( num < 0 )
	{
		//if num is negative ,the add '-'and change num to positive 
		*ptr++ = '-';
		num *= -1;
		negative = true;
	}

	while(num)
	{
		*ptr++ = table[ num % radix ];
		num /= radix;
	}
	*ptr = '\0'; //if num is negative ,the add '-'and change num to positive
	// in the below, we have to converse the string
	char *start =( negative ? str+1: str ); //now start points the head of the string
	ptr--; //now prt points the end of the string
	while( start<ptr )
	{
		char temp = *start; 
		*start = *ptr;
		*ptr = temp;
		start++;
		ptr--;
	} 

	return str;
}
void * XP_API_MEMCPY( void * data, const void * src, int size )
{
	memcpy( data, src, size );
	return data;
}
void Tracker::Console::Printf( const char * sender, va_list args)
{
	fprintf(outPutFile,"%s ",sender);
	char * format = va_arg( args, char * );
	//va_list args;
	//int num = 0;
	int len = 0;
	int idx = 0;
	char data[20*ONLINE_MAX_PACKET_SIZE];
	//time_t ltime;
	//struct tm*  tmTime;

	XP_API_MEMSET(data,0,20*ONLINE_MAX_PACKET_SIZE);

	//va_start( args, format );
	len = XP_API_STRLEN( (char *)format );

	for ( int i=0; i<len; i++ )
	{
		if ( '%' == format[i] )
		{
			bool isContinue = true;
			while( isContinue )
			{
				isContinue = false;
				switch( format[i+1] )
				{
				case 'd':
					{
						int value = va_arg( args, int );
						char temp[ 12 ] = { 0 };
						XP_API_ITOA( value, temp, 10 );
						int length = XP_API_STRLEN( temp );
						XP_API_MEMCPY( data + idx, temp, length );
						idx += length;
						i++;
					}break;
				case 'f':
					{
						float value = va_arg( args, double );
						char temp[ 32 ] = { 0 };
						int length = sprintf(temp,"%.3f",value);
						XP_API_MEMCPY( data + idx, temp, length );
						idx += length;
						i++;
					}break;
				case 's':
					{
						char * value = va_arg( args, char * );
						if ( NULL != value )
						{
							int length = XP_API_STRLEN( value );
							XP_API_MEMCPY( data + idx, value, length );
							idx += length;
						}
						i++;
					}break;
				default:
					{
						isContinue = true;
						i++;
					}break;
				}	
			}
		}
		else
		{
			data[ idx++ ] = format[i];
		}
	}	

	//va_end( args );
	if(outPutFile)
	{
		fprintf(outPutFile,"%s",data);
		fflush(outPutFile);
		return;
	}
	DEBUGLOG("%s", data);
}
#endif
#endif