

--初始化Lua，调用初始化函数，加载需要的库和类

function RegisterScript()
	Script:LoadClasses( LUALIB_TABLE )
	Script:LoadClasses( CLASS_WEIGHTS )
	return true
end



function CaculateWeigths( weigth )--权重计算函数，由c++调用，参数为下方设置的数值
	return weigth:Get(0)
end



function Initialization()
	this:InsertLandmark( "node-1", -40.9200744629, 0.0, -146.742050171, 1 )--添加路径图的节点，前面的参数是相对于远点的位置，第二个参数是楼层，第三个参数是节点名称
	this:InsertLandmark( "node-2", 126.903945923, 0.0, -146.610046387, 1 )
	this:InsertLandmark( "node-3", 206.939880371, 0.0, -147.00604248, 1 )
	this:InsertLandmark( "node-4", 126.704032898, 0.0, -108.959999084, 1 )
	this:InsertLandmark( "node-5", -159.510055542, 0.0, -73.18409729, 1 )
	this:InsertLandmark( "node-6", -117.804077148, 0.0, -73.18409729, 1 )
	this:InsertLandmark( "node-7", -102.912071228, 0.0, -73.18409729, 1 )
	this:InsertLandmark( "node-8", -39.2800750732, 0.0, -73.18409729, 1 )
	this:InsertLandmark( "node-9", -21.4400749207, 0.0, -73.18409729, 1 )
	this:InsertLandmark( "node-10", 17.6799259186, 0.0, -73.18409729, 1 )
	
	this:InsertLandmark( "node-11", 84.1599349976, 0.0, -73.18409729, 1 )
	this:InsertLandmark( "node-12", 127.391929626, 0.0, -73.3120956421, 1 )
	this:InsertLandmark( "node-13", 166.695922852, 0.0, -74.0120925903, 1 )
	this:InsertLandmark( "node-14", 205.399902344, 0.0, -73.8040924072, 1 )
	this:InsertLandmark( "node-15", 109.483840942, 0.0, -35.0860900879, 1 )
	this:InsertLandmark( "node-16", -158.242141724, 0.0, -13.1750926971, 1 )
	this:InsertLandmark( "node-17", -103.506164551, 0.0, -13.213092804, 1 )
	this:InsertLandmark( "node-18", 135.519836426, 0.0, -14.7600917816, 1 )
	this:InsertLandmark( "node-19", 205.279800415, 0.0, -14.8400917053, 1 )
	this:InsertLandmark( "node-20", -188.280227661, 0.0, 36.2899055481, 1 )
	
	this:InsertLandmark( "node-21", -101.260246277, 0.0, 8.76990795135, 1 )
	this:InsertLandmark( "node-22", -21.8202495575, 0.0 , 8.48990726471, 1 )
	this:InsertLandmark( "node-23", 16.6797466278, 0.0, 7.78990745544, 1 )
	this:InsertLandmark( "node-24", 108.059753418, 0.0, 7.78990745544, 1 )
	this:InsertLandmark( "node-25", -102.520248413, 0.0, 36.4299049377, 1 )
	this:InsertLandmark( "node-26", -57.1002388, 0.0, 51.3699073792, 1 )
	this:InsertLandmark( "node-27", -23.5002307892, 0.0, 41.8499031067, 1 )
	this:InsertLandmark( "node-29", 18.1397705078, 0.0, 42.40990448, 1 )
	this:InsertLandmark( "node-2+", 74.6397705078, 0.0, 42.5499038696, 1 )
	this:InsertLandmark( "node-30", -23.9202289581, 0.0, 62.7898979187, 1 )
	
	this:InsertLandmark( "node-31", -187.26020813, 0.0, 102.569900513, 1 )
	this:InsertLandmark( "node-32", -102.380218506, 0.0 , 103.269897461, 1 )
	this:InsertLandmark( "node-33", -20.1602191925, 0.0, 99.7698974609, 1 )
	this:InsertLandmark( "node-34", 0.119782932103, 0.0, 99.4898986816, 1 )
	this:InsertLandmark( "node-35", 17.7997837067, 0.0, 98.9298934937, 1 )
	this:InsertLandmark( "node-36", 112.459854126, 0.0, 99.0699005127, 1 )
	this:InsertLandmark( "node-37", 136.119857788, 0.0, 87.1698913574, 1 )
	
	this:InsertLandmark( "node-38", 110.116500854, 0.0, -15.5698127747, 1 )
	this:InsertLandmark( "node-39", 109.295982361, 0.0, -73.18409729, 1 )
	this:InsertLandmark( "node-40", 112.706474304, 0.0, 46.1793174744, 1 )
	this:InsertLandmark( "node-41", 206.485610962, 0.0, 45.9988937378, 1 )
	this:InsertLandmark( "空间应用工程与技术中心", -116.640029907, 0.0,  -79.0599822998 )
	this:InsertLandmark( "气球中心", -142.640014648, 0.0, 101.36000824 )
	this:InsertLandmark( "光电研究院", -61.360004425, 0.0, 53.3599853516 )
	this:InsertLandmark( "遥感与数字地球研究所", 61.5199966431, 0.0, 40.239982605 )
	this:InsertLandmark( "航空遥感中心", 140.239944458, 0.0, 88.8800048828 )
	this:InsertLandmark( "声学研究所北楼", 163.239959717, 0.0, -16.7799930573 )
	this:InsertLandmark( "声学研究所南楼", 160.039962769, 0.0, 45.9800109863 )
	this:InsertLandmark( "综合楼", 167.639953613, 0.0, -89.259979248 )
	
	--链接第一个参数为-1表示两个路径点之间的距离，第二，第三个参数为节点名称
	this:LinkPathBettwen( Weigths(-1), "node-1", "node-8" )
	this:LinkPathBettwen( Weigths(-1), "node-2", "node-4" )
	this:LinkPathBettwen( Weigths(-1), "node-2", "node-3" )
	this:LinkPathBettwen( Weigths(-1), "node-4", "node-12" )
	this:LinkPathBettwen( Weigths(-1), "node-5", "node-6" )
	this:LinkPathBettwen( Weigths(-1), "node-5", "node-16" )
	this:LinkPathBettwen( Weigths(-1), "node-6", "node-7" )
	this:LinkPathBettwen( Weigths(-1), "node-7", "node-8" )
	this:LinkPathBettwen( Weigths(-1), "node-7", "node-17" )
	this:LinkPathBettwen( Weigths(-1), "node-8", "node-9" )
	this:LinkPathBettwen( Weigths(-1), "node-9", "node-10" )
	this:LinkPathBettwen( Weigths(-1), "node-9", "node-22" )
	this:LinkPathBettwen( Weigths(-1), "node-10", "node-11" )
	this:LinkPathBettwen( Weigths(-1), "node-10", "node-23" )
	this:LinkPathBettwen( Weigths(-1), "node-12", "node-13" )
	this:LinkPathBettwen( Weigths(-1), "node-13", "node-14" )
	this:LinkPathBettwen( Weigths(-1), "node-14", "node-19" )
	this:LinkPathBettwen( Weigths(-1), "node-16", "node-17" )
	this:LinkPathBettwen( Weigths(-1), "node-16", "node-20" )
	this:LinkPathBettwen( Weigths(-1), "node-17", "node-21" )
	this:LinkPathBettwen( Weigths(-1), "node-18", "node-19" )
	this:LinkPathBettwen( Weigths(-1), "node-20", "node-31" )
	this:LinkPathBettwen( Weigths(-1), "node-21", "node-22" )
	this:LinkPathBettwen( Weigths(-1), "node-21", "node-25" )
	this:LinkPathBettwen( Weigths(-1), "node-22", "node-23" )
	this:LinkPathBettwen( Weigths(-1), "node-22", "node-27" )
	this:LinkPathBettwen( Weigths(-1), "node-23", "node-24" )
	this:LinkPathBettwen( Weigths(-1), "node-23", "node-28" )
	this:LinkPathBettwen( Weigths(-1), "node-25", "node-32" )
	this:LinkPathBettwen( Weigths(-1), "node-26", "node-27" )
	this:LinkPathBettwen( Weigths(-1), "node-26", "node-30" )
	this:LinkPathBettwen( Weigths(-1), "node-27", "node-30" )
	this:LinkPathBettwen( Weigths(-1), "node-28", "node-29" )
	this:LinkPathBettwen( Weigths(-1), "node-28", "node-35" )
	this:LinkPathBettwen( Weigths(-1), "node-30", "node-33" )
	this:LinkPathBettwen( Weigths(-1), "node-31", "node-32" )
	this:LinkPathBettwen( Weigths(-1), "node-32", "node-33" )
	this:LinkPathBettwen( Weigths(-1), "node-33", "node-34" )
	this:LinkPathBettwen( Weigths(-1), "node-34", "node-35" )
	this:LinkPathBettwen( Weigths(-1), "node-35", "node-36" )
	this:LinkPathBettwen( Weigths(-1), "node-36", "node-37" )
	
	this:LinkPathBettwen( Weigths(-1), "node-38", "node-15" )
	this:LinkPathBettwen( Weigths(-1), "node-38", "node-18" )
	this:LinkPathBettwen( Weigths(-1), "node-38", "node-24" )
	
	this:LinkPathBettwen( Weigths(-1), "node-39", "node-11" )
	this:LinkPathBettwen( Weigths(-1), "node-39", "node-12" )
	this:LinkPathBettwen( Weigths(-1), "node-39", "node-15" )
	
	this:LinkPathBettwen( Weigths(-1), "node-40", "node-24" )
	this:LinkPathBettwen( Weigths(-1), "node-40", "node-41" )
	this:LinkPathBettwen( Weigths(-1), "node-40", "node-31" )
	this:LinkPathBettwen( Weigths(-1), "node-40", "node-36" )
	
	this:LinkPathBettwen( Weigths(-1), "node-41", "node-219" )
	
	this:LinkPathBettwen( Weigths(-1), "空间应用工程与技术中心", "node-6" )
	this:LinkPathBettwen( Weigths(-1), "气球中心", "node-25" )
	this:LinkPathBettwen( Weigths(-1), "光电研究院", "node-26" )
	this:LinkPathBettwen( Weigths(-1), "遥感与数字地球研究所", "node-29" )
	this:LinkPathBettwen( Weigths(-1), "航空遥感中心", "node-37" )
	this:LinkPathBettwen( Weigths(-1), "声学研究所北楼", "node-18" )
	this:LinkPathBettwen( Weigths(-1), "声学研究所南楼", "node-40" )
	this:LinkPathBettwen( Weigths(-1), "声学研究所南楼", "node-41" )
	this:LinkPathBettwen( Weigths(-1), "综合楼", "node-13" )
end


function Clearup()

end


function Update( millisecond )
	return true
end