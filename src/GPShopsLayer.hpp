#pragma once

#include <chrono>
#include <iostream>
#include <Geode/Geode.hpp>
#include <Geode/binding/GJShopLayer.hpp>

using namespace geode::prelude;

class GPShopsLayer : public CCLayer {
public:
    struct Fields {
        EventListener<web::WebTask> m_listener;
    };

    virtual void KeyBackClicked() {
        CCDirector::get()->popScene();
    }

    static CCScene* scene() {
        auto scene = CCScene::create();
        scene->addChild(GPShopsLayer::create());
        return scene;
    }

    static GPShopsLayer* create() {
        GPShopsLayer* ret = new GPShopsLayer();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
    
    bool init() {
        if (!CCLayer::init())
            return false;

        setKeypadEnabled(true);

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto menu = CCMenu::create();

        auto bg = CCSprite::create("GaragePlus_shopsBG.png"_spr);
        bg->setPosition({winSize.width / 2, winSize.height / 2});
        bg->setID("background");

        {
            float contentWidth = bg->getContentWidth();
            float contentHeight = bg->getContentHeight();
            float winWidth = winSize.width;
            float winHeight = winSize.height;
            bg->setScaleX(winWidth / contentWidth);
            bg->setScaleY(winHeight / contentHeight);
        }

        auto backBtn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
            this,
            menu_selector(GPFeedbackLayer::onClick)
        );
        backBtn->setPosition(winSize.width * -0.45, winSize.height * 0.4);
        backBtn->setID("back-btn");
        menu->addChild(backBtn);

        auto shopSignBtnSpr = CCSprite::createWithSpriteFrameName("shopSign_001.png");
        auto shopSignBtn = CCMenuItemSpriteExtra::create(
            shopSignBtnSpr, this, menu_selector(GJGarageLayer::onShop)
        );
        menu->addChild(shopSignBtn);

        this->addChild(bg);
        this->addChild(menu);

        return true;
    }

private:
    geode::EventListener<web::WebTask> m_listener;
};