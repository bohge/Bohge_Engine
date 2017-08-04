require("debug/mobdebug").start(); --调试代码
require("GlobalDefined");
JSON = require("json");--全局json解析


local main = {};
--main.Camera = 0;
--main.Bloom = 0;
--main.MainUI = 0;
--main.PostHandle = 0;
--main.Script = 0;

--main.LayoutUpbar = 0;
--main.LayoutContent = 0;
--main.LayoutDownbar = 0;
--main.LayoutError = 0
--main.NextButton = 0;
--main.PreviouButton = 0;
--main.ContentComponent = {};--保存界面中的按钮组建以及其他组建
--main.ContentComponent[i].Button;--UI中的根节点
--main.ContentComponent[i].Icon;
--main.ContentComponent[i].Name;
--main.WebLiteInfo = {}
--main.WebLiteInfo[i].LiteID;
--main.WebLiteInfo[i].LiteName;
--main.WebLiteInfo[i].DownloadIcon;
--main.WebLiteInfo[i].Urllrl;

--main.BeginID = 0;--查询游戏信息的ID

function main:ShowErrorMessage( )
  self.LayoutError:EnableWithChild();
  self.LayoutUpbar:DisableWithChild();
  self.LayoutContent:DisableWithChild();
  self.LayoutDownbar:DisableWithChild();
end

function main:GetLiteappInfo( count )
  self.BeginID = self.BeginID + count;
  self:GetLiteinfo();
  self.NextButton:DisableWithChild();
  self.PreviouButton:DisableWithChild();
end

function main:Initialize( )
	self.BeginID = 0
	--self.Bloom = PostEffect();
	--self.Bloom:CreatePostEffect( "defs:PostEffect/Bloom.lua" );
	self.Camera = StaticCamera();
	self.Camera:CreatePerspectiveProjection( 1, 100 );
	self.Camera:LookAt(
		vector3(0,0,10),
		vector3(0,0,0),
		vector3(0,1,0) );
	self.Camera:AttachCamera();
	--找到脚本组件
	self.Script = self.HostNode:FindComponent( ScriptComponent.GetTypeName() );
  
	--初始化UI
	self.MainUI = UIEntity();
	self.MainUI:PushMetadata( PathMetadata( "lite:UserInterface/LiteHub.windows" ) );
	local callback = self.Script:SignFunciton( self.OnUILoaded, 1 );
	self.MainUI:SetScriptLoadedEvent( callback );
	self.MainUI:CreateResource();
end

function main:GetLiteinfo()
  self.PostHandle = IWebSystem:PostHandleFactory();
  callback = self.Script:SignFunciton( self.OnPostHandleFinish, 1 );
  self.PostHandle:SetFinishCallback( callback );
  self.PostHandle:SetUrl( "http://info.bohge.com/litehub_info/Lites.php" );
  local framework_ver = Framework:GetVersion();
  self.PostHandle:SetPostContent( "VERSION", tostring(framework_ver) );
  self.PostHandle:SetPostContent( "LOCATION", "cn" );
  self.PostHandle:SetPostContent( "RESULT_COUNT", tostring(GlobalDefind.BUTTON_COUNT) );  
  self.PostHandle:SetPostContent( "BEGIN_ID", tostring(self.BeginID) );
  self.PostHandle:Start();  
end

function main:OnUILoaded( entity )--ui加载后
  self.LayoutUpbar = entity:FindLayout( "Upbar" );
  self.LayoutContent = entity:FindLayout( "Content" );
  self.LayoutDownbar = entity:FindLayout( "Downbar" );  
  self.LayoutError = entity:FindLayout( "Error" );
  self.LayoutError:HideWithChild();
  self.NextButton = self.LayoutDownbar:FindWidget( "Button_Next" );
  self.PreviouButton = self.LayoutDownbar:FindWidget( "Button_Previou" );
  self.NextButton:DisableWithChild();
  self.PreviouButton:DisableWithChild();
  self.ContentComponent = {};
  for i = 1, GlobalDefind.BUTTON_COUNT do
    self.ContentComponent[i] = {};
    self.ContentComponent[i].Button = self.LayoutContent:FindWidget( "Button_" .. i );
    local sc = self.ContentComponent[i].Button:GetScriptComponent();
    self.ContentComponent[i].Button_Script = sc:GetScriptInstance();
    self.ContentComponent[i].Button:Disable();
    self.ContentComponent[i].Icon = self.LayoutContent:FindWidget( "LiteIcon_" .. i );
    self.ContentComponent[i].Name = self.LayoutContent:FindWidget( "LiteName_" .. i );
  end
  self:GetLiteinfo();--查询
end

function main:OnPostHandleFinish( handle )--查询网络资源
  if IWebProperty.HS_OK ~= handle:GetStatusCode() then
    self:ShowErrorMessage();
  else
    local res = handle:GetResult();
    local liteinfo = JSON:decode(res)
    self.WebLiteInfo = {};--清除旧数据
    if liteinfo["isSucess"] then
      local ResultInfo = liteinfo["Result"];
      for i,ele in ipairs( ResultInfo ) do--如果成功，则下载图片
        self.WebLiteInfo[i] = {}
        self.WebLiteInfo[i].LiteID = ele.liteid;
        self.WebLiteInfo[i].LiteName = ele.litename;
        self.WebLiteInfo[i].Urllrl = ele.urllrl;
        
        --下载游戏图标
        self.WebLiteInfo[i].DownloadIcon = IWebSystem:DownloadHandleFactory();
        self.WebLiteInfo[i].DownloadIcon:SetUrl( ele.urlicon );
        local filepath = "lite:Icons/" .. ele.liteid .. "." .. ele.urlicon:match(".+%.(%w+)$");
        self.WebLiteInfo[i].DownloadIcon:SetPath( filepath );
        local callback = self.Script:SignFunciton( self:OnIconDownloadFinish( i, filepath ), 1 );
        self.WebLiteInfo[i].DownloadIcon:SetFinishCallback( callback );      
        self.WebLiteInfo[i].DownloadIcon:Start();
        
        --针对UI相关操作
        --[[self.ContentComponent[i].Button:ReplacePressedCallback( 
          self.ContentComponent[i].Button_Script:MakePressedCallback( i ) );]]--
        self.ContentComponent[i].Button:EnableWithChild();
        self.ContentComponent[i].Name:ReplaceString( ele.litename );
      end
      for i = #ResultInfo + 1, GlobalDefind.BUTTON_COUNT do
        self.ContentComponent[i].Button:HideWithChild();
      end
      if #ResultInfo == GlobalDefind.BUTTON_COUNT then
        self.NextButton:EnableWithChild();
      end
      if 0 ~= self.BeginID then
        self.PreviouButton:EnableWithChild();      
      end
    else--如果服务器出错，显示错误提示框，退出游戏
      self:ShowErrorMessage();
    end
  end
end

function main:OnIconDownloadFinish( index, filepath )--传递的为索引，可以直接修改对应图片
  return function( handle )
    self.ContentComponent[index].Icon:ReplaceTexture( filepath );
  end
end

function main:BeginUpdate( )
end


function main:DoUpdate( )
  
end


function main:EndUpdate( )

end


function main:Exit( )
end


function main_factory()
  local new = { }
  main.__index = main;
  return setmetatable(new, main)
end