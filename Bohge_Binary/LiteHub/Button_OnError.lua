

local Button_OnError = {};


function Button_OnError:OnPressed( widget )
	print( "Widget " .. widget:GetName() .. " is pressed!\r\n" );
end


function Button_OnError_factory()
  local new = { }
  Button_OnError.__index = Button_OnError;
  return setmetatable(new, Button_OnError)
end
