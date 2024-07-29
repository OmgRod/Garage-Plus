#pragma once

#include <chrono>
#include <iostream>
#include <Geode/Geode.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/binding/GJGarageLayer.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/Notification.hpp>

#include "VideoPlayer.hpp"

using namespace geode::prelude;

class GPKofiLayer : public CCLayer {
public:
    struct Fields {
        EventListener<web::WebTask> m_listener;
    };

    virtual void KeyBackClicked() {
        CCDirector::get()->popScene();
    }

    static CCScene* garageScene() {
        auto scene = CCScene::create();
        scene->addChild(GJGarageLayer::scene());
        return scene;
    }

    static CCScene* scene() {
        auto scene = CCScene::create();
        scene->addChild(GPKofiLayer::create());
        return scene;
    }

    static GPKofiLayer* create() {
        GPKofiLayer* ret = new GPKofiLayer();
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

        auto title = CCLabelBMFont::create("Support OmgRod (Garage Plus)", "goldFont.fnt");
        title->setZOrder(1);
        title->setPositionX(winSize.width * 0.5);
        title->setPositionY(winSize.height * 0.85);
        title->setID("title");
        this->addChild(title);

        auto gameBgSpr = CCSprite::create("game_bg_01_001.png");
        gameBgSpr->setScale(2.f);
        gameBgSpr->setZOrder(-99999);
        gameBgSpr->setPositionY(winSize.height / -2);
        gameBgSpr->setPositionX(winSize.width / 2);
        gameBgSpr->setID("background");
        this->addChild(gameBgSpr);
        
        std::filesystem::path videoPath = Mod::get()->getResourcesDir() / "kofiPromo.mpg";

        // Create a VideoPlayer instance
        auto player = videoplayer::VideoPlayer::create(videoPath, true);

        player->setHeight(0.9 * winSize.height * 9 / 16);
        player->setPosition(winSize.width * 0.5, winSize.height * 0.5);

        auto linkBtnSpr = ButtonSprite::create("Open Ko-fi");
        auto linkBtn = CCMenuItemSpriteExtra::create(
            linkBtnSpr, this, menu_selector(GPKofiLayer::onKofiBtn)
        );
        linkBtn->setPositionY(winSize.height * -0.4);
        menu->addChild(linkBtn);

        auto backBtn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
            this,
            menu_selector(GPFeedbackLayer::onClick)
        );
        backBtn->setPosition(winSize.width * -0.45, winSize.height * 0.4);
        backBtn->setID("back-btn");
        menu->addChild(backBtn);

        auto corner1 = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
        corner1->setAnchorPoint(CCPoint(0, 0));
        corner1->setID("corner1");
        this->addChild(corner1);

        auto corner2 = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
        corner2->setPositionX(winSize.width);
        corner2->setAnchorPoint(CCPoint(1, 0));
        corner2->setFlipX(true);
        corner2->setID("corner2");
        this->addChild(corner2);


        this->addChild(player);
        this->addChild(menu);
        return true;
    }

    void onKofiBtn(CCObject* sender) {
        geode::utils::web::openLinkInBrowser("https://ko-fi.com/omgrod");
    }

private:
    geode::EventListener<web::WebTask> m_listener;
};