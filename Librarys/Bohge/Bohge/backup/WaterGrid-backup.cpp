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



//////////////////////////////
//使用projectedGrid的水面	//
/////////////////////////////
#include "WaterGrid.h"
#include "RendBuffer.h"
#include "Camera.h"
#include "Frustum.hpp"
#include "Ray.h"
#include "ShaderManage.h"
#include "Material.h"

#define SSE false	// SSE is broken atm
#define quadpipe false
#define packednoise true

#define SURFACE_LONG 8
#define SURFACE_WIDE 4
#define SURFACE_SIZE (SURFACE_LONG*SURFACE_WIDE)

#define log_xor || log_xor_helper() ||

struct log_xor_helper {
	bool value;
};

template<typename LEFT>
log_xor_helper &operator ||(const LEFT &left, log_xor_helper &xor) {
	xor.value = (bool)left;
	return xor;
}

template<typename RIGHT>
bool operator ||(const log_xor_helper &xor, const RIGHT &right) {
	return xor.value ^ (bool)right;
}

using namespace std;

//------------------------------------------------------------------------------------------------------
WaterGrid::WaterGrid(void)
:m_isRender(false),
m_WaterHight(0.0f),
m_pBuffer(NULL),
time(0.0f)
{
	this->SetAttrib( IRenderNode::RA_NOCULL );
	m_WaterPlane.FromPointNormal( vector3f(0,0,0), vector3f(0,1,0) );//构造一个水体平面，做投影
	m_Vertex.resize( SURFACE_SIZE );
	m_pBuffer = NEW RendBuffer(
		VertexTextureNormal::Layout(),
		IDriver::TRIANGLE_STRIP );
		//IDriver::LINE_STRIP );
	{//纹理坐标
		for ( int z = 0 ; z < SURFACE_LONG; z ++ )
		{
			for ( int x = 0 ; x < SURFACE_WIDE; x ++ )
			{
				m_Vertex[ x + z*SURFACE_WIDE].Normal() = vector3f(0,1,0);
				m_Vertex[ x + z*SURFACE_WIDE].SetCoordinate( vector2f( (float)x/(SURFACE_WIDE-1), (float)z/(SURFACE_LONG-1)) );
			}
		}
	}
	m_pBuffer->MakeVertexBuffer( &m_Vertex[0], m_Vertex.size(), sizeof(VertexTextureNormal), IDriver::DYNAMIC );
	{//初始化索引
		vector<ushort> Indices;
		Indices.resize( SURFACE_WIDE*(SURFACE_LONG-1)*2 );
		int i = 0;
		for ( int z = 0 ; z < SURFACE_LONG-1; z++ )
		{
			for ( int x = 0 ; x < SURFACE_WIDE; x++ )
			{
				Indices[i] = x + z * SURFACE_WIDE; 
				Indices[i+1] = x + (z + 1) * SURFACE_WIDE;
				i+=2;
			}
			z++;
			if( z < SURFACE_LONG-1 )
			{
				for( int x = SURFACE_WIDE-1; x >= 0 ; x-- )
				{
					Indices[i] = x + (z + 1) * SURFACE_WIDE;
					Indices[i+1] = x + z * SURFACE_WIDE; 
					i+=2;
				}
			}
		}
		m_pBuffer->MakeIndiesBuffer( &Indices[0], Indices.size(), IDriver::STATIC );
	}
	{//材质
		m_pMaterial = NEW Material;
		m_pMaterial->Ambient() = vector3f( 0.2, 0.2, 0.2 );
		m_pMaterial->Diffuse() = vector3f( 0.9, 0.9, 0.9 );
		m_pMaterial->Specular() = vector3f( 0.2, 0.2, 0.2 );
		m_pMaterial->SpeLevel() = 4;
		m_pMaterial->Shininess() = 84;
		m_pMaterial->SetNode( this );
		m_pMaterial->SetShader( &IDriver::Instance().GetShaderManage().GetShader<WaterShader>(ShaderManage::WaterShader) );
	}
	{//参数
		prm = NEW parameterhandler();
		prm->set_float( p_fStrength,	1.9f );
		prm->set_bool(	p_bDisplace,	true );
		prm->set_int(	p_iOctaves,		8 );
		prm->set_float( p_fScale,		0.38f );
		prm->set_float( p_fFalloff,		0.607f );
		prm->set_float( p_fAnimspeed,	1.4f );
		prm->set_float( p_fTimemulti,	1.27f );
		prm->set_bool(	p_bPaused,		false );

		prm->set_float(	p_fLODbias,		0.0f);
		prm->set_bool(	p_bDisplayTargets, false );
		prm->set_float(	p_fElevation,	7.0f );

		prm->set_float( p_fSunPosAlpha,		1.38f );
		prm->set_float( p_fSunPosTheta,		1.09f );
		prm->set_float( p_fSunShininess,	84.0f );
		prm->set_float( p_fSunStrength,		12.0f );

#ifdef CPU_NORMALS
		prm->set_bool(	p_bSmooth,			false );
#else
		prm->set_bool(	p_bSmooth,			true );
#endif
		prm->set_float(	p_bReflRefrStrength,0.1f );

		prm->set_float( p_fWaterColourR,	0.17f );
		prm->set_float( p_fWaterColourG,	0.27f );
		prm->set_float( p_fWaterColourB,	0.26f );

		prm->set_bool(	p_bAsPoints,		false );
		prm->set_bool(	p_bDrawDuckie,		true );
		prm->set_bool(	p_bDrawIsland,		false );
		prm->set_bool(	p_bDiffuseRefl,		false );

		prm->set_active_parameter(p_fStrength);
	}
	this->_InitNoise();
}
//------------------------------------------------------------------------------------------------------
WaterGrid::~WaterGrid(void)
{
	SAFE_DELETE( prm );
	SAFE_DELETE( m_pBuffer );
}
//------------------------------------------------------------------------------------------------------
void WaterGrid::Update( uint milisecon )
{
	this->_CaculateNoise();
	float magnitude = n_dec_magn * prm->get_float(p_fScale);

	IDriver& driver = IDriver::Instance();
	ICamera& camera = driver.GetCamera();

	float ViewFar = camera.GetFar();
	Rayf raylt, raylb, rayrt, rayrb;//计算四条射线尝试与水平面相交
	vector3f plt, plb, prt, prb;//四个交点
	vector3f splt, splb;//屏幕空间的坐上角开始，和结束
	Matrix44f Test = camera.GetViewProj() * camera.GetUnViewProj();
	Matrix44f vp = camera.GetViewProj();
	vp.InverseSelf();
	camera.PickRay( vector2f(-1,1), raylt );
	camera.PickRay( vector2f(-1,-1), raylb );
	camera.PickRay( vector2f(1,1), rayrt );
	camera.PickRay( vector2f(1,-1), rayrb );
	raylt.m_RayOrig = camera.GetPosition();
	raylb.m_RayOrig = camera.GetPosition();
	rayrt.m_RayOrig = camera.GetPosition();
	rayrb.m_RayOrig = camera.GetPosition();
	{//计算摄像机的可见性
		plb = raylb.m_RayOrig + raylb.m_RayDir * ViewFar;
		prb = rayrb.m_RayOrig + rayrb.m_RayDir * ViewFar;
		if( plb.m_y > m_WaterHight && prb.m_y > m_WaterHight )//如果远平面与bottom的焦点已经高于水高度了，说明水是看不到的
		{
			m_isRender = false;
			return;
		}
	}
	//计算全部的交点
	plb = m_WaterPlane.IntersectRay( raylb ); //下方交点
	prb = m_WaterPlane.IntersectRay( rayrb );
	//计算上交点
	plt = raylt.m_RayOrig + raylt.m_RayDir * ViewFar;
	if( plt.m_y > m_WaterHight )//如果与远平面的交点已经超过了水体高度，说明raylt无法与水面相交了，转换计算最远点为远平面与水面的相交
	{
		Rayf rayl, rayr;//远平面左右两条射线
		rayl.m_RayOrig = plt;
		rayr.m_RayOrig = rayrt.m_RayOrig + rayrt.m_RayDir * ViewFar;

		rayl.m_RayDir = raylb.m_RayOrig + raylb.m_RayDir * ViewFar;
		rayr.m_RayDir = rayrb.m_RayOrig + rayrb.m_RayDir * ViewFar;

		rayl.m_RayDir -= rayl.m_RayOrig;
		rayl.m_RayDir.NormalizeSelf();

		rayr.m_RayDir -= rayr.m_RayOrig;
		rayr.m_RayDir.NormalizeSelf();

		plt = m_WaterPlane.IntersectRay( rayl );
		prt = m_WaterPlane.IntersectRay( rayr );
	}
	else
	{
		plt = m_WaterPlane.IntersectRay( raylt ); //上方交点
		prt = m_WaterPlane.IntersectRay( rayrt ); //上方交点
	}
	//计算屏幕空间的投影点
	splt = vector4f(plt) * camera.GetViewProj();
	splb = vector4f(plb) * camera.GetViewProj();
	vector3f SceneDir = ( splb - splt ) / (SURFACE_LONG-1);
	//有四个交点后，计算网格
	vector3f rDir = prb - prt;//计算右面的递增方向
	rDir.NormalizeSelf();
	vector3f lBegin = plt;//左端的开始
	vector3f rBegin = prt;//右端的开始
	for ( int z = 0 ; z < SURFACE_LONG; z ++ )
	{
		vector3f wPoint = lBegin;//点的开始
		vector3f hWorldStep = (rBegin - lBegin) / (SURFACE_WIDE-1);//计算水平的增长
		for ( int x = 0 ; x < SURFACE_WIDE; x ++ )
		{
			m_Vertex[ x + z*SURFACE_WIDE].Position() = vector4f( wPoint.m_x,
				m_WaterHight ,//+ _get_height_dual(magnitude*wPoint.m_x, magnitude*wPoint.m_z ),
				wPoint.m_z );
			wPoint += hWorldStep;
		}
		splt += SceneDir;
		vector3f worldPostion = vector3f( vector4f(splt) * camera.GetUnViewProj() );
		lBegin = worldPostion - lBegin;
		float worldLebgth = lBegin.Length();//计算该递增产生的世界空间的长度

		lBegin = worldPostion;//计算新的左端
		rBegin += rDir * worldLebgth;//计算新的右端
	}
	this->_calc_normals();
	m_pBuffer->FlushVertexBuffer( &m_Vertex[0], 0, m_Vertex.size() );
	m_isRender = true;
}
//------------------------------------------------------------------------------------------------------
void WaterGrid::Render( IDriver& driver ) const
{
	if( m_isRender )
	{
		driver.Draw( *m_pBuffer, *m_pMaterial );
	}
}
//------------------------------------------------------------------------------------------------------
void WaterGrid::WaterSurface( float hight )
{
	m_WaterHight = hight;
	m_WaterPlane.FromPointNormal( vector3f(0,m_WaterHight,0), vector3f(0,1,0) );//构造一个水体平面，做投影
}
//------------------------------------------------------------------------------------------------------
void WaterGrid::_InitNoise()
{
	// create noise (uniform)
	float tempnoise[n_size_sq*noise_frames];
	for(int i=0; i<(n_size_sq*noise_frames); i++)
	{
		//this->noise[i] = rand()&0x0000FFFF;		
		float temp = (float) rand()/RAND_MAX;		
		tempnoise[i] = 4*(temp - 0.5f);	
	}	

	for(int frame=0; frame<noise_frames; frame++)
	{
		for(int v=0; v<n_size; v++)
		{
			for(int u=0; u<n_size; u++)
			{	
				/*float temp = 0.25f * (tempnoise[frame*n_size_sq + v*n_size + u] +
				tempnoise[frame*n_size_sq + v*n_size + ((u+1)&n_size_m1)] + 
				tempnoise[frame*n_size_sq + ((v+1)&n_size_m1)*n_size + u] +
				tempnoise[frame*n_size_sq + ((v+1)&n_size_m1)*n_size + ((u+1)&n_size_m1)]);*/
				int v0 = ((v-1)&n_size_m1)*n_size,
					v1 = v*n_size,
					v2 = ((v+1)&n_size_m1)*n_size,
					u0 = ((u-1)&n_size_m1),
					u1 = u,
					u2 = ((u+1)&n_size_m1),					
					f  = frame*n_size_sq;
				float temp = (1.0f/14.0f) * (	tempnoise[f + v0 + u0] + tempnoise[f + v0 + u1] + tempnoise[f + v0 + u2] +
					tempnoise[f + v1 + u0] + 6.0f*tempnoise[f + v1 + u1] + tempnoise[f + v1 + u2] +
					tempnoise[f + v2 + u0] + tempnoise[f + v2 + u1] + tempnoise[f + v2 + u2]);

				this->noise[frame*n_size_sq + v*n_size + u] = noise_magnitude*temp;
			}
		}
	}	

}
//------------------------------------------------------------------------------------------------------
void WaterGrid::_CaculateNoise()
{
	octaves = Math::GetMin(prm->params[p_iOctaves].iData, max_octaves);		

	// calculate the strength of each octave
	float sum=0.0f;
	for(int i=0; i<octaves; i++)
	{
		f_multitable[i] = powf(prm->get_float(p_fFalloff),1.0f*i);
		sum += f_multitable[i];
	}

	{
		for(int i=0; i<octaves; i++)
		{
			f_multitable[i] /= sum;
		}}

	{
		for(int i=0; i<octaves; i++)
		{
			multitable[i] = scale_magnitude*f_multitable[i];
		}}


	double itime = IDriver::Instance().GetTimeSpan();
	static double lp_itime=0.0;	
	itime *= 0.001 * prm->get_float( p_fAnimspeed );
	lp_itime = 0.99*lp_itime + 0.01 * itime;
	if ( !prm->get_bool(p_bPaused) )
		time += lp_itime;	

	double	r_timemulti = 1.0;

	for(int o=0; o<octaves; o++)
	{		
		unsigned int image[3];
		int amount[3];
		double dImage, fraction = modf(time*r_timemulti,&dImage);
		int iImage = (int)dImage;
		amount[0] = scale_magnitude*f_multitable[o]*(pow(sin((fraction+2)*PI/3),2)/1.5);
		amount[1] = scale_magnitude*f_multitable[o]*(pow(sin((fraction+1)*PI/3),2)/1.5);
		amount[2] = scale_magnitude*f_multitable[o]*(pow(sin((fraction)*PI/3),2)/1.5);
		image[0] = (iImage) & noise_frames_m1;
		image[1] = (iImage+1) & noise_frames_m1;
		image[2] = (iImage+2) & noise_frames_m1;
		{	
			for(int i=0; i<n_size_sq; i++)
			{
				o_noise[i + n_size_sq*o] =	(	((amount[0] * noise[i + n_size_sq * image[0]])>>scale_decimalbits) + 
					((amount[1] * noise[i + n_size_sq * image[1]])>>scale_decimalbits) + 
					((amount[2] * noise[i + n_size_sq * image[2]])>>scale_decimalbits));
			}
		}

		r_timemulti *= prm->get_float( p_fTimemulti );
	}

	if(packednoise)
	{
		int octavepack = 0;
		for(int o=0; o<octaves; o+=n_packsize)
		{
			for(int v=0; v<np_size; v++)
				for(int u=0; u<np_size; u++)
				{
					p_noise[v*np_size+u+octavepack*np_size_sq] = o_noise[(o+3)*n_size_sq + (v&n_size_m1)*n_size + (u&n_size_m1)];
					p_noise[v*np_size+u+octavepack*np_size_sq] += _mapsample( u, v, 3, o);
					p_noise[v*np_size+u+octavepack*np_size_sq] += _mapsample( u, v, 2, o+1);
					p_noise[v*np_size+u+octavepack*np_size_sq] += _mapsample( u, v, 1, o+2);				
				}
				octavepack++;

				/*for(int v=0; v<20; v++)
				for(int u=0; u<20; u++)
				p_noise[v*np_size+u] = 1000;*/
				// debug box

		}
	}
}
//------------------------------------------------------------------------------------------------------
void WaterGrid::_calc_normals()
{
	for ( int v = 1 ; v < SURFACE_LONG-1; v++ )
	{
		for ( int u = 1 ; u < SURFACE_WIDE-1; u++ )
		{
			vector3f vec1(	m_Vertex[v*SURFACE_WIDE + u + 1].Position().m_x-m_Vertex[v*SURFACE_WIDE + u - 1].Position().m_x,
				m_Vertex[v*SURFACE_WIDE + u + 1].Position().m_y-m_Vertex[v*SURFACE_WIDE + u - 1].Position().m_y, 
				m_Vertex[v*SURFACE_WIDE + u + 1].Position().m_z-m_Vertex[v*SURFACE_WIDE + u - 1].Position().m_z);

			vector3f vec2(	m_Vertex[(v+1)*SURFACE_WIDE + u].Position().m_x - m_Vertex[(v-1)*SURFACE_WIDE + u].Position().m_x,
				m_Vertex[(v+1)*SURFACE_WIDE + u].Position().m_y - m_Vertex[(v-1)*SURFACE_WIDE + u].Position().m_y,
				m_Vertex[(v+1)*SURFACE_WIDE + u].Position().m_z - m_Vertex[(v-1)*SURFACE_WIDE + u].Position().m_z);

			vector3f normal = Math::Cross( vec2, vec1 );
			normal.NormalizeSelf();
			m_Vertex[v*SURFACE_WIDE + u].Normal() = normal;

		}
	}
}











/*
//------------------------------------------------------------------------------------------------------
void WaterGrid::Update( uint milisecon )
{
this->_CaculateNoise();
float magnitude = n_dec_magn * prm->get_float(p_fScale);

IDriver& driver = IDriver::Instance();
ICamera& camera = driver.GetCamera();

float ViewFar = camera.GetFar();
Rayf raylt, raylb, rayrt, rayrb;//计算四条射线尝试与水平面相交
vector3f plt, plb, prt, prb;//四个交点
vector3f splt, splb;//屏幕空间的坐上角开始，和结束
camera.PickRay( vector2f(-1,1), raylt );
camera.PickRay( vector2f(-1,-1), raylb );
camera.PickRay( vector2f(1,1), rayrt );
camera.PickRay( vector2f(1,-1), rayrb );
raylt.m_RayOrig = camera.GetPosition();
raylb.m_RayOrig = camera.GetPosition();
rayrt.m_RayOrig = camera.GetPosition();
rayrb.m_RayOrig = camera.GetPosition();
{//计算摄像机的可见性
plb = raylb.m_RayOrig + raylb.m_RayDir * ViewFar;
prb = rayrb.m_RayOrig + rayrb.m_RayDir * ViewFar;
if( plb.m_y > m_WaterHight && prb.m_y > m_WaterHight )//如果远平面与bottom的焦点已经高于水高度了，说明水是看不到的
{
m_isRender = false;
return;
}
}
//计算全部的交点
plb = m_WaterPlane.IntersectRay( raylb ); //下方交点
prb = m_WaterPlane.IntersectRay( rayrb );
//计算上交点
plt = raylt.m_RayOrig + raylt.m_RayDir * ViewFar;
if( plt.m_y > m_WaterHight )//如果与远平面的交点已经超过了水体高度，说明raylt无法与水面相交了，转换计算最远点为远平面与水面的相交
{
Rayf rayl, rayr;//远平面左右两条射线
rayl.m_RayOrig = plt;
rayr.m_RayOrig = rayrt.m_RayOrig + rayrt.m_RayDir * ViewFar;

rayl.m_RayDir = raylb.m_RayOrig + raylb.m_RayDir * ViewFar;
rayr.m_RayDir = rayrb.m_RayOrig + rayrb.m_RayDir * ViewFar;

rayl.m_RayDir -= rayl.m_RayOrig;
rayl.m_RayDir.NormalizeSelf();

rayr.m_RayDir -= rayr.m_RayOrig;
rayr.m_RayDir.NormalizeSelf();

plt = m_WaterPlane.IntersectRay( rayl );
prt = m_WaterPlane.IntersectRay( rayr );
}
else
{
plt = m_WaterPlane.IntersectRay( raylt ); //上方交点
prt = m_WaterPlane.IntersectRay( rayrt ); //上方交点
}
//计算屏幕空间的投影点
splt = vector4f(plt) * camera.GetViewProj();
splb = vector4f(plb) * camera.GetViewProj();
vector3f SceneDir = ( splb - splt ) / (SURFACE_LONG-1);
//有四个交点后，计算网格
vector3f rDir = prb - prt;//计算右面的递增方向
rDir.NormalizeSelf();
vector3f lBegin = plt;//左端的开始
vector3f rBegin = prt;//右端的开始
for ( int z = 0 ; z < SURFACE_LONG; z ++ )
{
vector3f wPoint = lBegin;//点的开始
vector3f hWorldStep = (rBegin - lBegin) / (SURFACE_WIDE-1);//计算水平的增长
for ( int x = 0 ; x < SURFACE_WIDE; x ++ )
{
m_Vertex[ x + z*SURFACE_WIDE].Position() = vector4f( wPoint.m_x,
m_WaterHight ,//+ _get_height_dual(magnitude*wPoint.m_x, magnitude*wPoint.m_z ),
wPoint.m_z );
wPoint += hWorldStep;
}
splt += SceneDir;
vector3f worldPostion = vector3f( vector4f(splt) * camera.GetUnViewProj() );
lBegin = worldPostion - lBegin;
float worldLebgth = lBegin.Length();//计算该递增产生的世界空间的长度

lBegin = worldPostion;//计算新的左端
rBegin += rDir * worldLebgth;//计算新的右端
}
this->_calc_normals();
m_pBuffer->FlushVertexBuffer( &m_Vertex[0], 0, m_Vertex.size() );
m_isRender = true;
}
*/