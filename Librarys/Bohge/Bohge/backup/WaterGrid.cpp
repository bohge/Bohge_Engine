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
	plane.FromPointNormal( vector3f(0,0,0), vector3f(0,1,0) );//构造一个水体平面，做投影
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
vector4f WaterGrid::_calc_worldpos(vector2f uv, const Matrix44f &m )
{	
	// this is hacky.. this does take care of the homogenous coordinates in a correct way, 
	// but only when the plane lies at y=0
	vector4f	origin(uv.m_x,uv.m_y,-1,1);
	vector4f	direction(uv.m_x,uv.m_y,1,1);

	origin = origin * m;
	direction = direction * m;
	direction -= origin;    
	float	l = (-origin.m_y) / direction.m_y;	// assumes the plane is y=0

	vector4f worldPos = origin + direction*l;    
	return worldPos;
}
//------------------------------------------------------------------------------------------------------
void WaterGrid::Update( uint milisecon )
{
	this->_CaculateNoise();
	float magnitude = n_dec_magn * prm->get_float(p_fScale);
	float inv_magnitude_sq = 1.0f/(prm->get_float(p_fScale)*prm->get_float(p_fScale));

	Matrix44f m;
	this->_getMinMax(m);

	t_corners0 = _calc_worldpos(vector2f( 0.0f,+1.0f),m);
	t_corners1 = _calc_worldpos(vector2f(+1.0f,+1.0f),m);
	t_corners2 = _calc_worldpos(vector2f( 0.0f, 0.0f),m);
	t_corners3 = _calc_worldpos(vector2f(+1.0f, 0.0f),m);



	float	du = 1.0f/float(SURFACE_WIDE-1),
		dv = 1.0f/float(SURFACE_LONG-1),
		u,v=0.0f;
	vector4f result;
	int i=0;
	for(int iv=0; iv<SURFACE_LONG; iv++)
	{
		u = 0.0f;		
		for(int iu=0; iu<SURFACE_WIDE; iu++)
		{				

			//result = (1.0f-v)*( (1.0f-u)*t_corners0 + u*t_corners1 ) + v*( (1.0f-u)*t_corners2 + u*t_corners3 );				
			result.m_x = (1.0f-v)*( (1.0f-u)*t_corners0.m_x + u*t_corners1.m_x ) + v*( (1.0f-u)*t_corners2.m_x + u*t_corners3.m_x );				
			result.m_z = (1.0f-v)*( (1.0f-u)*t_corners0.m_z + u*t_corners1.m_z ) + v*( (1.0f-u)*t_corners2.m_z + u*t_corners3.m_z );				
			result.m_w = (1.0f-v)*( (1.0f-u)*t_corners0.m_w + u*t_corners1.m_w ) + v*( (1.0f-u)*t_corners2.m_w + u*t_corners3.m_w );				

			float divide = 1.0f/result.m_w;				
			result.m_x *= divide;
			result.m_z *= divide;

			m_Vertex[ iu + iv*SURFACE_WIDE].Position() = vector4f( result.m_x,
				m_WaterHight + _get_height_dual(magnitude*result.m_x, magnitude*result.m_z ),
				result.m_z );
			i++;
			u += du;
		}
		v += dv;			
	}
	//this->_calc_normals();
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
	plane.FromPointNormal( vector3f(0,m_WaterHight,0), vector3f(0,1,0) );//构造一个水体平面，做投影
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
//------------------------------------------------------------------------------------------------------
bool WaterGrid::_getMinMax( Matrix44f& out )
{
	ICamera& rendering_camera = IDriver::Instance().GetCamera();
	_set_displacement_amplitude(prm->params[p_fStrength].fData);
	float		x_min,y_min,x_max,y_max;
	vector3f frustum[8],proj_points[24];		// frustum to check the camera against

	int n_points=0;
	int cube[] = {	0,1,	0,2,	2,3,	1,3,
		0,4,	2,6,	3,7,	1,5,
		4,6,	4,5,	5,7,	6,7};	// which frustum points are connected together?

	// transform frustum points to worldspace (should be done to the rendering_camera because it's the interesting one)
	frustum[0] = vector3f( vector4f(-1,-1,-1) * rendering_camera.GetUnViewProj() );
	frustum[1] = vector3f( vector4f(+1,-1,-1) * rendering_camera.GetUnViewProj() );
	frustum[2] = vector3f( vector4f(-1,+1,-1) * rendering_camera.GetUnViewProj() );
	frustum[3] = vector3f( vector4f(+1,+1,-1) * rendering_camera.GetUnViewProj() );
	frustum[4] = vector3f( vector4f(-1,-1,+1) * rendering_camera.GetUnViewProj() );
	frustum[5] = vector3f( vector4f(+1,-1,+1) * rendering_camera.GetUnViewProj() );
	frustum[6] = vector3f( vector4f(-1,+1,+1) * rendering_camera.GetUnViewProj() );
	frustum[7] = vector3f( vector4f(+1,+1,+1) * rendering_camera.GetUnViewProj() );	


	// check intersections with upper_bound and lower_bound	
	Rayf r;
	for(int i=0; i<12; i++){
		int src=cube[i*2], dst=cube[i*2+1];
		if ((upper_bound.m_a*frustum[src].m_x + upper_bound.m_b*frustum[src].m_y + upper_bound.m_c*frustum[src].m_z + upper_bound.m_d*1)/(upper_bound.m_a*frustum[dst].m_x + upper_bound.m_b*frustum[dst].m_y + upper_bound.m_c*frustum[dst].m_z + upper_bound.m_d*1)<0){			
			r.m_RayOrig = frustum[src];
			r.m_RayDir = frustum[dst] - frustum[src];
			r.m_RayDir.NormalizeSelf();
			proj_points[n_points++] = upper_bound.IntersectRay(r);
		}
		if ((lower_bound.m_a*frustum[src].m_x + lower_bound.m_b*frustum[src].m_y + lower_bound.m_c*frustum[src].m_z + lower_bound.m_d*1)/(lower_bound.m_a*frustum[dst].m_x + lower_bound.m_b*frustum[dst].m_y + lower_bound.m_c*frustum[dst].m_z + lower_bound.m_d*1)<0){			
			r.m_RayOrig = frustum[src];
			r.m_RayDir = frustum[dst] - frustum[src];
			r.m_RayDir.NormalizeSelf();
			proj_points[n_points++] = lower_bound.IntersectRay(r);		
		}
	}
	// check if any of the frustums vertices lie between the upper_bound and lower_bound planes
	{
		for(int i=0; i<8; i++){		
			if ((upper_bound.m_a*frustum[i].m_x + upper_bound.m_b*frustum[i].m_y + upper_bound.m_c*frustum[i].m_z + upper_bound.m_d*1)/(lower_bound.m_a*frustum[i].m_x + lower_bound.m_b*frustum[i].m_y + lower_bound.m_c*frustum[i].m_z + lower_bound.m_d*1)<0){			
				proj_points[n_points++] = frustum[i];
			}		
		}	
	}

	//
	// create the camera the grid will be projected from
	//
	StaticCamera projecting_camera;
	projecting_camera.CreatePerspectiveProjection( rendering_camera.Fov(), rendering_camera.Aspect(), rendering_camera.GetNear(), rendering_camera.GetFar() );
	projecting_camera.LookAt( rendering_camera.GetPosition(), rendering_camera.GetLookAt(), rendering_camera.GetUp() );

	// make sure the camera isn't too close to the plane
	float height_in_plane = (lower_bound.m_a*projecting_camera.GetPosition().m_x +
		lower_bound.m_b*projecting_camera.GetPosition().m_y +
		lower_bound.m_c*projecting_camera.GetPosition().m_z);

	bool keep_it_simple = false;
	bool underwater=false;

	if (height_in_plane < 0.0f) underwater = true;

	if(keep_it_simple)
	{
		projecting_camera.SetForward( rendering_camera.GetForward() );
	}
	else
	{
		vector3f aimpoint, aimpoint2;		

		if (height_in_plane < (prm->params[p_fStrength].fData+prm->get_float(p_fElevation)))
		{					
			if(underwater)
				projecting_camera.SetPosition( projecting_camera.GetPosition() + vector3f(lower_bound.m_a,lower_bound.m_b,lower_bound.m_c)*(prm->params[p_fStrength].fData + prm->get_float(p_fElevation) - 2*height_in_plane) );
			else
				projecting_camera.SetPosition( projecting_camera.GetPosition() + vector3f(lower_bound.m_a,lower_bound.m_b,lower_bound.m_c)*(prm->params[p_fStrength].fData + prm->get_float(p_fElevation) - height_in_plane) );
		} 

		// aim the projector at the point where the camera view-vector intersects the plane
		// if the camera is aimed away from the plane, mirror it's view-vector against the plane
		if( (Math::Dot(plane.Normal(), rendering_camera.GetForward()) < 0.0f) log_xor (Math::Dot(plane, rendering_camera.GetPosition()) < 0.0f ) )
		{
			r.m_RayOrig = rendering_camera.GetPosition();
			r.m_RayDir = rendering_camera.GetPosition() + rendering_camera.GetForward() - rendering_camera.GetPosition();;
			r.m_RayDir.NormalizeSelf();
			aimpoint = plane.IntersectRay(r);
			//D3DXPlaneIntersectLine( &aimpoint, &plane, &(rendering_camera->position), &(rendering_camera->position + rendering_camera->forward) );			
		}
		else
		{
			vector3f flipped;
			flipped = rendering_camera.GetForward() - vector3f(0,1,0)*Math::Dot(rendering_camera.GetForward(),vector3f(0,1,0))*2;
			r.m_RayOrig = rendering_camera.GetPosition();
			r.m_RayDir = rendering_camera.GetPosition() + flipped - rendering_camera.GetPosition();;
			r.m_RayDir.NormalizeSelf();
			aimpoint = plane.IntersectRay(r);
			//D3DXPlaneIntersectLine( &aimpoint, &plane, &(rendering_camera.GetPosition()), &(rendering_camera->position + flipped) );			
		}

		// force the point the camera is looking at in a plane, and have the projector look at it
		// works well against horizon, even when camera is looking upwards
		// doesn't work straight down/up
		float af = fabs(Math::Dot(plane.Normal(), rendering_camera.GetForward()));
		//af = 1 - (1-af)*(1-af)*(1-af)*(1-af)*(1-af);
		//aimpoint2 = (rendering_camera->position + rendering_camera->zfar * rendering_camera->forward);
		aimpoint2 = (rendering_camera.GetPosition() + rendering_camera.GetForward() * 10.0f);
		aimpoint2 = aimpoint2 - vector3f(0,1,0)*Math::Dot(aimpoint2,vector3f(0,1,0));

		// fade between aimpoint & aimpoint2 depending on view angle

		aimpoint = aimpoint*af + aimpoint2*(1.0f-af);
		//aimpoint = aimpoint2;

		projecting_camera.SetForward( aimpoint-projecting_camera.GetPosition() );
	}



	{
		for(int i=0; i<n_points; i++){
			// project the point onto the surface plane
			proj_points[i] = proj_points[i] - vector3f(0,1,0)*Math::Dot(proj_points[i],vector3f(0,1,0));	
		}
	}

	{
		for(int i=0; i<n_points; i++)
		{
			proj_points[i] = vector3f( vector4f( proj_points[i] ) * projecting_camera.GetViewProj() );
		}
	}

	// debughonk

	/*	for(int i=0; i<n_points; i++){
	sprintf( debugdata, "%s%f  %f  %f\n",debugdata,proj_points[i].m_x,proj_points[i].m_y,proj_points[i].m_z);
	}*/

	// get max/min x & y-values to determine how big the "projection window" must be
	if (n_points > 0){
		x_min = proj_points[0].m_x;
		x_max = proj_points[0].m_x;
		y_min = proj_points[0].m_y;
		y_max = proj_points[0].m_y;
		for(int i=1; i<n_points; i++){
			if (proj_points[i].m_x > x_max) x_max = proj_points[i].m_x;
			if (proj_points[i].m_x < x_min) x_min = proj_points[i].m_x;
			if (proj_points[i].m_y > y_max) y_max = proj_points[i].m_y;
			if (proj_points[i].m_y < y_min) y_min = proj_points[i].m_y;
		}

		//sprintf( debugdata,	"%slimit_y_upper = %f\n",debugdata,limit_y_upper);
		//		sprintf( debugdata, "%sy1 = [%f] y2 = [%f]\n",debugdata,y1,y2);

		// build the packing matrix that spreads the grid across the "projection window"
		Matrix44f pack(	
			x_max-x_min,	0,				0,		0,
			0,				y_max-y_min,	0,		0,
			0,				0,				1,		0,	
			x_min,			y_min,			0,		1);
		out = pack * projecting_camera.GetUnViewProj();
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------------------------------
void WaterGrid::_set_displacement_amplitude( float amplitude )
{
	upper_bound.FromPointNormal( vector3f(0,m_WaterHight+amplitude,0), vector3f(0,1,0) );
	lower_bound.FromPointNormal( vector3f(0,m_WaterHight-amplitude,0), vector3f(0,1,0) );
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