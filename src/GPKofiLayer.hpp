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

        // geode::utils::web::openLinkInBrowser("https://ko-fi.com/omgrod");
        std::filesystem::path videoPath = Mod::get()->getResourcesDir() / "kofiPromo.mpg";

        // Create a VideoPlayer instance
        auto player = videoplayer::VideoPlayer::create(videoPath, true);

        player->setWidth(0.7 * winSize.width);

        player->setPosition(winSize.width * 0.5, winSize.height * 0.5);

        this->addChild(player);

        this->addChild(menu);
        return true;
    }

private:
    geode::EventListener<web::WebTask> m_listener;
};