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



#include "parameterhandler.h"
#include <stdio.h>


namespace BohgeEngine
{
#define f_step		 0.0001f
#define f_stepXL	 0.001f

	parameterhandler::parameterhandler()
	{
		id = 0;
		add_parameter(p_fStrength,		"Wave strength",			dtFloat);
		add_parameter(p_bRefrStrength,	"refrcation strength",		dtFloat);
		add_parameter(p_fNoiseheight,	"Make noise height",		dtFloat);
		add_parameter(p_iOctaves,		"Octaves",					dtInt);
		add_parameter(p_fScale,			"Wave scale",				dtFloat);
		add_parameter(p_fFalloff,		"Wave falloff",			dtFloat);
		add_parameter(p_fAnimspeed,		"Animation speed",			dtFloat);
		add_parameter(p_fTimemulti,		"Animation multi",			dtFloat);
		add_parameter(p_fElevation,		"Projector elevation",		dtFloat);
		add_parameter(p_fWaterBaseHeight,"Water base height",		dtFloat);
		add_parameter(p_fCanSeeDepth,	"Water can see depth",		dtFloat);
		add_parameter(p_fNoiseTile,		"Noise textrue repeat",		dtFloat);
		add_parameter(p_fColorChangeDepthInv,	"Color Change invers",		dtFloat);
		add_parameter(p_fNoiseStrength,	"Noise strength",		dtFloat);
		add_parameter(p_fNoiseWaveSpeed,"Noise wave speed",		dtFloat);
		add_parameter(p_fBumpScale,		"scale",		dtFloat);
		add_parameter(p_fBumpFarPer,	"far per",		dtFloat);
		add_parameter(p_fBumpStrength,	"strength",		dtFloat);
		add_parameter(p_fReserved1,		"p_fReserved1",		dtFloat);
		add_parameter(p_fReserved2,		"p_fReserved2",		dtFloat);
		add_parameter(p_fReserved3,		"p_fReserved3",		dtFloat);
		add_parameter(p_fReserved4,		"p_fReserved4",		dtFloat);
		add_parameter(p_fReserved5,		"p_fReserved5",		dtFloat);
		add_parameter(p_fReserved6,		"p_fReserved6",		dtFloat);
		add_parameter(p_fReserved7,		"p_fReserved7",		dtFloat);
		add_parameter(p_fReserved8,		"p_fReserved8",		dtFloat);
		add_parameter(p_fReserved9,		"p_fReserved9",		dtFloat);
		add_parameter(p_fReserved10,	"p_fReserved10",	dtFloat);
		add_parameter(p_fReserved11,	"p_fReserved11",	dtFloat);
		add_parameter(p_fReserved12,	"p_fReserved12",	dtFloat);
		add_parameter(p_fReserved13,	"p_fReserved13",	dtFloat);
		add_parameter(p_fReserved14,	"p_fReserved14",	dtFloat);
		add_parameter(p_fReserved15,	"p_fReserved15",	dtFloat);
		add_parameter(p_fReserved16,	"p_fReserved16",	dtFloat);
	}

	parameterhandler::~parameterhandler()
	{

	}

	bool parameterhandler::add_parameter(unsigned int id, const char* name, unsigned int datatype)
	{
		sprintf ( params[id].name, "%s", name);
		params[id].datatype = datatype;
		return true;
	}

	void parameterhandler::value_increase()
	{
		switch(params[id].datatype)
		{
		case dtBool:
			{
				params[id].bData = !params[id].bData;
				break;
			}
		case dtInt:
			{
				params[id].iData += 1;
				break;
			}
		case dtFloat:
			{
				params[id].fData += f_step;
				break;
			}
		};

	}

	void parameterhandler::value_decrease()
	{
		switch(params[id].datatype)
		{
		case dtBool:
			{
				params[id].bData = !params[id].bData;
				break;
			}
		case dtInt:
			{
				params[id].iData -= 1;
				break;
			}
		case dtFloat:
			{
				params[id].fData -= f_step;
				break;
			}
		};
	}

	void parameterhandler::value_increaseXL()
	{
		switch(params[id].datatype)
		{
		case dtBool:
			{
				params[id].bData = !params[id].bData;
				break;
			}
		case dtInt:
			{
				params[id].iData += 10;
				break;
			}
		case dtFloat:
			{
				params[id].fData *= 1.05;
				break;
			}
		};

	}

	void parameterhandler::value_decreaseXL()
	{
		switch(params[id].datatype)
		{
		case dtBool:
			{
				params[id].bData = !params[id].bData;
				break;
			}
		case dtInt:
			{
				params[id].iData -= 10;
				break;
			}
		case dtFloat:
			{
				params[id].fData *= 0.95;
				break;
			}
		};
	}

	void parameterhandler::value_reset()
	{
		switch(params[id].datatype)
		{
		case dtBool:
			{
				//params[id].bData = !params[id].bData;
				break;
			}
		case dtInt:
			{
				//params[id].iData -= 1;
				break;
			}
		case dtFloat:
			{
				params[id].fData = 0;
				break;
			}
		};
	}

	void parameterhandler::_Docopy( const parameterhandler& rhs )
	{
		for( int i = 0 ; i < numParameters ; i ++ )
		{
			params[i].Docopy( rhs.params[i] );
		}
	}

}