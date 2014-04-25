

--初始化Lua，调用初始化函数，加载需要的库和类

function RegisterScript()
	Script:LoadClasses( LUALIB_TABLE )
	Script:LoadClasses( CLASS_VECTOR3F )
	return true
end


function Initialization()
	this:AddLandmark( "空间应用工程与技术中心", -116.640029907, 0.0,  -79.0599822998 )--增加感兴趣的目标点，第一个参数为目标名字，第二个为坐标
	this:AddLandmark( "气球中心", -142.640014648, 0.0, 101.36000824 )
	this:AddLandmark( "光电研究院", -61.360004425, 0.0, 53.3599853516 )
	this:AddLandmark( "遥感与数字地球研究所", 61.5199966431, 0.0, 40.239982605 )
	this:AddLandmark( "航空遥感中心", 140.239944458, 0.0, 88.8800048828 )
	this:AddLandmark( "声学研究所北楼", 163.239959717, 0.0, -16.7799930573 )
	this:AddLandmark( "声学研究所南楼", 160.039962769, 0.0, 45.9800109863 )
	this:AddLandmark( "综合楼", 167.639953613, 0.0, -89.259979248 )
end


function Clearup()

end


function Update( millisecond )
	return true
end