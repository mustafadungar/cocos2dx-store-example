/*
 * Copyright (C) 2012 Soomla Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "AppDelegate.h"
#include "Cocos2dxStore.h"
#include "MuffinRushAssets.h"
#include "StoreAScene.h"
#include "MainScene.h"
#include "ExampleEventHandler.h"

USING_NS_CC;

AppDelegate::AppDelegate() {
    handler = new ExampleEventHandler();
}

AppDelegate::~AppDelegate() 
{
    soomla::CCSoomla::sharedSoomla()->removeEventHandler(handler);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // We initialize CCStoreController and the event handler before
    // we open the store.
    soomla::CCSoomla::sharedSoomla()->addEventHandler(handler);

    MuffinRushAssets *assets = MuffinRushAssets::create();
    __Dictionary *storeParams = __Dictionary::create();
    storeParams->setObject(__String::create("ExamplePublicKey"), "androidPublicKey");
    storeParams->setObject(__String::create("ExampleCustomSecret"), "customSecret");

    __String *soomSec = __String::create("ExampleSoomSecret");
    soomla::CCStoreController::sharedStoreController()->setSoomSec(soomSec);

    // This is the call to initialize CCStoreController
    soomla::CCStoreController::initShared(assets, storeParams);

    /*
     * ** Set the amount of each currency to 10,000 if the **
     * ** balance drops under 1,000                        **
     *
     * ** Of course, this is just for testing...           **
     */

    CCArray *currencies =
            soomla::CCStoreInfo::sharedStoreInfo()->getVirtualCurrencies();
    CCObject *currencyObject;
    CCARRAY_FOREACH(currencies, currencyObject) {
            soomla::CCVirtualCurrency *vc =
                    dynamic_cast<soomla::CCVirtualCurrency *>(currencyObject);
            int balance = soomla::CCStoreInventory::sharedStoreInventory()->
                    getItemBalance(vc->getItemId()->getCString(), NULL);
            if (balance < 1000) {
                soomla::CCStoreInventory::sharedStoreInventory()->
                        giveItem(vc->getItemId()->getCString(), 10000 - balance, NULL);
            }
        }

    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = MainScene::getMainScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
