

--初始化Lua，调用初始化函数，加载需要的库和类

function RegisterScript()
	Script:LoadClasses( LUALIB_BASE )
	Script:LoadClasses( LUALIB_MATH )
	Script:LoadClasses( CLASS_VECTOR3F )
	return true
end




function Initialization()

end

function Clearup()

end


PassedTime = 0
function Update( millisecond )
	PassedTime = PassedTime + millisecond  * 0.001
	y = math.cos( PassedTime ) * 10
	vec3f = Vector3f( 0, y, 0 )
	this:SetLocalPosition( vec3f )
	return true
end