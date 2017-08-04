LUA_CLASS_NAME = String("Test_1");

Test_1 = {};


function Test_1.OnPressed( widget )
	DebugLog:Print( String("Widget %s, is pressed!\r\n"), widget:GetName() );
end



