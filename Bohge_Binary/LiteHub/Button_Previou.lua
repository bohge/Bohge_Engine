

local Button_Previou = {};


function Button_Previou:Initialize( )
  self.maininstance = Framework:FindNodeScriptInstance("Root");
end

function Button_Previou:OnPressed( widget )
  self.maininstance:GetLiteappInfo( -GlobalDefind.BUTTON_COUNT );
end

function Button_Previou_factory()
  local new = { }
  Button_Previou.__index = Button_Previou;
  return setmetatable(new, Button_Previou)
end

