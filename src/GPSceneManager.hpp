#pragma once

#include <chrono>
#include <iostream>
#include <Geode/Geode.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/binding/GJGarageLayer.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/Notification.hpp>

#include "GPShopsLayer.hpp"

using namespace geode::prelude;

class GPSceneManager : public CCLayer
{
public:
    struct Fields {
        EventListener<web::WebTask> m_listener;
    };

private:
    Fields* m_fields;

public:
    GPSceneManager() : m_fields(new Fields()) {}

    ~GPSceneManager() {
        delete m_fields;
    }

    static GPSceneManager* create()
    {
        GPSceneManager* ret = new GPSceneManager();
        if (ret && ret->init())
        {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    bool init()
    {
        if (!CCLayer::init())
            return false;

        return true;
    }

    void onModSettings(CCObject* sender)
    {
        openSettingsPopup(Mod::get());
    }

    void onFeedbackBtn(CCObject* sender)
    {
        auto scenePrev = CCTransitionFade::create(0.5f, GPFeedbackLayer::scene());
        CCDirector::sharedDirector()->replaceScene(scenePrev);
    }

    void onShopsBtn(CCObject* sender)
    {
        auto scenePrev = CCTransitionFade::create(0.5f, GPShopsLayer::scene());
        CCDirector::sharedDirector()->replaceScene(scenePrev);
    }

    void demonInfo(CCObject* sender)
    {
        FLAlertLayer::create("Garage Plus", "This feature may or may not be coming soon", "OK")->show();
    }

    void starsInfo(CCObject* sender)
    {
        FLAlertLayer::create("Garage Plus", "This feature may or may not be coming soon", "OK")->show();
    }

    void moonsInfo(CCObject* sender)
    {
        FLAlertLayer::create("Garage Plus", "This feature may or may not be coming soon", "OK")->show();
    }

    void onDisabled(CCObject* sender)
    {
        FLAlertLayer::create("Garage Plus", "This feature is currently disabled possibly due to a bug.", "OK")->show();
    }
};