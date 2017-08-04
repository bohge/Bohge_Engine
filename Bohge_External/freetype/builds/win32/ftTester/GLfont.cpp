#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <gl\gl.h>
#include "gl\glu.h"
#include "glut.h"
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
//#pragma comment(lib , "lib/glut32.lib")
#pragma comment(lib , "lib/freetype2110.lib")

//opengl32.lib glu32.lib glaux.lib freetype235_D.lib odbc32.lib odbccp32.lib
#define MAX_NO_TEXTURES 1
#define CUBE_TEXTURE 0
GLuint texture_id[MAX_NO_TEXTURES];
struct xCharTexture
{
	GLuint  m_texID;
	wchar_t m_chaID;
	int     m_Width;
	int     m_Height;
	int     m_adv_x;
	int     m_adv_y;
	int     m_delta_x;
	int     m_delta_y;
public:
	xCharTexture()
	{
		m_texID  = 0;
		m_chaID  = 0;
		m_Width  = 0;
		m_Height = 0;
	}
}g_TexID[65536];
class xFreeTypeLib
{
	FT_Library m_FT2Lib;
	FT_Face    m_FT_Face;
public:
	int   m_w;
	int   m_h;
	void load(const char* font_file , int _w , int _h);
	GLuint loadChar(wchar_t ch);
};
void xFreeTypeLib::load(const char* font_file , int _w , int _h)
{
	FT_Library library;
	if (FT_Init_FreeType( &library) ) 
		exit(0);
	//加载一个字体,取默认的Face,一般为Regualer
	if (FT_New_Face( library, font_file, 0, &m_FT_Face )) 
		exit(0);
	FT_Select_Charmap(m_FT_Face, FT_ENCODING_UNICODE);
	m_w = _w ; m_h = _h;
	m_FT_Face->num_fixed_sizes;
	//大小要乘64.这是规定。照做就可以了。
	//FT_Set_Char_Size( m_FT_Face , 0 , m_w << 6, 96, 96);
	FT_Set_Pixel_Sizes(m_FT_Face,m_w, m_h);
}
GLuint xFreeTypeLib::loadChar(wchar_t ch)
{
	if(g_TexID[ch].m_texID)
		return g_TexID[ch].m_texID;
	if(FT_Load_Char(m_FT_Face, ch, /*FT_LOAD_RENDER|*/FT_LOAD_FORCE_AUTOHINT|
		(TRUE ? FT_LOAD_TARGET_NORMAL : FT_LOAD_MONOCHROME | FT_LOAD_TARGET_MONO) )   )
	{
		return 0;
	}
	/*if(FT_Load_Glyph( m_FT_Face, FT_Get_Char_Index( m_FT_Face, ch ), FT_LOAD_FORCE_AUTOHINT ))
	throw std::runtime_error("FT_Load_Glyph failed");*/
	xCharTexture& charTex = g_TexID[ch];

	//得到字模
	FT_Glyph glyph;
	if(FT_Get_Glyph( m_FT_Face->glyph, &glyph ))
		return 0;
	//转化成位图
	FT_Render_Glyph( m_FT_Face->glyph,   FT_RENDER_MODE_LCD );//FT_RENDER_MODE_NORMAL  ); 
	FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
	//取道位图数据
	FT_Bitmap& bitmap=bitmap_glyph->bitmap;
	//把位图数据拷贝自己定义的数据区里.这样旧可以画到需要的东西上面了。
	int width  =  bitmap.width;
	int height =  bitmap.rows;
	m_FT_Face->size->metrics.y_ppem;
	m_FT_Face->glyph->metrics.horiAdvance;

	charTex.m_Width = width;
	charTex.m_Height = height;
	charTex.m_adv_x = m_FT_Face->glyph->advance.x / 64.0f;
	charTex.m_adv_y = m_FT_Face->size->metrics.y_ppem; //m_FT_Face->glyph->metrics.horiBearingY / 64.0f;
	charTex.m_delta_x = (float)bitmap_glyph->left;
	charTex.m_delta_y = (float)bitmap_glyph->top - height;
	glGenTextures(1,&charTex.m_texID);
	glBindTexture(GL_TEXTURE_2D,charTex.m_texID);
	char* pBuf = new char[width * height * 4];
	for(int j=0; j  < height ; j++)
	{
		for(int i=0; i < width; i++)
		{
			unsigned char _vl =  (i>=bitmap.width || j>=bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width*j];
			pBuf[(4*i + (height - j - 1) * width * 4)  ] = 0xFF;
			pBuf[(4*i + (height - j - 1) * width * 4)+1] = 0xFF;
			pBuf[(4*i + (height - j - 1) * width * 4)+2] = 0xFF;
			pBuf[(4*i + (height - j - 1) * width * 4)+3] = _vl;
		}
	}
	glTexImage2D( GL_TEXTURE_2D,0,GL_RGBA,width, height,0,GL_RGBA,GL_UNSIGNED_BYTE,pBuf);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexEnvi(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	/*gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pBuf);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexEnvi(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_REPLACE);*/
	delete[] pBuf;
	return charTex.m_chaID;
}
xFreeTypeLib g_FreeTypeLib;
float ratio;
xCharTexture* getTextChar(wchar_t ch)
{
	g_FreeTypeLib.loadChar(ch);
	return &g_TexID[ch];
}
LPWSTR AnsiToUnicode(LPCSTR lpcstr)   //参数lpcstr类型也可是char*
{ 
	LPWSTR Pwstr;
	int  i;
	i=MultiByteToWideChar(CP_ACP,0,lpcstr,-1,NULL,0);
	Pwstr=new WCHAR[i]; 
	MultiByteToWideChar(CP_ACP,0,lpcstr,-1,Pwstr,i);
	return (Pwstr);
}
//wchar_t g_UnicodeString[]=L"aaabb\x4E2D\x6587\x0031\x0032\x0033";  
const char g_UnicodeString[]="aaa VB文件格式：\n\
							 若不明确就标为未知\n\
							 表演者： 若不明确就标为未知\n\
							 专辑：  若不明确就标为未知\n\
							 持续时间：01:01:00超过1小时；\n\
							 09:09不足小时；00:09不足1分钟\n\
							 glBindTexture(GL_TEXTURE_2D,pCharTex->m_texID);\n\
							 glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );\n\
							 glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );\n\
							 glEnable(GL_BLEND);\n\
							 glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA)" ;

void drawText(wchar_t* _strText,int x , int y, int maxW , int h)
{
	int sx = x;
	int sy = y;
	int maxH = h;
	size_t nLen = wcslen(_strText);

	for(int i = 0 ; i <nLen ; i ++)
	{
		if(_strText[i] =='\n')
		{
			sx = x ; sy += maxH + 12;
			continue;
		}
		xCharTexture* pCharTex = getTextChar(_strText[i]);
		glBindTexture(GL_TEXTURE_2D,pCharTex->m_texID);
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		//glDisable(GL_TEXTURE_2D);
		int w = pCharTex->m_Width;
		int h = pCharTex->m_Height;
		int ch_x = sx + pCharTex->m_delta_x;
		int ch_y = sy - h - pCharTex->m_delta_y;
		if(maxH < h) maxH = h;
		glBegin ( GL_QUADS );
		{
			glTexCoord2f(0.0f, 1.0f); glVertex3f(ch_x      , ch_y    ,  1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(ch_x +  w, ch_y    ,  1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(ch_x +  w, ch_y + h,  1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(ch_x     , ch_y + h,  1.0f);
		}
		glEnd();
		sx += pCharTex->m_adv_x;
		if(sx > x + maxW)
		{
			sx = x ; sy += maxH + 12;
		}
	}
}
void init(void)
{
	glShadeModel(GL_SMOOTH|GL_FLAT);       // Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);    // Black Background
	glEnable ( GL_COLOR_MATERIAL_FACE );
	glColorMaterial ( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	//g_FreeTypeLib.load("simhei.ttf",14,14);
	// g_FreeTypeLib.load("c:\\windows\\fonts\\simhei.ttf",14,14);
	g_FreeTypeLib.load("c:\\windows\\fonts\\simhei.ttf",12,12);

	glDisable ( GL_CULL_FACE );
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}
void reshape( int w, int h )
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;
	ratio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	glOrtho(0,w,h,0,-100,200);
	// Set the clipping volume
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 30 ,0 , 0 ,10 , 0.0f , 1.0f , 0.0f);
}
void display( void )
{
	glClearColor(0.0f , 0.0f , 0.6f , 1.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	/*glLoadIdentity ( );
	glPushMatrix();
	glTranslatef ( 0.0, 0.0, -5.0 );
	*/
	glEnable ( GL_TEXTURE_2D );
	wchar_t *wstr = AnsiToUnicode(g_UnicodeString);
	drawText(wstr, 50, 50, 900,25);
	//glPopMatrix();
	glutSwapBuffers();
}
void keyboard ( unsigned char key, int x, int y )  // Create Keyboard Function
{
	switch ( key ) {
case 27:        // When Escape Is Pressed...
	exit ( 0 );   // Exit The Program
	break;        // Ready For Next Case
default:        // Now Wrap It Up
	break;
	}
}
void arrow_keys ( int a_keys, int x, int y )  // Create Special Function (required for arrow keys)
{
	switch ( a_keys ) {
case GLUT_KEY_UP:     // When Up Arrow Is Pressed...
	glutFullScreen ( ); // Go Into Full Screen Mode
	break;
case GLUT_KEY_DOWN:               // When Down Arrow Is Pressed...
	glutReshapeWindow ( 900, 500 ); // Go Into A 500 By 500 Window
	break;
default:
	break;
	}
}
using namespace std;
int main ( int argc, char** argv )   // Create Main Function For Bringing It All Together
{
	//   ANSI   字符串，内容长度   7   字节   
	char           sz[20]   =   "中文123";  
	//   UNICODE   字符串，内容长度   5   个   wchar_t（10   字节）   
	wchar_t   wsz[20]   =   L"\x4E2D\x6587\x0031\x0032\x0033";     
	//   运行时设定当前   ANSI   编码，VC   格式   
	setlocale(LC_ALL,   ".936");  
	//   GCC   中格式   
	setlocale(LC_ALL,   "zh_CN.GBK");  
	//   Visual   C++   中使用小写   %s，按照   setlocale   指定编码输出到文件   
	//   GCC   中使用大写   %S   

	//   把   UNICODE   字符串按照   setlocale   指定的编码转换成字节   
	wcstombs(sz,   wsz,   20);   
	//   把字节串按照   setlocale   指定的编码转换成   UNICODE   字符串   
	mbstowcs(wsz,   sz,   20);    
	glutInit            ( &argc, argv ); // Erm Just Write It =)
	glutInitDisplayMode ( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA ); // Display Mode
	glutInitWindowPosition (0,0);
	glutInitWindowSize  ( 900, 500 ); // If glutFullScreen wasn't called this is the window size
	glutCreateWindow    ( "NeHe Lesson 6- Ported by Rustad" ); // Window Title (argv[0] for current directory as title)
	init ();
	//glutFullScreen      ( );          // Put Into Full Screen
	glutDisplayFunc     ( display );  // Matching Earlier Functions To Their Counterparts
	glutReshapeFunc     ( reshape );
	glutKeyboardFunc    ( keyboard );
	glutSpecialFunc     ( arrow_keys );
	glutIdleFunc    ( display );
	glutMainLoop        ( );          // Initialize The Main Loop
	return 1;
}