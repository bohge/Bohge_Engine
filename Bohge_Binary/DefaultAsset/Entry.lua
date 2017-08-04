LUA_CLASS_NAME = String("Entry");

Entry = {};
Entry.Camera = 0;
Entry.Bloom = 0;
Entry.Test_UI = 0;
Entry.Test_Particle = 0;

function Entry.Initialize( )
	Entry.Bloom = PostEffect();
	Entry.Bloom:CreatePostEffect( String("root:DefaultAsset/PostEffect/Bloom.lua") );
	Entry.Camera = StaticCamera();
	Entry.Camera:CreatePerspectiveProjection( Float(1), Float(100) );
	Entry.Camera:LookAt(
		vector3f(Float(0),Float(0),Float(10)),
		vector3f(Float(0),Float(0),Float(0)),
		vector3f(Float(0),Float(1),Float(0)) );
	Entry.Camera:AttachCamera();
	
	--初始化UI
	Entry.Test_UI = UIEntity();
	Entry.Test_UI:PushMetadata( PathMetadata( String( "root:/DefaultAsset/UserInterface/Test.windows" ) ) );
	Entry.Test_UI:CreateResource();
	
	--初始化粒子
	--ParticleEntity.GetTypeName();
	Entry.Test_Particle = ParticleEntity();
	Entry.Test_Particle:PushMetadata( PathMetadata( String( "root:DefaultAsset/Test.Emitter" ) ) );
	Entry.Test_Particle:CreateResource();
end


function Entry.BeginUpdate( )

end


function Entry.DoUpdate( )
	--DebugLog:Print( String("Entry.DoUpdate and time is %f s\r\n"), Timespan );
end


function Entry.EndUpdate( )

end


function Entry.Exit( )

end


