

--初始化Lua，调用初始化函数，加载需要的库和类

function RegisterScript()
	Script:LoadClasses( CLASS_ICAMERA )
	Script:LoadClasses( CLASS_VECTOR2F )
	Script:LoadClasses( CLASS_VECTOR3F )
	return true
end

g_MaxLiveTime = 60
g_MaxPositionArchive = 100

function MarkSize()
	this:SetMarkSize( 0.05, 0.07 )
end

function CameraTrackOutdoor( icamera, position )
		pos = Vector3f(0,0,0)
		dir = Vector3f(0,1,0)
		pos:Equal( position )
		dir:NormalizeSelf()
		pos:AddEqualVector( dir:MultiplyNumber( 200.0 ) )
		icamera:LookAt( pos, position, Vector3f(0,0,-1) )
end


function CameraTrackIndoor( icamera, position )
		pos = Vector3f(0,0,0)
		dir = Vector3f(0,1,0)
		pos:Equal( position )
		dir:NormalizeSelf()
		pos:AddEqualVector( dir:MultiplyNumber( 20.0 ) )
		icamera:LookAt( pos, position, Vector3f(0,0,-1) )
end