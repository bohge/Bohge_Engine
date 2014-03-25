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



/////////////////////////////////////////
//			State Manage class		   //
/////////////////////////////////////////
#include "FilePath.h"
#include "ActionManager.h"
#include "StateMainMenu.h"
#include "StateManager.h"
#include "Device.h"
#include "UI.h"
#include "Bfont.h"
#include "Particle.h"
#include "ShaderManager.h"
#include "Texture.h"
#include "Log.h"
#include "Emitter.h"
#include "UIManager.h"
#include "Camera.h"
#include "SkyBox.h"
#include "SceneManager.h"
#include "SaveHelper.h"
#include "GameResourceList.h"
#include "MessageBox.h"
#include "Language.h"
#include "GameSetting.h"
#include "UIAnimation.h"
#include "Material.h"
#include "Game.h"
#include "AdHelper.h"
#include "NetHost.h"
#include "SkyBox.h"
#include "BloomPS.h"
#include "HDRPostEffect.h"
#include "DOFPostEffect.h"
#include "SSAOPostEffect.h"
#include "FXAAPostEffect.h"
#include "SceneMain.h"
#include "ScriptBase.h"
#include "MaterialManager.h"

#include "SoundManager.h"
#include "SoundPlayer.h"

#include <string>


#include "Model.h"
#include "Pipeline.h"

using namespace std;
using namespace BohgeEngine;

LocatableModel* lm;
LocatableModel* lm2;
LocatableModel* lm3;
LocatableModel* lm4;
LocatableModel* lm5;
LocatableModel* lm6;
LocatableModel* lm7;
LocatableModel* lm8;
LocatableModel* lm9;
LocatableModel* lm0;

#define dis (200)

#include "ScriptEmitter.h"
ScriptEmitter* e;

#include "FakeLights.h"
#include "LightMaped.h"
#include "RealLights.h"
#include "ShadowCaster.h"
RealLightNode* reallight;
FakeLightNode* light;
LightMapedNode* light2;

#include "Bfont.h"
BString* str;

#include "ObjectGroup.h"
ObjectGroup* objgro;

using namespace std;

namespace BohgeGame
{

	//-------------------------------------------------------------------------------------------------------
	void StateMainMenu::LoadResource(Engine& engine)
	{
		//m_Bloom = NEW BloomPS;
		m_Bloom = NEW HDRPostEffect;
		//m_Bloom = (HDRPostEffect*)malloc( sizeof(HDRPostEffect) );//operator newºÍnew operator
		//m_Bloom = new (m_Bloom) HDRPostEffect;
		m_DOF = NEW DOFPostEffect;
		m_SSAO = NEW SSAOPostEffect;
		m_FXAA = NEW FXAAPostEffcet;

		engine.GetPipeline()->AddPostEffcet( m_Bloom );
		engine.GetPipeline()->AddPostEffcet( m_DOF );
		//engine.GetPipeline()->AddPostEffcet( m_SSAO );
		//engine.GetPipeline()->AddPostEffcet( m_FXAA );

		//engine.GetCamera()->CreatePerspectiveProjection( 0.1, 500 );
		engine.GetCamera()->CreatePerspectiveProjection( 1, 3000 );
		//engine.GetCamera()->LookAt( vector3f(-603.74371, 386.8570571, -1054.0507),
		//	vector3f(-604.68408, 7.1759810, -1053.9323),
		//	vector3f(0, 1, 0));
		engine.GetCamera()->LookAt( vector3f(-603.74371, 6.8570571, -1054.0507),
			vector3f(-604.68408, 7.1759810, -1053.9323),
			vector3f(0, 1, 0));
		//engine.GetCamera()->LookAt( vector3f(0, 10, 0),
		//	vector3f(0, 0, -1),
		//	vector3f(0, 1, 0));
		engine.GetDevice()->SetClearScreenColor( 0.1176f, 0.5294f, 0.9020f, 1.0f );


		//light = NEW FakePointLight;
		//light->SetColor( vector3f(1.0, 0.2, 0.3) );
		//light->SetLocalPosition( vector3f( -600,20,-1000 ) );
		//light->SetRange( 30 );

		//str = Engine::Instance().GetStringManager()->MakeString( vector2f(0.1,0.15), Color::White, std::wstring(L"123123") );
		//str->SetPosition( vector2f(0,0), BString::RightTop );

		e = new ScriptEmitter(
			vector3f(  -700,0,-1000 ),
			vector3f( 5,0,5 ),
			vector3f( 0,1,0 ),
			vector3f( 0,0.00015,0 ),
			vector2f( 0.01, 0.02 ),
			0.0,
			vector3f( PI/2,PI,PI/2 ),
			Color(0.95,0.5,0.3,1.0),
			Color(1.0,0.55,0.35,1.0),
			Color(0.1,0.1,0.1,1.0),
			Color(0.2,0.1,0.1,1.0),
			vector2f( 0.4, 1.0),
			vector2d( SEC(0.8),SEC(1.2) ),
			vector2f( 4, 16 ),
			vector2f( 8,12),
			vector2f( 30,40),
			vector2f( 2, 4),
			vector2d( 100, 200),
			-1,
			FILEPATH.TextureFolder() + "Smoke.png");
		e->CaculateBindBox();
		e->LoadScript( FILEPATH.ScriptFolder() + "Test.lua" );

		//engine.GetSceneManage()->LoadScene( "light.sfd" );
		//engine.GetSceneManage()->LoadScene( "tt.sfd" );
		//engine.GetSceneManage()->LoadScene( "GIS.sfd" );
		//engine.GetSceneManage()->LoadScene( "proj.sfd" );
		engine.GetSceneManager()->LoadScene( "Demo.sfd" );
		//engine.GetSceneManage()->LoadScene( "demo_island.sfd" );
		//engine.GetSceneManage()->LoadScene( "Test.sfd" );
		engine.GetSceneManager()->GetSkyBox().AndSkyBox( SkyBox::BACKGROUND, "fl.png", "bl.png", "ul.png", "dl.png", "ll.png", "rl.png" );
		lm = new LocatableModel();
		lm->LoadModel( FILEPATH.ModelFolder() + "Fox" );
		//lm->InitPhysical( 10, vector3f(0,0,0) );
		lm->SetLocalScale( vector3f( 5,5,5 ) );
		//lm->SetLocalScale( vector3f( 50,50,50 ) );
		//lm->SetScale( vector3f( 200,200,200 ) );
		lm->SetLocalPosition( vector3f( -720,3,-1050 ) );
		//lm->SetLocalPosition( vector3f( -220,50,-150 ) );
		lm->AnimationPlay();
		lm->AnimationLoop(true);
		lm->SetStatus( ISceneNode::RS_SHADOW );

		//const Material* mat = engine.GetMaterialManager()->GetMaterial( "Skeleton2BDirectionLight" );
		//lm->GetRendMeshVector()[0].m_pMaterial->SetShader( Pipeline::PT_LIGHTING, mat->ShaderIndex( Pipeline::PT_LIGHTING ) );
		//lm->GetRendMeshVector()[0].m_pMaterial->SetShader( Pipeline::PT_DEPTH, mat->ShaderIndex( Pipeline::PT_DEPTH ) );
		//lm->GetRendMeshVector()[0].m_pMaterial->SetShader( Pipeline::PT_NORMAL, mat->ShaderIndex( Pipeline::PT_NORMAL ) );
		//lm->GetRendMeshVector()[1].m_pMaterial->SetShader( Pipeline::PT_LIGHTING, mat->ShaderIndex( Pipeline::PT_LIGHTING ) );
		//lm->GetRendMeshVector()[1].m_pMaterial->SetShader( Pipeline::PT_DEPTH, mat->ShaderIndex( Pipeline::PT_DEPTH ) );
		//lm->GetRendMeshVector()[1].m_pMaterial->SetShader( Pipeline::PT_NORMAL, mat->ShaderIndex( Pipeline::PT_NORMAL ) );

		//light2 = NEW LightMapedPoint;
		//light2->SetColor( vector3f(1.0, 1.0, 0.3) );
		//light2->SetLocalPosition( vector3f( -0.04, 0.0, 0.0 ) );
		//light2->SetRange( 0.05 );

		////lm3 = new LocatableModel();
		////lm3->LoadModel( FILEPATH.ModelFolder() + "cone" );
		////lm3->SetLocalScale( vector3f(5,5,5) );
		////lm3->SetLocalRotation( Math::AxisToAxis( FORWARD, vector3f(0,1,0) ) );
		////lm3->SetLocalPosition( vector3f( 0,0.01,0 ) );

		//lm2 = new LocatableModel();
		//lm2->LoadModel( FILEPATH.ModelFolder() + "Chameleon" );
		////lm2->InitPhysical( 10, vector3f(0,0,0) );
		//lm2->SetLocalScale( vector3f( 200,200,200 ) );
		//lm2->SetLocalPosition( vector3f( -650,5,-1030 ) );
		//lm2->AnimationPlay();
		//lm2->AnimationLoop(true);
		//lm2->SetStatus( ISceneNode::RS_SHADOW );
		//lm2->GetNodeByIndex( 11 )->AttachNode( light2 );
		////lm2->GetJointNode( 11 )->AttachNode( lm3 );

		////lm5 = new LocatableModel;
		////lm5->LoadModel( FILEPATH.ModelFolder() + "Cube" );
		////lm5->SetLocalPosition( vector3f( -650,2,-1080 ) );
		////lm5->SetLocalScale(vector3f(0.1,0.1,0.1) );
		////lm5->SetStatus( ISceneNode::RS_SHADOW );

		//{//²âÊÔRenderTargetCube

		//	//lm = new LocatableModel;
		//	//lm->LoadModel( FILEPATH.ModelFolder() + "Fox" );
		//	//lm->SetLocalPosition( vector3f(40,-10,0) );
		//	//lm->SetStatus( ISceneNode::RS_SHADOW );
		//	//lm->SetLocalScale( vector3f(2,2,2) );
		//	//lm->AnimationPlay();

		//	//lm4 = new LocatableModel;
		//	//lm4->LoadModel( FILEPATH.ModelFolder() + "Fox" );
		//	//lm4->SetLocalPosition( vector3f(-40,-10,0) );
		//	//lm4->SetStatus( ISceneNode::RS_SHADOW );
		//	//lm4->SetLocalScale( vector3f(2,2,2) );
		//	//lm4->AnimationPlay();

		//	//lm2 = new LocatableModel;
		//	//lm2->LoadModel( FILEPATH.ModelFolder() + "Fox" );
		//	//lm2->SetLocalPosition( vector3f(0,-10,40) );
		//	//lm2->SetStatus( ISceneNode::RS_SHADOW );
		//	//lm2->SetLocalScale( vector3f(2,2,2) );
		//	//lm2->AnimationPlay();

		//	//lm4 = new LocatableModel;
		//	//lm4->LoadModel( FILEPATH.ModelFolder() + "Fox" );
		//	//lm4->SetLocalPosition( vector3f(0,-10,-40) );
		//	//lm4->SetStatus( ISceneNode::RS_SHADOW );
		//	//lm4->SetLocalScale( vector3f(2,2,2) );
		//	//lm4->AnimationPlay();

		//	//lm3 = new LocatableModel;
		//	//lm3->LoadModel( FILEPATH.ModelFolder() + "Fox" );
		//	//lm3->SetLocalPosition( vector3f(0,30,0) );
		//	//lm3->SetStatus( ISceneNode::RS_SHADOW );
		//	//lm3->SetLocalScale( vector3f(2,2,2) );
		//	//lm3->AnimationPlay();

		//	lm3 = new LocatableModel;
		//	lm3->LoadModel( FILEPATH.ModelFolder() + "Cube" );
		//	lm3->GetRendMeshVector()[0].m_pMaterial->SetShader( Pipeline::PT_LIGHTING, ShaderManage::ModelSpotLightPerpixel );
		//	lm3->SetLocalPosition( vector3f(0,25,5) );
		//	lm3->SetLocalScale( vector3f(0.2,0.2,0.2) );
		//	lm3->SetStatus( ISceneNode::RS_SHADOW );

		//	lm3 = new LocatableModel;
		//	lm3->LoadModel( FILEPATH.ModelFolder() + "Cube" );
		//	lm3->GetRendMeshVector()[0].m_pMaterial->SetShader( Pipeline::PT_LIGHTING, ShaderManage::ModelSpotLightPerpixel );
		//	lm3->SetLocalPosition( vector3f(0,1,-25) );
		//	lm3->SetLocalScale( vector3f(0.2,0.2,0.2) );
		//	lm3->SetStatus( ISceneNode::RS_SHADOW );

		//	lm3 = new LocatableModel;
		//	lm3->LoadModel( FILEPATH.ModelFolder() + "Cube" );
		//	lm3->GetRendMeshVector()[0].m_pMaterial->SetShader( Pipeline::PT_LIGHTING, ShaderManage::ModelSpotLightPerpixel );
		//	lm3->SetLocalPosition( vector3f(0,0,0) );
		//	lm3->SetStatus( ISceneNode::RS_SHADOW );
		//	//lm3->SetLocalScale( vector3f(0.1,0.1,0.1) );
		//	//lm3->AnimationPlay();

		//	lm5 = new LocatableModel;
		//	lm5->LoadModel( FILEPATH.ModelFolder() + "Cube" );
		//	lm5->GetRendMeshVector()[0].m_pMaterial->SetShader( Pipeline::PT_LIGHTING, ShaderManage::ModelSpotLightPerpixel );
		//	lm5->SetLocalPosition( vector3f(0,-100,0) );
		//	lm5->SetLocalScale(vector3f(50,5,50) );

		//	reallight = NEW RealSpotLight;
		//	reallight->SetColor( vector3f(1,1,1) );
		//	reallight->SetLightAngle( vector2f( PI*0.1, PI*0.3 ) );
		//	reallight->SetLocalDirection( vector3f( 0, -1, -1 ) );
		//	reallight->SetLocalPosition( vector3f( 0, 100, 50) );
		//	reallight->SetRange( 500 );
		//	reallight->SetAttenuation( 1, 0.0, 0.0, 0 );
		//	reallight->SetAttribute( LightNode::LA_SHADOW );
		//}

		//ÉùÒô²âÊÔ
		SoundPlayer* s1 = engine.GetSoundManager()->LoadSound( FILEPATH.SoundFolder() + "Button.ogg" );
		//SoundPlayer* s1 = engine.GetSoundManager()->LoadSound( FILEPATH.SoundFolder() + "backGroundMusic.ogg" );
		s1->SetVolume( 1.0 );
		s1->SetLoop( true );
		s1->Paly();
		SoundPlayer* s2 = engine.GetSoundManager()->LoadSound( FILEPATH.SoundFolder() + "Button.ogg" );
		s2->SetVolume( 2.0 );
		s2->SetLoop( true );
		s2->Paly();
		SoundPlayer* s3 = engine.GetSoundManager()->LoadSound( FILEPATH.SoundFolder() + "Slider.ogg" );
		s3->SetVolume( 3.0 );
		s3->SetLoop( true );
		s3->Paly();
		//SoundPlayer* s4 = engine.GetSoundManager()->LoadSound( FILEPATH.SoundFolder() + "Button.ogg" );
		//SoundPlayer* s5 = engine.GetSoundManager()->LoadSound( FILEPATH.SoundFolder() + "Button.ogg" );

		//engine.GetSoundManager()->ReleaseSound( s1 );
		//engine.GetSoundManager()->ReleaseSound( s2 );
		//engine.GetSoundManager()->ReleaseSound( s3 );
		//engine.GetSoundManager()->ReleaseSound( s4 );
		//engine.GetSoundManager()->ReleaseSound( s5 );
	}
	//-------------------------------------------------------------------------------------------------------
	void StateMainMenu::ReleaseResource(Engine& engine)
	{
		SAFE_DELETE( m_Bloom );
		SAFE_DELETE( m_DOF );
		SAFE_DELETE( m_FXAA );
		SAFE_DELETE( lm2 );
		SAFE_DELETE( light );
		SAFE_DELETE( light2 );
		//SAFE_DELETE( lm3 );
		//SAFE_DELETE( lm4 );
		//SAFE_DELETE( lm5 );
		//SAFE_DELETE( lm6 );
		//SAFE_DELETE( lm7 );
		//SAFE_DELETE( lm8 );
		//SAFE_DELETE( lm9 );
		//SAFE_DELETE( lm0 );
		SAFE_DELETE( lm );
		SAFE_DELETE( e );
	}
	//-------------------------------------------------------------------
	void StateMainMenu::OnEnter(Engine& engine, StateManager::GameState Previous)
	{

	}
	//-------------------------------------------------------------------
	void StateMainMenu::OnLeave(Engine& engine)
	{

	}
	//-------------------------------------------------------------------
	void StateMainMenu::Update(StateManager& state, Engine& engine, uint millisecond)
	{
		//vector3f pos = engine.GetCamera()->GetPosition() + engine.GetCamera()->GetForward() * 80;
		//pos.m_y = engine.GetSceneManage()->GetWorld().GetHeight( pos.m_x, pos.m_z );
		//lm2->SetLocalPosition( pos );
		//static float size = 1;
		//size += 0.5;
		//if ( size > 200 )
		//{
		//	size = 1;
		//}
		//lm2->SetLocalScale( vector3f(size, size, size) );
	}
}