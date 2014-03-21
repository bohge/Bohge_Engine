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
#include "Frustum.hpp"
#include "Ray.h"
#include "ShaderManager.h"
#include "Material.h"
#include "Log.h"
#include "Environment.h"
//#include "Ocean.h"
#ifdef TERRAINMANAGER
#include "TerrainManager.h"
#endif
#ifdef TERRAINEDIT
#include "TerrainEdit.h"
#endif

#include <math.h>
#include <pthread.h>


namespace BohgeEngine
{
#define SURFACE_LONG 128 //因为索引时short，所以256*256就是极限了，上不去了
#define SURFACE_WIDE 64
#define SURFACE_SIZE (SURFACE_LONG*SURFACE_WIDE)


	using namespace std;


	//多线程计算
	pthread_t g_ThreadID;
	static bool g_isWork = false;
	static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
	static pthread_mutex_t mtxRender = PTHREAD_MUTEX_INITIALIZER;
	static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
	void* DoWaterUpdate( void* prama )
	{
		WaterGrid* pWater = static_cast<WaterGrid*>(prama);
		pthread_mutex_lock(&mtx);
		//这个mutex_lock主要是用来保护wait等待临街时期的情况，
		//当在wait为放入队列时，这时，已经存在Head等待条件激活的条件，此时可能会漏掉这种处理
		//这个while要特别说明一下，单个pthread_cond_wait功能很完善，
		//为何这里要有一个while (head == NULL)呢？因为pthread_cond_wait里的线程可能会被意外唤醒，如果这个时候head != NULL，
		//则不是我们想要的情况。这个时候，应该让线程继续进入pthread_cond_wait
		while (true)
		{
			pthread_cond_wait(&cond, &mtx);
			if( false == g_isWork)
			{
				break;
			}
			//pthread_cond_wait会先解除之前的pthread_mutex_lock锁定的mtx，然后阻塞在等待对列里休眠，直到再次被唤醒
			//（大多数情况下是等待的条件成立而被唤醒，唤醒后，该进程会先锁定先pthread_mutex_lock(&mtx);，
			//再读取资源 用这个流程是比较清楚的/*block-->unlock-->wait() return-->lock*/
			pWater->DoUpdata();
			//pWater->DoUpdataFFT();
			pthread_mutex_unlock(&mtxRender); //临界区数据操作完毕，释放互斥锁
		}
		return NULL;
	}


	//------------------------------------------------------------------------------------------------------
#ifdef TERRAINMANAGER
	WaterGrid::WaterGrid( Texture2D& scene, Texture2D& depth, const TerrainManager& terrain )
#endif
#ifdef TERRAINEDIT
		WaterGrid::WaterGrid( Texture2D& scene, Texture2D& depth, const TerrainEdit& terrain )
#endif
		:m_isRender(false),
		m_pRendBuffer(NULL),
		time(0.0f),
		m_isChange(false),
		m_Terrain(terrain)
	{
		//启动计算线程
		g_isWork = true;
		pthread_create(&g_ThreadID,NULL, DoWaterUpdate, this);
		plane.FromPointNormal( vector3f(0,0,0), vector3f(0,1,0) );//构造一个水体平面，做投影

		m_Vertex.resize( SURFACE_SIZE );
		m_pRendBuffer = NEW RendBuffer( 
			//Device::LINE_STRIP );
			Device::RM_TRIANGLE_STRIP );
		m_pVertexBuffer = NEW VertexBuffer( Device::MU_DYNAMIC );
		m_pIndicesBuffer = NEW IndicesBuffer( Device::MU_STATIC );
		m_pRendBuffer->SetVertexBuffer( m_pVertexBuffer );
		m_pRendBuffer->SetIndicesBuffer( m_pIndicesBuffer );
		{//纹理坐标
			for ( int z = 0 ; z < SURFACE_LONG; z ++ )
			{
				for ( int x = 0 ; x < SURFACE_WIDE; x ++ )
				{
					//m_Vertex[ x + z*SURFACE_WIDE].Coordinate() = vector2f(0,0);
					m_Vertex[ x + z*SURFACE_WIDE].Normal() = vector3f(0,1,0);
				}
			}
		}
		m_pVertexBuffer->FlushVertexBuffer( m_Vertex.size(), &m_Vertex[0] );
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
			m_pIndicesBuffer->FlushIndiesBuffer( Indices.size(), &Indices[0] );
		}
		{//材质
			m_pMaterial = NEW Material;
			m_pMaterial->Ambient() = vector3f( 0.3764, 0.6953, 0.6055 ); //浅滩颜色
			m_pMaterial->Diffuse() = vector3f( 0.22, 0.353, 0.502 );//深水颜色
			m_pMaterial->Specular() = vector3f( 0.2, 0.2, 0.2 );
			m_pMaterial->SpeLevel() = 6;
			m_pMaterial->Shininess() = 84;
			m_pMaterial->SetNode( this );
			m_pMaterial->LoadTexture( Material::ATT_TEX_USER0, depth );
			m_pMaterial->LoadTexture( Material::ATT_TEX_REFRACTION, scene );

			//Texture* temp = m_pMaterial->LoadTexture( Material::ATT_TEX_OPACITY, "WaterNoise.png" );
			//temp->SetTextureWarp( Device::TW_REPEAT, Device::TW_REPEAT );
		}
		prm = NEW parameterhandler();//参数
		this->_InitNoise();

		//m_pOcean = NEW Ocean( 64, 0.0005f, vector2f(100.0f,100.0f), 80, false );
	}
	//------------------------------------------------------------------------------------------------------
	WaterGrid::~WaterGrid(void)
	{
		pthread_mutex_lock(&mtx); //需要操作head这个临界资源，先加锁，
		g_isWork = false;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mtx); //解锁
		SAFE_DELETE( prm );
		SAFE_DELETE( m_pRendBuffer );
		SAFE_DELETE( m_pVertexBuffer );
		SAFE_DELETE( m_pIndicesBuffer );
		SAFE_DELETE( m_pMaterial );
		//SAFE_DELETE( m_pOcean );
	}
	//------------------------------------------------------------------------------------------------------
	void WaterGrid::BeginUpdate( Engine& engine )
	{
		pthread_mutex_lock(&mtx); //需要操作head这个临界资源，先加锁，
		m_UpdataDone = false;
		m_RenderCamera = *engine.GetCamera();
		float itime = engine.GetTimeSpan();
		if ( itime >= 1000 )
		{
			itime = 33;
		}
		static float lp_itime=0.0;	
		itime *= 0.001 * prm->get_float( p_fAnimspeed );
		lp_itime = 0.99*lp_itime + 0.01 * itime;
		time += lp_itime;
		pthread_cond_signal(&cond);
		pthread_mutex_lock(&mtxRender);
		pthread_mutex_unlock(&mtx); //解锁
	}
	//------------------------------------------------------------------------------------------------------
	void WaterGrid::Render( Engine& engine )
	{
		engine.GetDevice()->Draw( *m_pRendBuffer, *m_pMaterial );
	}
	//------------------------------------------------------------------------------------------------------
	void WaterGrid::EndUpdate( Engine& engine )
	{
		pthread_mutex_lock(&mtxRender); //等待辅助线程处理完毕
		if( m_isRender )
		{
			if( m_isChange )
			{
				m_isChange = false;
				m_pVertexBuffer->FlushVertexBuffer( m_Vertex.size(), &m_Vertex[0] );
			}
			if ( engine.GetEnvironment()->isCastShadow() )//场景中的灯光只要有要发出阴影的，海水一半都会接收，应为海水是一个整块
			{
				m_pMaterial->SetShader( Pipeline::PT_LIGHTING, ShaderManager::WaterShader );
			}
			else
			{
				m_pMaterial->SetShader( Pipeline::PT_LIGHTING, ShaderManager::WaterShaderNoShadow );
			}
		}
		pthread_mutex_unlock(&mtxRender); //解锁
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
	////------------------------------------------------------------------------------------------------------
	//void WaterGrid::DoUpdataFFT()
	//{
	//	float magnitude = n_dec_magn * prm->get_float(p_fScale);
	//	float inv_magnitude_sq = 1.0f/(prm->get_float(p_fScale)*prm->get_float(p_fScale));
	//
	//	Matrix44f m;
	//	this->_getMinMax(m);
	//
	//	t_corners0 = this->_calc_worldpos(vector2f( 0.0f,+1.0f),m);
	//	t_corners1 = this->_calc_worldpos(vector2f(+1.0f,+1.0f),m);
	//	t_corners2 = this->_calc_worldpos(vector2f( 0.0f, 0.0f),m);
	//	t_corners3 = this->_calc_worldpos(vector2f(+1.0f, 0.0f),m);
	//
	//	float	du = 1.0f/float(SURFACE_WIDE-1),
	//		dv = 1.0f/float(SURFACE_LONG-1),
	//		u,v=0.0f;
	//	vector4f result;
	//	int i=0;
	//	float wh = prm->params[p_fWaterBaseHeight].fData;
	//	for(int iv=0; iv<SURFACE_LONG; iv++)
	//	{
	//		u = 0.0f;		
	//		for(int iu=0; iu<SURFACE_WIDE; iu++)
	//		{				
	//			result.m_x = (1.0f-v)*( (1.0f-u)*t_corners0.m_x + u*t_corners1.m_x ) + v*( (1.0f-u)*t_corners2.m_x + u*t_corners3.m_x );				
	//			result.m_z = (1.0f-v)*( (1.0f-u)*t_corners0.m_z + u*t_corners1.m_z ) + v*( (1.0f-u)*t_corners2.m_z + u*t_corners3.m_z );				
	//			result.m_w = (1.0f-v)*( (1.0f-u)*t_corners0.m_w + u*t_corners1.m_w ) + v*( (1.0f-u)*t_corners2.m_w + u*t_corners3.m_w );				
	//
	//			float divide = 1.0f/result.m_w;				
	//			result.m_x *= divide;
	//			result.m_z *= divide;
	//
	//			int Index = iu + iv*SURFACE_WIDE;
	//			m_Vertex[Index].Position() = vector4f( result.m_x,
	//				0.0,
	//				result.m_z,
	//				0.0 );
	//			i++;
	//			u += du;
	//		}
	//		v += dv;			
	//	}
	//
	//	//for(int iv=0; iv<SURFACE_LONG; iv++)
	//	//{	
	//	//	for(int iu=0; iu<SURFACE_WIDE; iu++)
	//	//	{
	//	//		int Index = iu + iv*SURFACE_WIDE;
	//	//		m_pOcean->Vertexs()[Index].ox = m_Vertex[Index].Position().m_x;
	//	//		m_pOcean->Vertexs()[Index].oz = m_Vertex[Index].Position().m_z;
	//	//	}
	//	//}
	//
	//	//m_pOcean->evaluateWavesFFT( time );
	//	for(int iv=0; iv<SURFACE_LONG; iv++)
	//	{	
	//		for(int iu=0; iu<SURFACE_WIDE; iu++)
	//		{
	//			int Index = iu + iv*SURFACE_WIDE;
	//			m_Vertex[Index].Position().m_x += m_pOcean->Vertexs()[Index].ox - m_pOcean->Vertexs()[Index].x;
	//			m_Vertex[Index].Position().m_y += m_pOcean->Vertexs()[Index].oy - m_pOcean->Vertexs()[Index].y;
	//			m_Vertex[Index].Position().m_z += m_pOcean->Vertexs()[Index].oz - m_pOcean->Vertexs()[Index].z;
	//
	//			//m_Vertex[Index].Normal().m_x = m_pOcean->Vertexs()[Index].nx;
	//			//m_Vertex[Index].Normal().m_y = m_pOcean->Vertexs()[Index].ny;
	//			//m_Vertex[Index].Normal().m_z = m_pOcean->Vertexs()[Index].nz;
	//		}
	//	}
	//		this->_calc_normals();
	//	m_isChange = true;
	//	m_isRender = true;
	//	m_UpdataDone = true;
	//}
	//------------------------------------------------------------------------------------------------------
	void WaterGrid::DoUpdata()
	{
		this->_CaculateNoise();
		float magnitude = n_dec_magn * prm->get_float(p_fScale);
		float inv_magnitude_sq = 1.0f/(prm->get_float(p_fScale)*prm->get_float(p_fScale));

		Matrix44f m;
		this->_getMinMax(m);

		t_corners0 = this->_calc_worldpos(vector2f( 0.0f,+1.0f),m);
		t_corners1 = this->_calc_worldpos(vector2f(+1.0f,+1.0f),m);
		t_corners2 = this->_calc_worldpos(vector2f( 0.0f, 0.0f),m);
		t_corners3 = this->_calc_worldpos(vector2f(+1.0f, 0.0f),m);

		float	du = 1.0f/float(SURFACE_WIDE-1),
			dv = 1.0f/float(SURFACE_LONG-1),
			u,v=0.0f;
		vector4f result;
		int i=0;
		float wh = prm->params[p_fWaterBaseHeight].fData;
		float colormaxdepth = 0.99 / prm->params[p_fColorChangeDepthInv].fData;
		for(int iv=0; iv<SURFACE_LONG; iv++)
		{
			u = 0.0f;		
			for(int iu=0; iu<SURFACE_WIDE; iu++)
			{				
				result.m_x = (1.0f-v)*( (1.0f-u)*t_corners0.m_x + u*t_corners1.m_x ) + v*( (1.0f-u)*t_corners2.m_x + u*t_corners3.m_x );				
				result.m_z = (1.0f-v)*( (1.0f-u)*t_corners0.m_z + u*t_corners1.m_z ) + v*( (1.0f-u)*t_corners2.m_z + u*t_corners3.m_z );				
				result.m_w = (1.0f-v)*( (1.0f-u)*t_corners0.m_w + u*t_corners1.m_w ) + v*( (1.0f-u)*t_corners2.m_w + u*t_corners3.m_w );				

				float divide = 1.0f/result.m_w;				
				result.m_x *= divide;
				result.m_z *= divide;

				int Index = iu + iv*SURFACE_WIDE;
				float height;
				height = iv <= 1 ? wh : wh + this->_get_height_dual(magnitude*result.m_x, magnitude*result.m_z );
				m_Vertex[Index].Position() = vector4f( result.m_x, height, result.m_z,
					height - m_Terrain.GetHeightForWater( result.m_x, result.m_z, colormaxdepth ) );
				//m_Vertex[Index].Coordinate() = vector2f( 
				//	Math::Clamp0to1( height - prm->get_float(p_fNoiseheight) ),
				//	height - m_Terrain.GetHeightForWater( result.m_x, result.m_z ) );
				i++;
				u += du;
			}
			v += dv;			
		}
		//for(int v=1; v<(SURFACE_LONG-1); v++)
		//{
		//	for(int u=1; u<(SURFACE_WIDE-1); u++)
		//	{
		//		m_Vertex[v*SURFACE_WIDE + u].Position().m_y =	
		//			0.2f * (m_Vertex[v*SURFACE_WIDE + u].Position().m_y +
		//			m_Vertex[v*SURFACE_WIDE + (u+1)].Position().m_y + 
		//			m_Vertex[v*SURFACE_WIDE + (u-1)].Position().m_y + 
		//			m_Vertex[(v+1)*SURFACE_WIDE + u].Position().m_y + 
		//			m_Vertex[(v-1)*SURFACE_WIDE + u].Position().m_y);															
		//	}
		//}
		this->_calc_normals();
		m_isChange = true;
		m_isRender = true;
		m_UpdataDone = true;
	}
	//------------------------------------------------------------------------------------------------------
	void WaterGrid::WaterSurface( Material& m, parameterhandler& p )
	{
		*m_pMaterial = m;
		*prm = p;
		plane.FromPointNormal( vector3f(0, prm->params[p_fWaterBaseHeight].fData, 0), vector3f(0,1,0) );//构造一个水体平面，做投影
	}
	//------------------------------------------------------------------------------------------------------
	void WaterGrid::_InitNoise()
	{
		// create noise (uniform)
		float tempnoise[n_size_sq*noise_frames];
		for(int i=0; i<(n_size_sq*noise_frames); i++)
		{	
			float temp = (float) rand()/RAND_MAX;		
			tempnoise[i] = 4*(temp - 0.5f);	
		}	

		for(int frame=0; frame<noise_frames; frame++)
		{
			for(int v=0; v<n_size; v++)
			{
				for(int u=0; u<n_size; u++)
				{	
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
			//f_multitable[i] = powf(prm->get_float(p_fFalloff),1.0f*i);
			f_multitable[i] = Math::Pow(prm->get_float(p_fFalloff),1.0f*i);
			sum += f_multitable[i];
		}

		{
			for(int i=0; i<octaves; i++)
			{
				f_multitable[i] /= sum;
			}
		}

		{
			for(int i=0; i<octaves; i++)
			{
				multitable[i] = scale_magnitude*f_multitable[i];
			}
		}

		float	r_timemulti = 1.0;

		for(int o=0; o<octaves; o++)
		{		
			unsigned int image[3];
			int amount[3];
			//double dImage, fraction = modf(time*r_timemulti,&dImage);
			//int iImage = (int)dImage;
			//amount[0] = scale_magnitude*f_multitable[o]*(pow(sin((fraction+2)*PI/3),2)/1.5);
			//amount[1] = scale_magnitude*f_multitable[o]*(pow(sin((fraction+1)*PI/3),2)/1.5);
			//amount[2] = scale_magnitude*f_multitable[o]*(pow(sin((fraction)*PI/3),2)/1.5);
			int iImage(0);
			float fraction = Math::Mod<float>(time*r_timemulti, &iImage);
			amount[0] = scale_magnitude*f_multitable[o]*(Math::Pow<float>(Math::Sin((fraction+2)*PI_D_3),2)*0.66666666666666666666666666666667f);
			amount[1] = scale_magnitude*f_multitable[o]*(Math::Pow<float>(Math::Sin((fraction+1)*PI_D_3),2)*0.66666666666666666666666666666667f);
			amount[2] = scale_magnitude*f_multitable[o]*(Math::Pow<float>(Math::Sin((fraction)*PI_D_3),2)*0.66666666666666666666666666666667f);
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
				int index1 = v*SURFACE_WIDE + u + 1;
				int index2 = v*SURFACE_WIDE + u - 1;
				vector3f vec1(	m_Vertex[index1].Position().m_x-m_Vertex[index2].Position().m_x,
					m_Vertex[index1].Position().m_y-m_Vertex[index2].Position().m_y, 
					m_Vertex[index1].Position().m_z-m_Vertex[index2].Position().m_z);

				index1 = (v+1)*SURFACE_WIDE + u;
				index2 = (v-1)*SURFACE_WIDE + u;
				vector3f vec2(	m_Vertex[index1].Position().m_x - m_Vertex[index2].Position().m_x,
					m_Vertex[index1].Position().m_y - m_Vertex[index2].Position().m_y,
					m_Vertex[index1].Position().m_z - m_Vertex[index2].Position().m_z);
				Math::Cross( vec2, vec1, m_Vertex[v*SURFACE_WIDE + u].Normal() );
			}
		}
	}
	//------------------------------------------------------------------------------------------------------
	bool WaterGrid::_getMinMax( Matrix44f& out )
	{
		_set_displacement_amplitude(prm->params[p_fStrength].fData);
		float		x_min,y_min,x_max,y_max;
		vector3f frustum[8],proj_points[24];		// frustum to check the camera against

		int n_points=0;
		int cube[] = {	0,1,	0,2,	2,3,	1,3,
			0,4,	2,6,	3,7,	1,5,
			4,6,	4,5,	5,7,	6,7};	// which frustum points are connected together?

		// transform frustum points to worldspace (should be done to the m_RenderCamera because it's the interesting one)
		//m_RenderCamera.CreatePerspectiveProjection( m_RenderCamera.Fov(), m_RenderCamera.Aspect(), m_RenderCamera.GetNear(), m_RenderCamera.GetFar() + 0.01 * m_RenderCamera.GetFar() );
		//m_RenderCamera.Update();
		frustum[0] = vector3f( vector4f(-1,-1,-1) * m_RenderCamera.GetUnViewProj() );
		frustum[1] = vector3f( vector4f(+1,-1,-1) * m_RenderCamera.GetUnViewProj() );
		frustum[2] = vector3f( vector4f(-1,+1,-1) * m_RenderCamera.GetUnViewProj() );
		frustum[3] = vector3f( vector4f(+1,+1,-1) * m_RenderCamera.GetUnViewProj() );
		frustum[4] = vector3f( vector4f(-1,-1,+1) * m_RenderCamera.GetUnViewProj() );
		frustum[5] = vector3f( vector4f(+1,-1,+1) * m_RenderCamera.GetUnViewProj() );
		frustum[6] = vector3f( vector4f(-1,+1,+1) * m_RenderCamera.GetUnViewProj() );
		frustum[7] = vector3f( vector4f(+1,+1,+1) * m_RenderCamera.GetUnViewProj() );	


		// check intersections with upper_bound and lower_bound	
		Rayf r;
		for(int i=0; i<12; i++){
			int src=cube[i*2], dst=cube[i*2+1];
			if ((upper_bound.A()*frustum[src].m_x + upper_bound.B()*frustum[src].m_y + upper_bound.C()*frustum[src].m_z + upper_bound.D())/(upper_bound.A()*frustum[dst].m_x + upper_bound.B()*frustum[dst].m_y + upper_bound.C()*frustum[dst].m_z + upper_bound.D())<0){			
				r.SetRayOrigin( frustum[src] );
				r.SetRayDirection( (frustum[dst] - frustum[src]).NormalizeSelf() );
				proj_points[n_points++] = upper_bound.IntersectRay(r);
			}
			if ((lower_bound.A()*frustum[src].m_x + lower_bound.B()*frustum[src].m_y + lower_bound.C()*frustum[src].m_z + lower_bound.D())/(lower_bound.A()*frustum[dst].m_x + lower_bound.B()*frustum[dst].m_y + lower_bound.C()*frustum[dst].m_z + lower_bound.D())<0){			
				r.SetRayOrigin( frustum[src]);
				r.SetRayDirection( (frustum[dst] - frustum[src]).NormalizeSelf() );
				proj_points[n_points++] = lower_bound.IntersectRay(r);		
			}
		}
		// check if any of the frustums vertices lie between the upper_bound and lower_bound planes
		{
			for(int i=0; i<8; i++){		
				if ((upper_bound.A()*frustum[i].m_x + upper_bound.B()*frustum[i].m_y + upper_bound.C()*frustum[i].m_z + upper_bound.D())/(lower_bound.A()*frustum[i].m_x + lower_bound.B()*frustum[i].m_y + lower_bound.C()*frustum[i].m_z + lower_bound.D())<0){			
					proj_points[n_points++] = frustum[i];
				}		
			}	
		}

		//
		// create the camera the grid will be projected from
		//
		StaticCamera projecting_camera;
		projecting_camera.CreatePerspectiveProjection( m_RenderCamera.Fov(), m_RenderCamera.Aspect(), m_RenderCamera.GetNear(), m_RenderCamera.GetFar() );
		projecting_camera.LookAt( m_RenderCamera.GetPosition(), m_RenderCamera.GetLookAt(), m_RenderCamera.GetUp() );
		projecting_camera.Update();

		// make sure the camera isn't too close to the plane
		float height_in_plane = (lower_bound.A()*projecting_camera.GetPosition().m_x +
			lower_bound.B()*projecting_camera.GetPosition().m_y +
			lower_bound.C()*projecting_camera.GetPosition().m_z);

		bool keep_it_simple = false;
		bool underwater=false;

		if (height_in_plane < 0.0f) underwater = true;

		if(keep_it_simple)
		{
			projecting_camera.SetForward( m_RenderCamera.GetForward() );
			projecting_camera.Update();
		}
		else
		{
			vector3f aimpoint, aimpoint2;		

			if (height_in_plane < (prm->params[p_fStrength].fData+prm->get_float(p_fElevation)))
			{					
				if(underwater)
					projecting_camera.SetPosition( projecting_camera.GetPosition() + vector3f(lower_bound.A(),lower_bound.B(),lower_bound.C())*(prm->params[p_fStrength].fData + prm->get_float(p_fElevation) - 2*height_in_plane) );
				else
					projecting_camera.SetPosition( projecting_camera.GetPosition() + vector3f(lower_bound.A(),lower_bound.B(),lower_bound.C())*(prm->params[p_fStrength].fData + prm->get_float(p_fElevation) - height_in_plane) );
				projecting_camera.Update();
			} 

			// aim the projector at the point where the camera view-vector intersects the plane
			// if the camera is aimed away from the plane, mirror it's view-vector against the plane
			if( ( plane.Normal().Dot(m_RenderCamera.GetForward()) < 0.0f) != (Math::Dot(plane, m_RenderCamera.GetPosition()) < 0.0f ) )
			{
				r.SetRayOrigin( m_RenderCamera.GetPosition() );
				r.SetRayDirection( m_RenderCamera.GetForward() );
				aimpoint = plane.IntersectRay(r);			
			}
			else
			{
				vector3f flipped;
				flipped.m_y = m_RenderCamera.GetForward().m_y - m_RenderCamera.GetForward().m_y*2;
				r.SetRayOrigin( m_RenderCamera.GetPosition() );
				r.SetRayDirection( flipped.NormalizeSelf() );
				aimpoint = plane.IntersectRay(r);		
			}

			// force the point the camera is looking at in a plane, and have the projector look at it
			// works well against horizon, even when camera is looking upwards
			// doesn't work straight down/up
			float af = Math::Abs(m_RenderCamera.GetForward().m_y);
			//float af = fabs(m_RenderCamera.GetForward().m_y);
			aimpoint2 = (m_RenderCamera.GetPosition() + m_RenderCamera.GetForward() * 10.0f);
			aimpoint2.m_y -= aimpoint2.m_y;
			// fade between aimpoint & aimpoint2 depending on view angle
			aimpoint = aimpoint*af + aimpoint2*(1.0f-af);
			projecting_camera.SetForward( aimpoint-projecting_camera.GetPosition() );
			projecting_camera.Update();
		}



		{
			for(int i=0; i<n_points; i++){
				// project the point onto the surface plane
				proj_points[i].m_y -= proj_points[i].m_y;
			}
		}

		{
			for(int i=0; i<n_points; i++)
			{
				proj_points[i] = vector3f( vector4f( proj_points[i] ) * projecting_camera.GetViewProj() );
			}
		}

		// debughonk
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
		upper_bound.FromPointNormal( vector3f(0,prm->params[p_fWaterBaseHeight].fData+amplitude,0), vector3f(0,1,0) );
		lower_bound.FromPointNormal( vector3f(0,prm->params[p_fWaterBaseHeight].fData-amplitude,0), vector3f(0,1,0) );
	}
	//------------------------------------------------------------------------------------------------------
	void WaterGrid::SetLineMode( bool islinemode )
	{
		m_pRendBuffer->SetRenderMode( islinemode ? Device::RM_LINE_LOOP : Device::RM_TRIANGLE_STRIP );
	}
	//------------------------------------------------------------------------------------------------------
	const void* WaterGrid::Attribute( const Node_Attribute& name ) const
	{
		switch( name )
		{
		case IAttributeNode::ATT_USER_0: return &prm->params[p_bRefrStrength].fData;
		case IAttributeNode::ATT_USER_1: return &prm->params[p_fCanSeeDepth].fData;
		case IAttributeNode::ATT_USER_2: return &prm->params[p_fNoiseTile].fData;
		case IAttributeNode::ATT_USER_3: return &prm->params[p_fColorChangeDepthInv].fData;
		case IAttributeNode::ATT_USER_4: return &prm->params[p_fNoiseStrength].fData;
		case IAttributeNode::ATT_USER_5: return &prm->params[p_fNoiseWaveSpeed].fData;
		case IAttributeNode::ATT_USER_6: return &prm->params[p_fBumpScale].fData;
		case IAttributeNode::ATT_USER_7: return &prm->params[p_fBumpFarPer].fData;
		case IAttributeNode::ATT_USER_8: return &prm->params[p_fBumpStrength].fData;
		}
		return NULL;
	}
}