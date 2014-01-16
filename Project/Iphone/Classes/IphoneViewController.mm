//////////////////////////////////////////////////////////////////////////////////////
//
//						The Bohge Engine License (BEL)
//
//	Copyright (c) 2011-2014 Peng Zhao
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in 
//	all copies or substantial portions of the Software. And the logo of 
//	Bohge Engine shall be displayed full screen for more than 3 seconds 
//	when the software is started. Copyright holders are allowed to develop 
//	game edit based on Bohge Engine, The edit must be released under the MIT 
//	open source license if it is going to be published. In no event shall 
//	copyright holders be prohibited from using any code of Bohge Engine 
//	to develop any other analogous game engines.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//
//////////////////////////////////////////////////////////////////////////////////////



//
//  IphoneViewController.m
//  Iphone
//
//  Created by Zhao on 12-8-21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>

#import "IphoneViewController.h"
#import "EAGLView.h"

#include "Game.h"
#include "FilePath.h"
#include <string>
#include <sys/utsname.h>

//IAP
IAPHelper* iap;
void IAPBuyItem( int index )
{
    [iap BuyItemWith: index];
}
void RefreshItem()
{
    [iap RefreshItem];
}

//Edit
CGSize          g_ViewSize;
bool            g_isKeyboardShow;
float           g_KeyboradBeginY;
float           g_EditEndY;
UITextView*     g_Edit;
uint            g_InputMaxSize;
bool            g_isNumberOnly;
void HideKeyBorad()
{
    [g_Edit resignFirstResponder];
    g_isKeyboardShow = false;
    g_Edit.hidden = TRUE;
    NSString *content = g_Edit.text;
    std::wstring wstr = (wchar_t*)[content cStringUsingEncoding:NSUTF32StringEncoding];
    Engine::Instance().GetUIManager()->OnReceiveText(wstr);
}
void Keyborad(const vector4f& rect, uint maxsize, const std::wstring& was, bool isNumberOnly)
{
    g_isKeyboardShow = true;
    g_isNumberOnly = isNumberOnly;
    g_InputMaxSize = maxsize;
    NSString* empty = @"";
    vector4f rectIos;
    rectIos.m_x = (rect.m_x+1.0)/2.0*g_ViewSize.width;
    rectIos.m_y = g_ViewSize.height - (rect.m_y+1.0)/2.0*g_ViewSize.height;
    rectIos.m_z = (rect.m_z+1.0)/2.0*g_ViewSize.width - rectIos.m_x;
    rectIos.m_w = g_ViewSize.height - (rect.m_w+1.0)/2.0*g_ViewSize.height - rectIos.m_y;
    g_EditEndY = rectIos.m_y + rectIos.m_w;
    g_Edit.frame = CGRectMake( rectIos.m_x, rectIos.m_y, rectIos.m_z, rectIos.m_w );
    [g_Edit setText:empty];
	[g_Edit setKeyboardType:UIKeyboardTypeDefault];
    g_Edit.hidden = FALSE;
    [g_Edit becomeFirstResponder];
}

// Uniform index.
enum {
    UNIFORM_TRANSLATE,
    NUM_UNIFORMS
};
GLint uniforms[NUM_UNIFORMS];

// Attribute index.
enum {
    ATTRIB_VERTEX,
    ATTRIB_COLOR,
    NUM_ATTRIBUTES
};

@interface IphoneViewController ()
@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, assign) CADisplayLink *displayLink;
@end

@implementation IphoneViewController

@synthesize animating, context, displayLink;


Engine::DeviceLevel GetDeviceLevel()
{
    struct utsname sysinfo;
    uname(&sysinfo);
    //NSString *platform = [[[UIDevice currentDevice].systemName copy] autorelease];
    NSString* platform = [NSString stringWithCString: sysinfo.machine encoding: NSUTF8StringEncoding];
    if ([platform isEqualToString:@"iPhone1,1"])    return Engine::LEL_LOW;//return @"iPhone 1G";
    if ([platform isEqualToString:@"iPhone1,2"])    return Engine::LEL_LOW;//return @"iPhone 3G";
    if ([platform isEqualToString:@"iPhone2,1"])    return Engine::LEL_LOW;//return @"iPhone 3GS";
    if ([platform isEqualToString:@"iPhone3,1"])    return Engine::LEL_BASE;//return @"iPhone 4";
    if ([platform isEqualToString:@"iPhone3,3"])    return Engine::LEL_BASE;//return @"Verizon iPhone 4";
    if ([platform isEqualToString:@"iPhone4,1"])    return Engine::LEL_ULTRA;//return @"iPhone 4S";
    if ([platform isEqualToString:@"iPod1,1"])      return Engine::LEL_LOW;//return @"iPod Touch 1G";
    if ([platform isEqualToString:@"iPod2,1"])      return Engine::LEL_LOW;//return @"iPod Touch 2G";
    if ([platform isEqualToString:@"iPod3,1"])      return Engine::LEL_LOW;//return @"iPod Touch 3G";
    if ([platform isEqualToString:@"iPod4,1"])      return Engine::LEL_BASE;//return @"iPod Touch 4G";
    if ([platform isEqualToString:@"iPad1,1"])      return Engine::LEL_LOW;//return @"iPad";
    if ([platform isEqualToString:@"iPad2,1"])      return Engine::LEL_ULTRA;//return @"iPad 2 (WiFi)";
    if ([platform isEqualToString:@"iPad2,2"])      return Engine::LEL_ULTRA;//return @"iPad 2 (GSM)";
    if ([platform isEqualToString:@"iPad2,3"])      return Engine::LEL_ULTRA;//return @"iPad 2 (CDMA)";
    if ([platform isEqualToString:@"iPad2,4"])      return Engine::LEL_ULTRA;//return @"iPad 2";
    if ([platform isEqualToString:@"iPad3,1"])      return Engine::LEL_ULTRA;//return @"iPad-3G (WiFi)";
    if ([platform isEqualToString:@"iPad3,2"])      return Engine::LEL_ULTRA;//return @"iPad-3G (4G)";
    if ([platform isEqualToString:@"iPad3,3"])      return Engine::LEL_ULTRA;//return @"iPad-3G (4G)";
    return Engine::LEL_ULTRA;
}


- (void)awakeFromNib
{
    EAGLContext *aContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    if (!aContext)
    {
        ASSERT(false);
    }
    
	self.context = aContext;
	[aContext release];
	
    [(EAGLView *)self.view setContext:context];
    [(EAGLView *)self.view setFramebuffer];
    
	animating = FALSE;
    animationFrameInterval = 1;
    self.displayLink = nil;
    
    //chu shi hua edit
    CGSize sizeDevice = [[UIScreen mainScreen] bounds].size;
    CGFloat width = sizeDevice.width;
    CGFloat height = sizeDevice.height;
    sizeDevice.width = fmax( width, height );
    sizeDevice.height = fmin( width, height );
    g_ViewSize = sizeDevice;
    g_KeyboradBeginY = sizeDevice.height / 2; // chu shi hua , bao zheng bu wei 0;
    g_Edit = [[UITextView alloc] initWithFrame:CGRectMake(0, 0, sizeDevice.width, sizeDevice.height/2)];
	g_Edit.textAlignment=UITextAlignmentLeft;
	g_Edit.autocorrectionType=UITextAutocorrectionTypeNo;
	g_Edit.autocapitalizationType=UITextAutocapitalizationTypeNone;
    g_Edit.returnKeyType = UIReturnKeyDone;
    g_Edit.backgroundColor = [UIColor whiteColor];
    g_Edit.layer.cornerRadius = 15;
    g_Edit.font = [UIFont systemFontOfSize:30.0];
    g_Edit.delegate = self; 
    g_Edit.hidden = TRUE;
    [self.view addSubview:g_Edit];
    //my code
	NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES );
	NSString *documentsDirectory = [paths objectAtIndex:0];		
	const char *scoreFile = [[documentsDirectory stringByAppendingPathComponent:@"Save"] UTF8String];
	NSString* appFolderPath = [[NSBundle mainBundle] resourcePath];
	const char* rootpath = [[appFolderPath stringByAppendingString:@"/"] UTF8String];
	FILEPATH.SetWritePath(std::string(scoreFile)+std::string("/"));
	FILEPATH.SetRootPath(std::string(rootpath));
    /*if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
    {
        CGRect rect = [[UIScreen mainScreen] bounds];
        Game::Instance().Initialization(rect.size.height, rect.size.width, GetDeviceLevel(), false, std::string(rootpath));
    }
    else
    {
        CGSize size = [[[UIScreen mainScreen] currentMode] size];
        Game::Instance().Initialization(size.height, size.width, GetDeviceLevel(), false, std::string(rootpath));    
    }*/
    NSString *uuid = [[UIDevice currentDevice] uniqueIdentifier];
    std::string strudid = [uuid UTF8String];
    
    CGSize size = [[[UIScreen mainScreen] currentMode] size];
    BohgeGame::Game::Instance().Initialization( vector2d(size.height, size.width), vector2d(size.height, size.width), GetDeviceLevel(), false, std::string(rootpath), strudid );
	[(EAGLView *)self.view InitPipeling];
    //iap
    m_IAP = [IAPHelper alloc];
    iap = m_IAP;
    [m_IAP initlized];
    BohgeGame::Game::Instance().CheckIapStatus();
}


- (void)viewDidLoad {
    [super viewDidLoad];
    
    
    if ( [[UIDevice currentDevice].systemVersion floatValue] >= 5.0)
    {
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                selector:@selector(keyboardWillShow:)
                                                name:UIKeyboardWillChangeFrameNotification object:nil];
    }
    else
    {
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(keyboardWillShow:)
                                                 name:UIKeyboardWillShowNotification object:nil];
    }  
    
    
    
    // 在屏幕底部创建标准尺寸的视图。
    bannerView_ = [[GADBannerView alloc] initWithAdSize: kGADAdSizeSmartBannerLandscape];    
    
    // 指定广告的“单元标识符”，也就是您的 AdMob 发布商 ID。
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
    {
        std::string adid = BohgeGame::Game::Instance().GetAdmobID(BohgeGame::AdHelper::PF_IPAD);
        //printf( "%s", adid.c_str() );
        bannerView_.adUnitID = [NSString stringWithUTF8String: adid.c_str()];
    }
    else
    {
        std::string adid = BohgeGame::Game::Instance().GetAdmobID(BohgeGame::AdHelper::PF_IPHONE);
        //printf( "%s", adid.c_str() );        
        bannerView_.adUnitID = [NSString stringWithUTF8String: adid.c_str()];    
    }
    
    // 告知运行时文件，在将用户转至广告的展示位置之后恢复哪个 UIViewController 
    // 并将其添加至视图层级结构。
    bannerView_.rootViewController = self;
    [self.view addSubview:bannerView_];
    
    // 启动一般性请求并在其中加载广告。
    [bannerView_ loadRequest:[GADRequest request]];
}


- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    if (program)
    {
        glDeleteProgram(program);
        program = 0;
    }
    
    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
    
    [context release];
    
    [super dealloc];
}

- (void)viewWillAppear:(BOOL)animated
{
    [self startAnimation];
    
    [super viewWillAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [self stopAnimation];
    
    [super viewWillDisappear:animated];
}

- (void)viewDidUnload
{
	[super viewDidUnload];
	[bannerView_ release];
    if (program)
    {
        glDeleteProgram(program);
        program = 0;
    }

    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
	self.context = nil;	
}

- (NSInteger)animationFrameInterval
{
    return animationFrameInterval;
}

- (void)setAnimationFrameInterval:(NSInteger)frameInterval
{
    /*
	 Frame interval defines how many display frames must pass between each time the display link fires.
	 The display link will only fire 30 times a second when the frame internal is two on a display that refreshes 60 times a second. The default frame interval setting of one will fire 60 times a second when the display refreshes at 60 times a second. A frame interval setting of less than one results in undefined behavior.
	 */
    if (frameInterval >= 1)
    {
        animationFrameInterval = frameInterval;
        
        if (animating)
        {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

- (void)startAnimation
{
    if (!animating)
    {
        CADisplayLink *aDisplayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawFrame)];
        [aDisplayLink setFrameInterval:animationFrameInterval];
        [aDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        self.displayLink = aDisplayLink;
        
        animating = TRUE;
    }
}

- (void) onBackground
{
    BohgeGame::Game::Instance().BecomeFrontground();
}

- (void)stopAnimation
{
    if (animating)
    {
        [self.displayLink invalidate];
        self.displayLink = nil;
        animating = FALSE;
    }
    BohgeGame::Game::Instance().BecomeBackground();
}

- (void)drawFrame
{
	BohgeGame::Game::Instance().Update();
    if( false == BohgeGame::Game::Instance().isShowAd() 
       && false == [ bannerView_ isHidden ] )
    {
        bannerView_.hidden = true; 
    }
    else if ( true == BohgeGame::Game::Instance().isShowAd() && true == [ bannerView_ isHidden ] )
    {
        bannerView_.hidden = false;
    }
	[(EAGLView *)self.view GetBufferHandle];
	BohgeGame::Game::Instance().Paint();
    [(EAGLView *)self.view presentFramebuffer];
}

- (void) onExit
{
	BohgeGame::Game::Instance().Exit();
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

//-(BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
//{
//    return (interfaceOrientation == UIInterfaceOrientationLandscapeRight);
//}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation 
{
    return UIInterfaceOrientationIsLandscape( interfaceOrientation );
}

- (BOOL) shouldAutorotate {
    return YES;
}

- (void)textViewDidBeginEditing:(UITextView *)textView
{
    if( g_EditEndY > g_KeyboradBeginY )
    {
        [UIView beginAnimations:nil context:NULL];
        [UIView setAnimationDuration:.3];
        self.view.transform = CGAffineTransformTranslate(self.view.transform, 0, 
                                                         g_KeyboradBeginY - g_EditEndY );
        [UIView commitAnimations];
    }
}

- (void)textViewDidEndEditing:(UITextView *)textView
{
    g_isKeyboardShow = false;
    NSString *content = g_Edit.text;
    std::wstring wstr = (wchar_t*)[content cStringUsingEncoding:NSUTF32StringEncoding];
    Engine::Instance().GetUIManager()->OnReceiveText(wstr);
    g_Edit.hidden = TRUE;
    if( g_EditEndY > g_KeyboradBeginY )
    {
        [UIView beginAnimations:nil context:NULL];
        [UIView setAnimationDuration:.3];
        self.view.transform = CGAffineTransformTranslate(self.view.transform, 0, 
                                                         g_EditEndY - g_KeyboradBeginY );
        [UIView commitAnimations];
    }
}

- (void) keyboardWillShow:(NSNotification *)notification 
{
    /*CGSize keyboardSize = [[[notification userInfo] 
                            objectForKey:UIKeyboardFrameBeginUserInfoKey] 
                           CGRectValue].size;*/
    if( g_isKeyboardShow )
    {
        g_isKeyboardShow = false;
        CGRect keyboardEndFrame;
        [[notification.userInfo objectForKey:UIKeyboardFrameEndUserInfoKey] getValue:&  keyboardEndFrame];
        CGRect keyboardFrame = [self.view convertRect:keyboardEndFrame fromView:nil];
        
        g_KeyboradBeginY = keyboardFrame.origin.y;// you yu shi heng ban muo shi
    }
}


#define NUMBERS_ONLY @"1234567890"
- (BOOL)textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text {
    
    if([text isEqualToString:@"\n"]) 
    {
        [g_Edit resignFirstResponder];
        g_Edit.hidden = TRUE;
        return NO;
    }
    else if( range.location >= g_InputMaxSize )
    {
        return NO;
    }
    else if( g_isNumberOnly )
    {
        NSCharacterSet *cs = [[NSCharacterSet characterSetWithCharactersInString:NUMBERS_ONLY] invertedSet];
        NSString *filtered = [[text componentsSeparatedByCharactersInSet:cs] componentsJoinedByString:@""];
        return [text isEqualToString:filtered];
    }
    return YES;
}
@end
