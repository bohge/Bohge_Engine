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



/////////////////////////////
//			包装层		   //
/////////////////////////////
#pragma once
#include "VariableType.h"
#include "3DMath.h"

#include <string>
#include <vector>

typedef std::vector<float>	FloatVector;
typedef std::vector<int>	IntVector;

namespace BohgeEngine
{
	class Shader;
	class Texture;
	class RendBuffer;
	class RenderTarget;
	class TextureCube;
	class Texture2D;
	class Attributes;
	class Material;
	struct TextureData;
	class Device
	{
	public:
		enum Clear	//清理模板的参数
		{
			COLOR_BUFFER	= 1 << 0,
			DEPTH_BUFFER	= 1 << 1,
			STENCIL_BUFFER	= 1 << 2,
		};
		enum RenderMode	//顶点绘制的方式
		{
			RM_POINTS = 0,
			RM_LINES,
			RM_LINE_LOOP,
			RM_LINE_STRIP,
			RM_TRIANGLES,
			RM_TRIANGLE_STRIP,
			RM_TRIANGLE_FAN,
		};
		enum DataType
		{
			DT_BYTE = 0,
			DT_UNSIGNED_BYTE,
			DT_SHORT,
			DT_UNSIGNED_SHORT,
			DT_INT,
			DT_UNSIGNED_INT,
			DT_FLOAT,
		};
		enum Function
		{
			FN_NEVER = 0,
			FN_LESS,
			FN_EQUAL,
			FN_LEQUAL,
			FN_GREATER,
			FN_NOTEQUAL,
			FN_GEQUAL,
			FN_ALWAYS,
		};
		enum CullFaceMode
		{
			CFM_NONE = 0,
			CFM_FRONT,
			CFM_BACK,
		};
		enum Operation
		{
			ON_ZERO = 0,
			ON_ONE,
			ON_KEEP,
			ON_REPLACE,
			ON_INCR,
			ON_DECR,
			ON_INVERT,
			ON_INCR_WRAP,
			ON_DECR_WRAP,
		};
		enum Blend
		{
			BL_ZERO = 0,
			BL_ONE,
			BL_SRC_COLOR,
			BL_ONE_MINUS_SRC_COLOR,
			BL_SRC_ALPHA,
			BL_ONE_MINUS_SRC_ALPHA,
			BL_DST_ALPHA,
			BL_ONE_MINUS_DST_ALPHA ,
			BL_DST_COLOR,
			BL_ONE_MINUS_DST_COLOR,
		};
	public:
		enum State	//记录当前绘制状态
		{
			DEPTH_TEST		= 1 <<0,
			ALPHA_BLEND		= 1 <<1,
			CULL_FACE_TEST	= 1 <<2,
			STANCIL_TEST	= 1 <<3,
			ALPHA_TEST		= 1 <<4,
		};
	protected:
		bitmask			m_mRenderState;
	protected:
		BOHGE_FORCEINLINE void SetState( State state )
		{
			m_mRenderState |= state;
		}
		BOHGE_FORCEINLINE void EraseState( State state )
		{
			m_mRenderState &= ~state;
		}
	public:
		BOHGE_FORCEINLINE bool isState( State state )
		{
			return ( m_mRenderState & state ) != 0;
		}
	public:
		//设备属性
		void DeviceCaps();
	public:
		struct DrawState
		{
			bool			m_isDepthTest;
			bool			m_isDepthMask;
			Function		m_DepthFunction;
			CullFaceMode	m_isCullFace;
			DrawState()
				:m_isDepthTest(true),
				m_isDepthMask(true),
				m_DepthFunction(FN_LESS),
				m_isCullFace(CFM_BACK)
			{

			}
			BOHGE_FORCEINLINE bool operator == ( const DrawState& rhs) const
			{
				return m_isDepthMask == rhs.m_isDepthMask
					&& m_isDepthTest == rhs.m_isDepthTest
					&& m_DepthFunction == rhs.m_DepthFunction
					&& m_isCullFace == rhs.m_isCullFace;
			}
			BOHGE_FORCEINLINE const DrawState& operator = ( const DrawState& rhs)
			{
				m_isDepthMask = rhs.m_isDepthMask;
				m_isDepthTest = rhs.m_isDepthTest;
				m_DepthFunction = rhs.m_DepthFunction;
				m_isCullFace = rhs.m_isCullFace;
				return *this;
			}
		};
	private:
		DrawState m_DrawState;
	public:
		BOHGE_FORCEINLINE const DrawState& GetDrawState()
		{
			return m_DrawState;
		}
		BOHGE_FORCEINLINE void SetDrawState( const DrawState& ds )
		{
			SetCullFace( ds.m_isCullFace );
			if (ds.m_isDepthTest)
			{
				EnableDepthTest();
			}
			else
			{
				DisableDepthTest();
			}
			DepthMask( ds.m_isDepthMask );
			DepthFunction( ds.m_DepthFunction );
		}
		//depth
		void EnableDepthTest();
		void DisableDepthTest();
		void DepthFunction(Function func);
		void DepthMask(bool d);
		//cull faces
		void SetCullFace( const CullFaceMode& mod );
		//alpha blend
		struct AlphaState
		{
			bool m_isAlphaEnable;//是否开启了alpha
			bool m_isAlphaTest;
			Blend m_Src;
			Blend m_Des;
			Blend m_SrcA;
			Blend m_DesA;
			AlphaState()
				:m_isAlphaEnable(false),
				m_isAlphaTest(false),
				m_Src(BL_SRC_ALPHA),
				m_Des(BL_ONE_MINUS_SRC_ALPHA),
				m_SrcA(BL_ONE),
				m_DesA(BL_ONE)
			{

			}
			BOHGE_FORCEINLINE bool operator == ( const AlphaState& rhs) const
			{
				return m_isAlphaEnable == rhs.m_isAlphaEnable
					&& m_isAlphaTest == rhs.m_isAlphaTest
					&& m_Src == rhs.m_Src
					&& m_Des == rhs.m_Des
					&& m_SrcA == rhs.m_SrcA
					&& m_DesA == rhs.m_DesA;
			}
			BOHGE_FORCEINLINE const AlphaState& operator = ( const AlphaState& rhs)
			{
				m_isAlphaEnable = rhs.m_isAlphaEnable;
				m_isAlphaTest = rhs.m_isAlphaTest;
				m_Src = rhs.m_Src;
				m_Des = rhs.m_Des;
				m_SrcA = rhs.m_SrcA;
				m_DesA = rhs.m_DesA;
				return *this;
			}
		};
		AlphaState m_AlphaState;
		void EnableAlphaCoverage( );
		void EnableAlphaBlend(Blend src, Blend des, Blend srcA, Blend desA);
		void EnableAlphaBlend( const AlphaState& as )
		{
			if( as.m_isAlphaEnable )
			{
				if( as.m_isAlphaTest )
				{
					EnableAlphaCoverage();
				}
				else
				{
					EnableAlphaBlend( as.m_Src, as.m_Des, as.m_SrcA, as.m_DesA );
				}
			}
		}
		BOHGE_FORCEINLINE const AlphaState& GetAlphaState( ) const 
		{
			return m_AlphaState;
		}
		void DisableAlpha();
		//stancil
		void EnableStancil();
		void DisableStancil();
		void SetStencilMask(bitmask mask);
		void StencilFunction(Function func, uint ref, bitmask mask);
		void StencilOp(Operation fail, Operation zfail, Operation zpass);
		void StencilOpSeparate(CullFaceMode face, Operation fail, Operation zfail, Operation zpass);
		//color
		friend class Pipeline;
	protected:
		Color		m_vCurrentClearColor;
		Color		m_vUseColor;//用户设置的颜色
		void ClearScreenColor(clamp r, clamp g, clamp b, clamp a);
		void ClearScreenColor()
		{
			ClearScreenColor(m_vUseColor.m_r, m_vUseColor.m_g, m_vUseColor.m_b, m_vUseColor.m_a );
		}
	public:
		void SetClearScreenColor(clamp r, clamp g, clamp b, clamp a)
		{
			m_vUseColor = Color(r,g,b,a);
			ClearScreenColor(m_vUseColor.m_r, m_vUseColor.m_g, m_vUseColor.m_b, m_vUseColor.m_a );
		}
		void SetClearScreenColor(const Color& clr)
		{
			m_vUseColor = clr;
			ClearScreenColor(m_vUseColor.m_r, m_vUseColor.m_g, m_vUseColor.m_b, m_vUseColor.m_a );
		}
		BOHGE_FORCEINLINE const Color& GetClearColor()
		{
			return m_vUseColor;
		}
		void ColorMask(bool r, bool g, bool b, bool a );
		//line
	protected:
		float m_LineWidth;
	public:
		float LineWidth(float s);
		//视口相关
	public:
		enum ViewType
		{
			R_16_9 = 0,//16:9的显示模式
			R_3_2,//iphone
			R_4_3,//平板电脑的4:3
		};
	protected:
		ViewType		m_eViewType;
		vector4d		m_Viewport;
		vector2d		m_Resolution;
	public:
		/*将屏幕坐标转换为相对坐标*/
		BOHGE_FORCEINLINE void SceenToRelative(vector2f& input)
		{
			input.m_x = input.m_x / m_Viewport.m_z * 2.0f - 1.0f;
			input.m_y = input.m_y / m_Viewport.m_w * 2.0f - 1.0f;
		}
		BOHGE_FORCEINLINE const vector2d& GetResolutionSize()
		{
			return m_Resolution;
		}
		BOHGE_FORCEINLINE void SetResolutionSize( const vector2d& size )
		{
			m_Resolution = size;
		}
		BOHGE_FORCEINLINE const vector4d& GetViewprot()
		{
			return m_Viewport;
		}
		BOHGE_FORCEINLINE ViewType GetViewType()
		{
			return m_eViewType;
		}
		void Clear(bitmask mask);
		void SetViewport(const vector4d& vp);	//偏移与尺寸


		//shader相关
	protected:
		handle		m_hCurrentShader;//判断是否变换shader
		uint		m_uTextrueReferenceCount;//纹理的索引
	public:
		void DeleteProgram(handle h);
		bool CreateShaderProgram(handle& hProgram, Attributes& attr, int shaderindex );
		bool CreateShaderProgram(handle& hProgram, Attributes& attr, const std::string& vscode, const std::string& pscode );
		handle GetShaderParameterHandle(handle hPragma, const char* name);
		void BindShader(handle h);
		/*shader可能会绑定多个纹理此处清除绑定纹理的递增计数*/
		void CleanTextrueReference();
		void SetShaderParameter(handle h, const std::vector<Quaternionf>& input);
		void SetShaderParameter(handle h, const int input);
		void SetShaderParameter(handle h, const float input);
		void SetShaderParameter(handle h, const std::vector<float>& input);
		void SetShaderParameter(handle h, const vector2f& input);
		void SetShaderParameter(handle h, const vector3f& input);
		void SetShaderParameter(handle h, const vector4f& input);
		void SetShaderParameter(handle h, const Matrix33f& input);
		void SetShaderParameter(handle h, const Matrix44f& input);
		void SetShaderParameter(handle h, const Texture& input);


		//纹理相关
	public:
		enum PixelFormat
		{
			PF_A8 = 0,
			PF_L8,
			PF_R8G8B8,//8_8_8
			PF_R5G6B5,//5_6_5
			PF_R8G8B8A8,//8_8_8_8
			PF_R4G4B4A4,//4_4_4_4
			PF_DEPTH16,
			PF_DEPTH24_STENCIL8,
			//PF_LA8
		};
		enum TextureType
		{
			TT_TEXTURE1D = 0,
			TT_TEXTURE2D,
			TT_TEXTURE3D,
			TT_TEXTURECUBE,
		};
		enum TextureWarp
		{
			TW_REPEAT = 0,
			TW_CLAMP_TO_EDGE,
			TW_MIRRORED_REPEAT,
		};
		enum TextureFilter
		{
			TF_NEAREST = 0,
			TF_LINEAR,
			TF_NEAREST_MIPMAP_NEAREST,
			TF_LINEAR_MIPMAP_NEAREST,
			TF_NEAREST_MIPMAP_LINEAR,
			TF_LINEAR_MIPMAP_LINEAR,
		};
	protected:
		bool					m_isUsingAnisotropic;
		float					m_fMaxAnisotropic;	//最大支持各向异性
		handle					m_hCurrentTexture;
	public:
		bool LoadTextureFromFile( PixelFormat pf, TextureData& out, const std::string& strFileName);
		bool LoadCubeTextureFromFile( PixelFormat pf, TextureData& out, const std::string& f, const std::string& b, const std::string& u, const std::string& d, const std::string& l, const std::string& r  );
		bool MakeTextureFromStream(handle& h, const TextureData& input, const byte* buffer);
		bool SubTextureFromStream(handle& h, const TextureData& input, const byte* buffer);
		void DeleteTexture(handle& h);
		void SetTexture(const Texture& tex);
		void SetTextureWarp(const handle& h, TextureType type, TextureWarp s, TextureWarp t);
		void SetTextureFilter(const handle& h, TextureType type, TextureFilter mag, TextureFilter min);
		uint SetTextureAnisotropic( const handle& h, TextureType type, uint x );
		void GenerateMipmap( const handle& h, TextureType type);
		//FBO
		enum TargetType	//fbo的类型
		{
			FBO_EMPTY = 0,		//没有初始化的
			FBO_TCOLOR,	//只有色彩纹理
			FBO_TDEPTH,	//深度纹理
			FBO_TCOLOR_TDEPTH,	//色彩与深度纹理
			FBO_TCOLOR_BDEPTH,	//色彩纹理,深度buffer
			FBO_TDEPTHSTENCIL,	//深度，模板纹理
			FBO_TCOLOR_BDEPTHSTENCIL,	//色彩纹理，深度，模板 buffer
			FBO_TCOLOR_TDEPTHSTENCIL,	//色彩纹理，深度，模板纹理
			FBO_CCOLOR_BDEPTH,	//cube map 深度，专门为point light做阴影用的
		};
	protected:
		bool				m_isPacked_Depth_Stencil;
		vector4d			m_vOldViewPort;
		RenderTarget*		m_pCurrentFBO;//当前的fbo
	protected:
		friend class RenderTarget;
		//RenderTarget GetBindRenderTarget( );
		bool CreateRenderTarget(  RenderTarget& rt );
		void DestroyRenderTarget( RenderTarget& rt );
	public:
		//push和pop需要支持处理NULL指针的跳过，引擎被设计为NULL指针就画在屏幕上
		void PushRenderTarget( RenderTarget* const rt );
		RenderTarget* PopRenderTarget( );
		void CopyFragment( PixelFormat outformat, std::vector<byte>& out );//慎用，从当前FBO拷贝纹理，效率低下
		//void CopyFragment( Texture2D& tex, const RenderTarget* from );//拷贝当前FBO中的像素到纹理
		BOHGE_FORCEINLINE bool isPackedDepthStencil()
		{
			return m_isPacked_Depth_Stencil;
		}
		BOHGE_FORCEINLINE RenderTarget* GetCurrentRenderTarget()
		{
			return m_pCurrentFBO;
		}

		//VBO
	protected:
		handle		m_hCurrentBuffer;//判断是否变换buffer
	public:
		enum BufferType
		{
			VERTEX_BUFFER = 0,
			INDIES_BUFFER,
		};
		enum MemoryUseage
		{
			MU_DYNAMIC = 0,
			MU_STATIC,
		};
		void CreateBuffer(handle& h, BufferType type, uint totleSize, const void* dataptr, MemoryUseage use);
		void ReallocBuffer(handle& h, BufferType type, uint totleSize, const void* dataptr, MemoryUseage use);//重新设定一个buffer
		void ChangeData(handle h, BufferType type, uint offset, uint biteSize, const void* dataptr );
		void BindBuffer(handle h, BufferType type);
		void DeleteBuffer(handle&h);
		//绘制
#ifdef _DEBUG
	protected:
		uint		m_AlphaChange;
		uint		m_DrawStateChange;
		uint		m_DrawCall;
		uint		m_DrawPrimitive;
#endif
	public:
		void UsingVertexAttribPointer(handle h, uint size, bool normalize, uint stribie, const FloatVector* vector);
		void DropVertexAttribPointer(handle h);
		//virtual void Draw(VertexBuffer& buff, Shader& shader, Texture* tex);
		void Draw(const RendBuffer& buff, Shader& shader, const Texture* tex);
		void Draw(const RendBuffer& buff, const Material& material);
		void Finish();
#ifdef _DEBUG
	private:
		void _DebugLog();
#endif
	public:
		Device()
			:m_mRenderState(0),
			m_isUsingAnisotropic(false),
			m_uTextrueReferenceCount(0),
			m_hCurrentShader(-1),
			m_hCurrentTexture(-1),
			m_pCurrentFBO(NULL),
			m_hCurrentBuffer(-1)
#ifdef _DEBUG
			,
			m_AlphaChange(0),
			m_DrawStateChange(0),
			m_DrawCall(0),
			m_DrawPrimitive(0)
#endif
		{}
		void OnDeviceCreate();
		void OnDeviceDestroy();
		BOHGE_FORCEINLINE void Update()
		{
#ifdef _DEBUG
			_DebugLog();
			m_AlphaChange = 0;
			m_DrawStateChange = 0;
			m_DrawCall = 0;
			m_DrawPrimitive = 0;
#endif
			m_hCurrentTexture = -1;
			m_hCurrentShader = -1;
			m_pCurrentFBO = NULL;
		}
	};
}