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



#ifndef _parameterhandler_
#define _parameterhandler_
#include "Predefine.h"
#include <string.h>


namespace BohgeEngine
{
	enum dtDatatypes
	{
		dtBool=0,
		dtFloat,
		dtInt
	};

	enum pParameters
	{
		p_fStrength=0,
		p_fNoiseheight,//产生早点的高度
		p_bRefrStrength,
		p_fFalloff,	
		p_fScale,
		p_iOctaves,	
		p_fAnimspeed,	
		p_fTimemulti,
		p_fElevation,
		p_fWaterBaseHeight,
		p_fCanSeeDepth,
		p_fNoiseTile,
		p_fColorChangeDepthInv,
		p_fNoiseStrength,
		p_fNoiseWaveSpeed,
		p_fBumpScale,
		p_fBumpFarPer,
		p_fBumpStrength,
		p_fReserved1,
		p_fReserved2,
		p_fReserved3,
		p_fReserved4,
		p_fReserved5,
		p_fReserved6,
		p_fReserved7,
		p_fReserved8,
		p_fReserved9,
		p_fReserved10,
		p_fReserved11,
		p_fReserved12,
		p_fReserved13,
		p_fReserved14,
		p_fReserved15,
		p_fReserved16,
		numParameters
	};

	struct parameter
	{
		char name[128];
		unsigned int datatype;
		union{
			int		iData;
			bool	bData;
			float	fData;
		};
		void Docopy( const parameter& rhs )
		{
			memcpy( name, rhs.name, 32 );
			datatype = rhs.datatype;
			iData = rhs.iData;
		}
	};

	class parameterhandler
	{
	private:
		void _Docopy( const parameterhandler& rhs );
	public:
		parameterhandler();
		~parameterhandler();
		parameterhandler( const parameterhandler& rhs )
		{
			_Docopy( rhs );
		}
		BOHGE_FORCEINLINE parameterhandler& operator = ( const parameterhandler& rhs )
		{
			_Docopy( rhs );
			return *this;
		}
		bool add_parameter(unsigned int id, const char* name, unsigned int datatype);
		void next_parameter(){ id = (id+1)%numParameters; }
		void previous_parameter(){ id = (id-1+numParameters)%numParameters; }

		float get_float(pParameters id){ return this->params[id].fData; }
		int get_int(pParameters id){ return this->params[id].iData; }
		bool get_bool(pParameters id){ return this->params[id].bData; }
		void set_float(pParameters id, float value){ params[id].fData = value; }
		void set_int(pParameters id, int value){ params[id].iData = value; }
		void set_bool(pParameters id, bool value){ params[id].bData = value; }

		void value_increase();
		void value_decrease();
		void value_increaseXL();
		void value_decreaseXL();
		void value_reset();
		parameter params[numParameters];
	private:
		unsigned int id;
	};

}

#endif