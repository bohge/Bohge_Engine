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
//  EAGLView.m
//  Iphone
//
//  Created by Zhao on 12-8-21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>

#import "EAGLView.h"

#include "Game.h"
#include "Pipeline.h"


ActionManage::InputAction input;

@interface EAGLView (PrivateMethods)
- (void)createFramebuffer;
- (void)deleteFramebuffer;
@end

@implementation EAGLView

@dynamic context;

// You must implement this method
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

//The EAGL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:.
- (id)initWithCoder:(NSCoder*)coder
{
    self = [super initWithCoder:coder];
	if (self)
    {
        isMouseDown = FALSE;

		// set the layer to opaque
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        eaglLayer.opaque = TRUE;
        
        
        //scale view or not
        CGSize sizeView = [[UIScreen mainScreen] bounds].size;
        CGFloat width = sizeView.width;
        CGFloat height = sizeView.height;
        sizeView.width = Math::GetMax( width, height );
        sizeView.height = Math::GetMin( width, height );        
        CGSize sizeDevice = [[[UIScreen mainScreen] currentMode] size];
        width = sizeDevice.width;
        height = sizeDevice.height;
        sizeDevice.width = Math::GetMax( width, height );
        sizeDevice.height = Math::GetMin( width, height );
        if ( abs( sizeDevice.width - sizeView.width ) > 1.0f )
        {
            float ws = sizeDevice.width / sizeView.width;
            float hs = sizeDevice.height / sizeView.height;
            ws = ws < 1.0 ? 1.0 / ws : ws;
            hs = hs < 1.0 ? 1.0 / hs : hs;
            float s = 1.0;//Math::GetMin(ws, hs);
            self.contentScaleFactor = s;
            eaglLayer.contentsScale = s;
        }
		//init egl
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking,
                                        kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
                                        nil];
    }
    touchSize = [[UIScreen mainScreen] bounds].size;
    return self;
}

- (void)dealloc
{
    [self deleteFramebuffer];    
    [context release];
    
    [super dealloc];
}

- (EAGLContext *)context
{
    return context;
}

- (void)setContext:(EAGLContext *)newContext
{
    if (context != newContext)
    {
        [self deleteFramebuffer];
        
        [context release];
        context = [newContext retain];
        
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)createFramebuffer
{
    if (context && !defaultFramebuffer)
    {
        [EAGLContext setCurrentContext:context];
        
        // Create default framebuffer object.
        glGenFramebuffers(1, &defaultFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
        
        // Create color render buffer and allocate backing store.
        glGenRenderbuffers(1, &colorRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)self.layer];
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
        
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
        
		glGenRenderbuffers(1, &depthstencilRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthstencilRenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, width, height);
		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthstencilRenderbuffer );
		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthstencilRenderbuffer );
		
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));		
    }
}

- (void)deleteFramebuffer
{
    if (context)
    {
        [EAGLContext setCurrentContext:context];
        
        if (defaultFramebuffer)
        {
            glDeleteFramebuffers(1, &defaultFramebuffer);
            defaultFramebuffer = 0;
        }
        
        if (colorRenderbuffer)
        {
            glDeleteRenderbuffers(1, &colorRenderbuffer);
            colorRenderbuffer = 0;
        }
    }
}

- (void)setFramebuffer
{
	if (context)
    {
        [EAGLContext setCurrentContext:context];
        
        if (!defaultFramebuffer)
            [self createFramebuffer];
        
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
    }
}

- (GLuint)GetBufferHandle;
{
    if (context)
    {
        [EAGLContext setCurrentContext:context];
        
        if (!defaultFramebuffer)
            [self createFramebuffer];
        
        //glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
        
        //glViewport(0, 0, framebufferWidth, framebufferHeight);
		return defaultFramebuffer;
    }
	return 0;
}

- (BOOL)presentFramebuffer
{
    BOOL success = FALSE;
    
    if (context)
    {
        [EAGLContext setCurrentContext:context];
        
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        
        success = [context presentRenderbuffer:GL_RENDERBUFFER];
    }
    
    return success;
}

- (void)layoutSubviews
{
    // The framebuffer will be re-created at the beginning of the next setFramebuffer method call.
    [self deleteFramebuffer];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	if( !isMouseDown )
	{
        isMouseDown = TRUE;
        input.Begin();
        input.SetType(ActionManage::INPUT_DOWN);
        NSSet *allTouches = [event allTouches];
        for (UITouch *touch in allTouches)
        {
            CGPoint location = [touch locationInView:self];		
            //◊™ªªŒ™openglø’º‰
            float w = touchSize.height;
            float h = touchSize.width;
            float xPos = location.x;
            float yPos = h - location.y;//◊™ªª
            w = xPos / w;
            w = 2.0 * w - 1.0;
            h = yPos / h;
            h = 2.0 * h - 1.0;
            if( !input.SetPoint( vector2f( w,h ) ) )
            {
                break;
            }
        }
        BohgeGame::Game::Instance().TouchPoint( input );
	}
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	if (isMouseDown )
	{
        input.Begin();
        input.SetType(ActionManage::INPUT_MOVE);
        NSSet *allTouches = [event allTouches];
        for (UITouch *touch in allTouches)
        {
            CGPoint location = [touch locationInView:self];		
            //◊™ªªŒ™openglø’º‰
            float w = touchSize.height;
            float h = touchSize.width;
            float xPos = location.x;
            float yPos = h - location.y;//◊™ªª
            w = xPos / w;
            w = 2.0 * w - 1.0;
            h = yPos / h;
            h = 2.0 * h - 1.0;
            if( !input.SetPoint( vector2f( w,h ) ) )
            {
                break;
            }
        }
        BohgeGame::Game::Instance().TouchPoint( input );
	}
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	if (isMouseDown )
	{
		isMouseDown = FALSE;
        input.Begin();
        input.SetType(ActionManage::INPUT_UP);
        NSSet *allTouches = [event allTouches];
        for (UITouch *touch in allTouches)
        {
            CGPoint location = [touch locationInView:self];		
            //◊™ªªŒ™openglø’º‰
            float w = touchSize.height;
            float h = touchSize.width;
            float xPos = location.x;
            float yPos = h - location.y;//◊™ªª
            w = xPos / w;
            w = 2.0 * w - 1.0;
            h = yPos / h;
            h = 2.0 * h - 1.0;
            if( !input.SetPoint( vector2f( w,h ) ) )
            {
                break;
            }
        }
        BohgeGame::Game::Instance().TouchPoint( input );
	}
}
- (void)InitPipeling
{
	Engine::Instance().GetPipeline()->SetRendTargetHandle( defaultFramebuffer, vector2d(height, width) );
}

@end
