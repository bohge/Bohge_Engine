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
//	使用projectedGrid的水面//
/////////////////////////////

#pragma once
#include "Vertex.h"
#include "ISceneNode.h"
#include "Plane.hpp"
#include "parameterhandler.h"
#include "Texture.h"
#include "Camera.h"

#include <vector>


class TerrainEdit;
namespace BohgeEngine
{

#define n_bits				4 //修改这个会加速，5对应256*128，4对应128*64
#define n_size				(1<<(n_bits-1))
#define n_size_m1			(n_size - 1)
#define n_size_sq			(n_size*n_size)
#define n_size_sq_m1		(n_size_sq - 1)


#define n_packsize			4

#define np_bits				(n_bits+n_packsize-1)
#define np_size				(1<<(np_bits-1))
#define np_size_m1			(np_size-1)
#define np_size_sq			(np_size*np_size)
#define np_size_sq_m1		(np_size_sq-1)

#define n_dec_bits			12
#define n_dec_magn			4096
#define n_dec_magn_m1		4095

#define max_octaves			32

#define noise_frames		128//修改这个会加速，256对应256*128，128对应128*64
#define noise_frames_m1		(noise_frames-1)

#define noise_decimalbits	15
#define noise_magnitude		(1<<(noise_decimalbits-1))

#define scale_decimalbits	15
#define scale_magnitude		(1<<(scale_decimalbits-1))


	//摄像机是不能带有X轴上的旋转的
	//类使用了projected Grid 来创建一个水面
	//网格的索引顺序是
	//01
	//^
	//|
	//|
	//00------>10
	//class Ocean;
	class RendBuffer;
	class VertexBuffer;
	class IndicesBuffer;
	class NormalGenerate;
	class TerrainManager;
	class WaterGrid : public IAttributeNode
	{
	private:
		typedef std::vector<VertexNormal> VertexVector;
	private:
		volatile bool			m_isChange;
		volatile bool			m_UpdataDone;
		volatile bool			m_isRender;//是否需要绘制
		//绘制相关
		Material*				m_pMaterial;
		VertexVector			m_Vertex;
		RendBuffer*				m_pRendBuffer;//水体的buffer
		VertexBuffer*			m_pVertexBuffer;
		IndicesBuffer*			m_pIndicesBuffer;
		//地形管理器
#ifdef TERRAINMANAGER
		const TerrainManager&	m_Terrain;
#endif
#ifdef TERRAINEDIT
		const TerrainEdit&	m_Terrain;
#endif
	private://投影网格使用
		ICamera			m_RenderCamera;
		Planef	plane, upper_bound, lower_bound;
		vector4f t_corners0,t_corners1,t_corners2,t_corners3;
	private://水面噪声用的
		int					noise[n_size_sq*noise_frames];
		int					o_noise[n_size_sq*max_octaves];
		int					p_noise[np_size_sq*(max_octaves>>(n_packsize-1))];	
		int					*r_noise;
		int					octaves;
		parameterhandler	*prm;
		int					multitable[max_octaves];
		float				f_multitable[max_octaves];	
		double				time;

		//fft海面
		//Ocean*				m_pOcean;
	public:
#ifdef TERRAINMANAGER
		WaterGrid( Texture2D& scene, Texture2D& depth, const TerrainManager& terrain );
#else
		WaterGrid( Texture2D& scene, Texture2D& depth, const TerrainEdit& terrain );
#endif
		~WaterGrid(void);
	private:
		void _InitNoise();
		void _CaculateNoise();
		void _calc_normals();
		bool _getMinMax( Matrix44f& out );
		vector4f _calc_worldpos(vector2f uv, const Matrix44f &m );
		void _set_displacement_amplitude(float amplitude);
	private:
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE int _mapsample(int u, int v, int upsamplepower, int octave)
		{
			int magnitude = 1<<upsamplepower;
			int pu = u >> upsamplepower;
			int pv = v >> upsamplepower;	
			int fu = u & (magnitude-1);
			int fv = v & (magnitude-1);
			int fu_m = magnitude - fu;
			int fv_m = magnitude - fv;

			int o = fu_m*fv_m*o_noise[octave*n_size_sq + ((pv)&n_size_m1)*n_size + ((pu)&n_size_m1)] +
				fu*fv_m*o_noise[octave*n_size_sq + ((pv)&n_size_m1)*n_size + ((pu+1)&n_size_m1)] +
				fu_m*fv*o_noise[octave*n_size_sq + ((pv+1)&n_size_m1)*n_size + ((pu)&n_size_m1)] +
				fu*fv*o_noise[octave*n_size_sq + ((pv+1)&n_size_m1)*n_size + ((pu+1)&n_size_m1)];

			return o >> (upsamplepower+upsamplepower);
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE float _get_height_dual(int u, int v)
		{	
			int value=0;	
			r_noise = p_noise;	// pointer to the current noise source octave
			int hoct = octaves / n_packsize;
			for(int i=0; i<hoct; i++)
			{		
				value += _readtexel_linear_dual(u,v,0);
				u = u << n_packsize;
				v = v << n_packsize;
				r_noise += np_size_sq;
			}		
			return value*prm->params[p_fStrength].fData/noise_magnitude;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE int _readtexel_linear_dual(int u, int v, int o)
		{
			int iu, iup, iv, ivp, fu, fv;
			iu = (u>>n_dec_bits)&np_size_m1;
			iv = ((v>>n_dec_bits)&np_size_m1)*np_size;

			iup = ((u>>n_dec_bits) + 1)&np_size_m1;
			ivp = (((v>>n_dec_bits) + 1)&np_size_m1)*np_size;

			fu = u & n_dec_magn_m1;
			fv = v & n_dec_magn_m1;

			int ut01 = ((n_dec_magn-fu)*r_noise[iv + iu] + fu*r_noise[iv + iup])>>n_dec_bits;
			int ut23 = ((n_dec_magn-fu)*r_noise[ivp + iu] + fu*r_noise[ivp + iup])>>n_dec_bits;
			int ut = ((n_dec_magn-fv)*ut01 + fv*ut23) >> n_dec_bits;
			return ut;
		}
	public:
		virtual const void* Attribute(const Node_Attribute& name) const; //取得该节点的一写信息
	public:
		void DoUpdata();//多线程调用
		//void DoUpdataFFT();//多线程调用
		void BeginUpdate( Engine& engine );
		void Render(Engine& engine);
		void EndUpdate(Engine& engine);
		void WaterSurface( Material& m, parameterhandler& p );
		void SetLineMode( bool islinemode );//切换渲染模式
	};

}