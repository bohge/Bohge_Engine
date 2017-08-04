
local Button_Next = {};

function Button_Next:Initialize( )
  self.maininstance = Framework:FindNodeScriptInstance("Root");
end

function Button_Next:OnPressed( widget )
  self.maininstance:GetLiteappInfo( GlobalDefind.BUTTON_COUNT );
end

function Button_Next_factory()
  local new = { }
  Button_Next.__index = Button_Next;
  return setmetatable(new, Button_Next)
end

