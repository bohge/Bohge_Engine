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




/////////////////////////////////////
//			后期渲染特效		   //
/////////////////////////////////////
#include "NormalGenerate.h"
#include "Device.h"
#include "RendBuffer.h"
#include "RenderTarget.h"
#include "ShaderManage.h"
#include "Material.h"


namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	NormalGenerate::NormalGenerate( const vector2d& size, Texture2D& n1, Texture2D& n2, IAttributeNode* node  )
	{
		VertexTexture Vertex[4];
		Vertex[0] = VertexTexture(vector4f(-1,1,0), vector2f(0,1));
		Vertex[1] = VertexTexture(vector4f(-1,-1,0), vector2f(0,0));
		Vertex[2] = VertexTexture(vector4f(1,1,0), vector2f(1,1));
		Vertex[3] = VertexTexture(vector4f(1,-1,0), vector2f(1,0));
		ushort Indies[4]={0,1,2,3};
		m_pRendBuffer = NEW RendBuffer( Device::RM_TRIANGLE_STRIP);
		m_pVertexBuffer = NEW VertexBuffer( Device::MU_STATIC );
		m_pIndicesBuffer = NEW IndicesBuffer( Device::MU_STATIC );
		m_pRendBuffer->SetVertexBuffer( m_pVertexBuffer );
		m_pRendBuffer->SetIndicesBuffer( m_pIndicesBuffer );
		m_pVertexBuffer->FlushVertexBuffer( 4, &Vertex[0] );
		m_pIndicesBuffer->FlushIndiesBuffer( 4, &Indies[0] );

		m_pHighmapRT = NEW RenderTarget( size, Device::FBO_TCOLOR);
		m_pHighmapMaterial = NEW Material;
		//m_pHighmapMaterial->SetShader( ShaderManage::HighmapGenerateShader );
		m_pHighmapMaterial->LoadTexture( Material::ATT_TEX_USER0, n1 );
		m_pHighmapMaterial->LoadTexture( Material::ATT_TEX_USER1, n2 );
		m_pHighmapMaterial->SetNode( node );

		m_pNormalmapRT = NEW RenderTarget( size, Device::FBO_TCOLOR);
		m_pNormalmapMaterial = NEW Material;
		//m_pNormalmapMaterial->SetShader( ShaderManage::NormalmapGenerateShader );
		m_pNormalmapMaterial->LoadTexture( Material::ATT_TEX_USER0, *((Texture2D*)m_pHighmapRT->GetColorBuffer()) );
		m_pNormalmapMaterial->SetNode( node );
	}
	//-------------------------------------------------------------------------------------------------------
	NormalGenerate::~NormalGenerate(void)
	{
		SAFE_DELETE( m_pNormalmapMaterial );
		SAFE_DELETE( m_pNormalmapRT );
		SAFE_DELETE( m_pHighmapMaterial );
		SAFE_DELETE( m_pHighmapRT );
		SAFE_DELETE( m_pRendBuffer );
		SAFE_DELETE( m_pVertexBuffer );
		SAFE_DELETE( m_pIndicesBuffer );
	}
	//-------------------------------------------------------------------------------------------------------
	void NormalGenerate::GenerateNormal( Engine& dirver )
	{
		RenderTarget* now = dirver.GetDevice()->PopRenderTarget( );

		dirver.GetDevice()->PushRenderTarget( m_pHighmapRT );
		dirver.GetDevice()->Draw( *m_pRendBuffer, *m_pHighmapMaterial );
		dirver.GetDevice()->PopRenderTarget();

		dirver.GetDevice()->PushRenderTarget( m_pNormalmapRT );
		dirver.GetDevice()->Draw( *m_pRendBuffer, *m_pNormalmapMaterial );
		dirver.GetDevice()->PopRenderTarget();

		dirver.GetDevice()->PushRenderTarget(now);
	}
	//-------------------------------------------------------------------------------------------------------
	Texture2D& NormalGenerate::Normalmap()
	{
		return *((Texture2D*)m_pNormalmapRT->GetColorBuffer());
	}

	//-------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------


	//#include <pthread.h>
	//#include <unistd.h>
	//#include <stdio.h>
	//#include <string.h>
	//#include <stdlib.h>
	//static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
	//static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
	//struct node
	//{
	//	int n_number;
	//	struct node *n_next;
	//} *head = NULL; /*[thread_func]*/
	///*释放节点内存 */
	//static void cleanup_handler(void *arg)
	//{
	//	printf("Cleanup handler of second thread.\n");
	//	free(arg);
	//	(void)pthread_mutex_unlock(&mtx);
	//}
	//void *thread_func(void *arg)
	//{node *p = NULL;
	//(cleanup_handler, p);
	//while (1)
	//{
	//	pthread_mutex_lock(&mtx);
	//	　　//这个mutex_lock主要是用来保护wait等待临街时期的情况，
	//	　　//当在wait为放入队列时，这时，已经存在Head等待条件激活的条件，此时可能会漏掉这种处理
	//	　　//这个while要特别说明一下，单个pthread_cond_wait功能很完善，
	//	　　//为何这里要有一个while (head == NULL)呢？因为pthread_cond_wait里的线程可能会被意外唤醒，如果这个时候head != NULL，
	//	　　//则不是我们想要的情况。这个时候，应该让线程继续进入pthread_cond_wait
	//	while (head == NULL)
	//	　　{
	//		  pthread_cond_wait(&cond, &mtx);
	//		  　　// pthread_cond_wait会先解除之前的pthread_mutex_lock锁定的mtx，然后阻塞在等待对列里休眠，直到再次被唤醒
	//		  　　//（大多数情况下是等待的条件成立而被唤醒，唤醒后，该进程会先锁定先pthread_mutex_lock(&mtx);，
	//		  　　// 再读取资源 用这个流程是比较清楚的/*block-->unlock-->wait() return-->lock*/
	//		  　　p = head;
	//			head = head->n_next;
	//			printf("Got %d from front of queue\n", p->n_number);
	//			free(p);
	//			pthread_mutex_unlock(&mtx); //临界区数据操作完毕，释放互斥锁
	//	　　}
	//　　}
	//pthread_cleanup_pop(0);
	//return 0;
	///*EC_CLEANUP_BGN (void)pthread_mutex_unlock(&mtx); EC_FLUSH("thread_func") return 1; EC_CLEANUP_END*/
	//}
	//int main(void)
	//{
	//	pthread_t tid;
	//	int i;
	//	struct node *p;
	//	pthread_create(&tid, NULL, thread_func, NULL);
	//	　　//子线程会一直等待资源，类似生产者和消费者，但是这里的消费者可以是多个消费者，而不仅仅支持普通的单个消费者，这个模型虽然简单，但是很强大 /*[tx6-main]*/
	//	for (i = 0; i < 10; i++)
	//	{
	//		p = (struct node *)malloc(sizeof(struct node));
	//		p->n_number = i;
	//		pthread_mutex_lock(&mtx); //需要操作head这个临界资源，先加锁，
	//		p->n_next = head;
	//		head = p;
	//		pthread_cond_signal(&cond);
	//		pthread_mutex_unlock(&mtx); //解锁
	//		sleep(1);
	//	}
	//	　　printf("thread 1 wanna end the cancel thread 2.\n");
	//	  pthread_cancel(tid);
	//	  　　//关于pthread_cancel，有一点额外的说明，它是从外部终止子线程，
	//	  　　//子线程会在最近的取消点，退出线程，而在我们的代码里，最近的取消点肯定就是pthread_cond_wait()了。
	//	  　　pthread_join(tid, NULL);
	//		　　printf("All done -- exiting\n");
	//		  return 0; /*[]*/ /*EC_CLEANUP_BGN return EXIT_FAILURE; EC_CLEANUP_END*/
	//　　}

}