

--初始化Lua，调用初始化函数，加载需要的库和类

function RegisterScript()
	Script:LoadClasses( CLASS_VECTOR2F )
	Script:LoadClasses( CLASS_VECTOR3F )
	Script:LoadClasses( CLASS_COLOR )
	Script:LoadClasses( CLASS_QUATERNIONF )
	Script:LoadClasses( CLASS_MATRIX44F )
	
	Script:LoadClasses( CLASS_ENGINE )
	Script:LoadClasses( CLASS_ICAMERA )
	
	Script:LoadClasses( CLASS_UIMANAGER )
	Script:LoadClasses( CLASS_UIGROUP )
	Script:LoadClasses( CLASS_UIBUTTON )
	return true
end



globalpos = 0
LoadedUI = 0
function Initialization()
	this:StartAnimation()
	globalpos = this:GetLocalPosition()
	uimana = EngineInstance:GetUIManager()
	LoadedUI = uimana:CreateUIGroup( "TreeUI.ui" )
	uibutton = LoadedUI:ControlUIButton( "OK" )
	uibutton:SetString( Vector2f( 0.08, 0.07 ), Color( 1.0, 0.0, 0.0, 1.0), false, "ok" )
	uibutton:AddResponse( Script, "OnOkCallBack" )
end

function Clearup()
	uimana = EngineInstance:GetUIManager()
	uimana:DeleteUIGroup( LoadedUI )
end

function OnOkCallBack( ui )
	PassedTime = 0
	if isEmit then
		isEmit = false
		this:StopAnimation()
	else
		isEmit = true
		this:StartAnimation()
	end
end

PassedTime = 0
PassedTimeNotoZero = 0;
isEmit = true
function Update( millisecond )
	camera = EngineInstance:GetCamera()
	PassedTimeNotoZero = PassedTimeNotoZero + millisecond
	PassedTime = PassedTime + millisecond
	vec3f = Vector3f( 0, 1, 0 )
	Rotation = Quaternionf( 0,0,0,1 )
	Rotation:RotateAxis( vec3f, PassedTimeNotoZero * 0.003 )
	Scale3f = this:GetWorldScale()
	vec3f = Vector3f( 50, 0, 0 )
	vec3f:MultiplyEqualVector( Scale3f )
	inv = Rotation:ToMatrix44()
	inv:InverseSelf()
	Rotation = inv:ToQuaternion()
	vec3f:MultiplyEqualQuaternion( Rotation )
	if isEmit then
		camePos = camera:GetPosition()
		cameDir = camera:GetForward()
		cameDir:MultiplyEqualNumber( 200 )
		camePos:AddEqualVector( cameDir )
		--vec3f = globalpos:AddVector(vec3f)
		vec3f = camePos:AddVector(vec3f)
		this:SetLocalPosition( vec3f )
		if PassedTime > 20000 then
			isEmit = false
			this:StopAnimation()
		end
	else
		if PassedTime > 23000 then

			PassedTime = 0
			isEmit = true
			this:StartAnimation()
		end
	end
	return true

end