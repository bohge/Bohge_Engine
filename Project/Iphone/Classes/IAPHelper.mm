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
//  IAPHelper.m
//  Iphone
//
//  Created by zhao peng on 12-10-13.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#import "IAPHelper.h"
#include "ShopHelper.h"
#include "Log.h"

#define Item1 @"com.BPGames.MisterCubeOnline.1"
#define Item2 @"com.BPGames.MisterCubeOnline.2"
#define Item3 @"com.BPGames.MisterCubeOnline.3"
#define Item4 @"com.BPGames.MisterCubeOnline.4"
#define Item5 @"com.BPGames.MisterCubeOnline.5"
#define Item6 @"com.BPGames.MisterCubeOnline.6"


@implementation IAPHelper

//这里发送请求  
- (void)requestProductData:(NSString *) Identifier
{  
    SKProductsRequest *request = [[SKProductsRequest alloc]initWithProductIdentifiers:  
                                  [NSSet setWithObject: Identifier]];
    request.delegate = self;  
    [request start];  
}  

- (void)RefreshItem
{
    for ( SKProductList::iterator it = m_ProductList.begin();
         it != m_ProductList.end();
         it++ )
    {
        [(*it) release];
    }
    m_ProductList.clear();
    [self requestProductData: Item1];
    [self requestProductData: Item2];
    [self requestProductData: Item3];
    [self requestProductData: Item4];
    [self requestProductData: Item5];
    [self requestProductData: Item6];
    //ShopHelper::Instance().InsertItemString( 1, L"1", L"1" );
    //ShopHelper::Instance().InsertItemString( 1, L"1", L"1" );
    //ShopHelper::Instance().InsertItemString( 1, L"1", L"1" );
    //ShopHelper::Instance().InsertItemString( 1, L"1", L"1" );
    //ShopHelper::Instance().InsertItemString( 1, L"1", L"1" );
    //ShopHelper::Instance().InsertItemString( 1, L"1", L"1" );
}

- (void)initlized
{
    if ([SKPaymentQueue canMakePayments])
    {
        BohgeGame::ShopHelper::Instance().isAllowIAP() = true;
    }
    else
    {
        BohgeGame::ShopHelper::Instance().isAllowIAP() = false;        
    }
    //[[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
}

//release
- (void)dealloc
{
    for ( SKProductList::iterator it = m_ProductList.begin();
         it != m_ProductList.end();
         it++ )
    {
        [(*it) release];
    }
    m_ProductList.clear();
    [super dealloc];
}

//这个是响应的delegate方法  
#pragma mark -
#pragma mark SKProductsRequestDelegate methods

- (NSString*)localizedPrice:(SKProduct*)product {
    NSNumberFormatter *numberFormatter = [[NSNumberFormatter alloc] init];
    [numberFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
    [numberFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
    [numberFormatter setLocale:product.priceLocale];
    NSString *formattedString = [numberFormatter stringFromNumber:product.price];
    return formattedString;
}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    NSArray *products = response.products;
    SKProduct *Product = [products count] == 1 ? [[products firstObject] retain] : nil;
    if (Product )
    {
        if( nil != Product.localizedTitle && nil != Product.priceLocale && nil != Product.price )
        {
            BohgeGame::ShopHelper::Instance().InsertItemString([Product.price intValue],
                                                (wchar_t*)[[self localizedPrice:Product ] cStringUsingEncoding:NSUTF32StringEncoding],
                                                (wchar_t*)[Product.localizedTitle cStringUsingEncoding:NSUTF32StringEncoding] );
            m_ProductList.push_back( Product );
            //NSLog(@"Product price: %d", [Product.price intValue]);
            //NSLog(@"Product localizedTitle: %s", [Product.localizedTitle cStringUsingEncoding:NSUTF16StringEncoding]);
            //NSLog(@"Product localizedTitle: %s", [Product.localizedDescription cStringUsingEncoding:NSUTF16StringEncoding]);
            //NSLog(@"Product localizedTitle: %s", [[self localizedPrice:Product ] cStringUsingEncoding:NSUTF16StringEncoding]);
        }
    }    
    // finally release the reqest we alloc/init’ed in requestProUpgradeProductData
    //[Product release];
}
//buy
- (void) BuyItemWith:(int) index
{
    SKPayment *payment = nil;
    NSString* productIdentifier;
    SKProduct *Product;
    switch (index) {
        case 0: productIdentifier = Item1; break;
        case 1: productIdentifier = Item2; break;
        case 2: productIdentifier = Item3; break;
        case 3: productIdentifier = Item4; break;
        case 4: productIdentifier = Item5; break;
        case 5: productIdentifier = Item6; break;
        default: productIdentifier = Item1; break;
    }
    for ( SKProductList::iterator it = m_ProductList.begin();
         it != m_ProductList.end();
         it++ )
    {
        if( [productIdentifier isEqualToString: (*it).productIdentifier ] )
        {
            Product = (*it);
            break;
        }
    }
    payment = [SKPayment paymentWithProduct: Product];
    //NSLog(@"Buy item: %s", payment.productIdentifier);
    [[SKPaymentQueue defaultQueue] addPayment:payment];//insert to default queue and wite result
}

- (void) completeTransaction: (SKPaymentTransaction *)transaction
{
    BohgeGame::ShopHelper::Instance().PaymentTransactionStatePurchased( std::string( (char*)[transaction.transactionReceipt bytes] ) );
    // Remove the transaction from the payment queue.
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

- (void) restoreTransaction: (SKPaymentTransaction *)transaction
{
    BohgeGame::ShopHelper::Instance().PaymentTransactionStatePurchased( std::string( (char*)[transaction.transactionReceipt bytes] ) );
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}
- (void) failedTransaction: (SKPaymentTransaction *)transaction
{
    BohgeGame::ShopHelper::Instance().PaymentTransactionStateFailed();
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

// result
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    for (SKPaymentTransaction *transaction in transactions)
    {
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased:
                [self completeTransaction:transaction];
                break;
            case SKPaymentTransactionStateFailed:
                [self failedTransaction:transaction];
                break;
            case SKPaymentTransactionStateRestored:
                [self restoreTransaction:transaction];
            default:
                break;
        }
    }
}
@end
