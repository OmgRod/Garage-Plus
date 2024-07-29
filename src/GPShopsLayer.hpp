// #pragma once

// #include <chrono>
// #include <iostream>
// #include <Geode/Geode.hpp>
// #include <Geode/binding/GJShopLayer.hpp>
// #include <Geode/modify/GJShopLayer.hpp>
// #include "GPShopLayer.hpp"

// using namespace geode::prelude;

// class GPShopsLayer : public CCLayer {
// public:
//     // Constructor
//     GPShopsLayer() : CCLayer() {}

//     // Method to create the garage scene
//     static CCScene* garageScene() {
//         auto scene = CCScene::create();
//         scene->addChild(GJGarageLayer::scene());
//         return scene;
//     }

//     // Key back click handler
//     virtual void KeyBackClicked() {
//         CCDirector::get()->popScene();
//     }

//     // Method to create the main scene
//     static CCScene* scene() {
//         auto scene = CCScene::create();
//         scene->addChild(GPShopsLayer::create());
//         return scene;
//     }

//     // Factory method to create an instance of GPShopsLayer
//     static GPShopsLayer* create() {
//         GPShopsLayer* ret = new GPShopsLayer();
//         if (ret && ret->init()) {
//             ret->autorelease();
//             return ret;
//         }
//         delete ret;
//         return nullptr;
//     }
    
//     // Initialization method
//     bool init() {
//         if (!CCLayer::init())
//             return false;

//         setKeypadEnabled(true);

//         auto winSize = CCDirector::sharedDirector()->getWinSize();

//         auto menu = CCMenu::create();

//         auto bg = CCSprite::create("GaragePlus_shopsBG.png"_spr);
//         bg->setPosition({winSize.width / 2, winSize.height / 2});
//         bg->setID("background");

//         {
//             float contentWidth = bg->getContentWidth();
//             float contentHeight = bg->getContentHeight();
//             float winWidth = winSize.width;
//             float winHeight = winSize.height;
//             bg->setScaleX(winWidth / contentWidth);
//             bg->setScaleY(winHeight / contentHeight);
//         }

//         auto backBtn = CCMenuItemSpriteExtra::create(
//             CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
//             this,
//             menu_selector(GPShopsLayer::onClick)
//         );
//         backBtn->setPosition(winSize.width * -0.45, winSize.height * 0.4);
//         backBtn->setID("back-btn");
//         menu->addChild(backBtn);

//         auto shopSignBtnSpr = CCSprite::createWithSpriteFrameName("shopSign_001.png");
//         auto shopSignBtn = CCMenuItemSpriteExtra::create(
//             shopSignBtnSpr, this, menu_selector(GPShopsLayer::onShop1)
//         );
//         menu->addChild(shopSignBtn);

//         this->addChild(bg);
//         this->addChild(menu);

//         return true;
//     }

//     // Click handler
//     void onClick(CCObject* sender) {
//         auto scenePrev = CCTransitionFade::create(0.5f, GPShopsLayer::garageScene());
//         CCDirector::sharedDirector()->replaceScene(scenePrev);
//     }

//     // Shop button click handler (assuming you have such a method)
//     void onShop1(CCObject* sender) {
//         // Implement your onShop1 logic here
//     }
// };
