
local Button_OnlineGame = {};
Button_OnlineGame.Script = 0;


function Button_OnlineGame:Initialize( )
  self.Script = self.HostNode:FindComponent( ScriptComponent.GetTypeName() );
end

--返回按钮回掉函数s
function Button_OnlineGame:MakePressedCallback( index )
  return self.Script:SignFunciton( self.OnOnlineGamePressed( index ), 1 );
end


--[[function Button_OnlineGame:OnOnlineGamePressed( index )
  return function ( self, widget )--系统会自动添加self变量，由于返回的函数是匿名函数，没有self指针所以此处需要将self作为第一个参数
    str = widget:GetName();
    print( "pressend " .. str );
  end
end]]--

function Button_OnlineGame:OnPressed( widget )
  str = widget:GetName();
  print( "pressend " .. str );
end

function Button_OnlineGame_factory()
  local new = { }
  Button_OnlineGame.__index = Button_OnlineGame;
  return setmetatable(new, Button_OnlineGame)
end