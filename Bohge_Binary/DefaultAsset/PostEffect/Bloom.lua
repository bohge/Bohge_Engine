LUA_CLASS_NAME = "Bloom";

Bloom = {}
Bloom.RoLumi = 0
Bloom.RoGuassX = 0
Bloom.RoGuassY = 0
Bloom.RoCombine = 0
Bloom.Rt1 = 0;
Bloom.Rt2 = 0;

Bloom.Queue = 500;

function Bloom.Initialize( host, size )
	targetsize = size / 4;
	GuassStep = vector2( 2.0, 2.0 ) / targetsize;
	Bloom.RoLumi = host:CreateRenderObject( "root:DefaultAsset/Material/Luminance.Material" );
	Bloom.RoLumi:SetUserParameters( MaterialProperty.ATT_MATERIAL_USE_0, MaterialProperty.UPT_VEC1, vector1( 1.0 ) );
	
	Bloom.RoGuassX = host:CreateRenderObject( "root:DefaultAsset/Material/Gaussian.Material" );
	Bloom.RoGuassX:SetUserParameters( MaterialProperty.ATT_MATERIAL_USE_0, MaterialProperty.UPT_VEC2, vector2( GuassStep:x(), 0 ) );
	
	Bloom.RoGuassY = host:CreateRenderObject( "root:DefaultAsset/Material/Gaussian.Material" );
	Bloom.RoGuassY:SetUserParameters( MaterialProperty.ATT_MATERIAL_USE_0, MaterialProperty.UPT_VEC2, vector2( 0, GuassStep:y() ) );
	
	Bloom.RoCombine = host:CreateRenderObject( "root:DefaultAsset/Material/Combine.Material" );
	Bloom.RoCombine:SetUserParameters( MaterialProperty.ATT_MATERIAL_USE_0, MaterialProperty.UPT_VEC2, vector2( 1, 2 ) );
	
	Bloom.Rt1 = host:CreateRenderTarget( RenderTargetProperty.ST_SWAP_A, targetsize );
	Bloom.Rt2 = host:CreateRenderTarget( RenderTargetProperty.ST_SWAP_B, targetsize );

end

function Bloom.OnResizeview( size )
	targetsize = size / 4;
	GuassStep = vector2( 2.0, 2.0 ) / targetsize;

	Bloom.RoGuassX:SetUserParameters( MaterialProperty.ATT_MATERIAL_USE_0, MaterialProperty.UPT_VEC2, vector2( GuassStep:x(), 0 ) );
	Bloom.RoGuassY:SetUserParameters( MaterialProperty.ATT_MATERIAL_USE_0, MaterialProperty.UPT_VEC2, vector2( 0, GuassStep:y() ) );
end


function Bloom.ProcessPostEffect( Original, Scene, Output )
	
	Bloom.Rt1:PushRenderTarget();
	Bloom.Rt1:ClearBuffer( RenderTargetProperty.CF_COLOR );
	Bloom.RoLumi:SetTextureEntity(
		MaterialProperty.ATT_TEXTURE_DIFFUSE,
		Scene:GetAttachment( RenderTargetProperty.TA_COLOR_0 ) );
	Bloom.RoLumi:Draw( PipelineProperty.PT_UNIVERSAL_POST_EFFECT );
		
	Bloom.Rt2:PushRenderTarget();
	Bloom.Rt2:ClearBuffer( RenderTargetProperty.CF_COLOR );
	Bloom.RoGuassX:SetTextureEntity(
		MaterialProperty.ATT_TEXTURE_DIFFUSE,
		Bloom.Rt1:GetAttachment( RenderTargetProperty.TA_COLOR_0 ) );
	Bloom.RoGuassX:Draw( PipelineProperty.PT_UNIVERSAL_POST_EFFECT );
		
	Bloom.Rt1:PushRenderTarget();
	Bloom.Rt1:ClearBuffer( RenderTargetProperty.CF_COLOR );
	Bloom.RoGuassY:SetTextureEntity(
		MaterialProperty.ATT_TEXTURE_DIFFUSE,
		Bloom.Rt2:GetAttachment( RenderTargetProperty.TA_COLOR_0 ) );
	Bloom.RoGuassY:Draw( PipelineProperty.PT_UNIVERSAL_POST_EFFECT );
	
	Bloom.Rt2:PushRenderTarget();
	Bloom.Rt2:ClearBuffer( RenderTargetProperty.CF_COLOR );
	Bloom.RoGuassX:SetTextureEntity(
		MaterialProperty.ATT_TEXTURE_DIFFUSE,
		Bloom.Rt1:GetAttachment( RenderTargetProperty.TA_COLOR_0 ) );
	Bloom.RoGuassX:Draw( PipelineProperty.PT_UNIVERSAL_POST_EFFECT );
	
	Bloom.Rt1:PushRenderTarget();
	Bloom.Rt1:ClearBuffer( RenderTargetProperty.CF_COLOR );
	Bloom.RoGuassX:SetTextureEntity(
		MaterialProperty.ATT_TEXTURE_DIFFUSE,
		Bloom.Rt2:GetAttachment( RenderTargetProperty.TA_COLOR_0 ) );
	Bloom.RoGuassX:Draw( PipelineProperty.PT_UNIVERSAL_POST_EFFECT );
	
		
	Output:PushRenderTarget();
	Output:ClearBuffer( RenderTargetProperty.CF_COLOR );
	Bloom.RoCombine:SetTextureEntity(
		MaterialProperty.ATT_TEXTURE_DIFFUSE,
		Scene:GetAttachment( RenderTargetProperty.TA_COLOR_0 ) );
	Bloom.RoCombine:SetTextureEntity(
		MaterialProperty.ATT_TEXTURE_GLOSSINESS,
		Bloom.Rt1:GetAttachment( RenderTargetProperty.TA_COLOR_0 ) );
	Bloom.RoCombine:Draw( PipelineProperty.PT_UNIVERSAL_POST_EFFECT );
end