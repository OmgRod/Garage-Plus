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
#include "GPKofiLayer.hpp"
#include "GPFeedbackLayer.hpp" // Include this header

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
    GPSceneManager();
    ~GPSceneManager();

    static GPSceneManager* create();

    bool init();

    void onKofiBtn(CCObject* sender);
    void onModSettings(CCObject* sender);
    void onFeedbackBtn(CCObject* sender);
    void onShopsBtn(CCObject* sender);
    void demonInfo(CCObject* sender);
    void starsInfo(CCObject* sender);
    void moonsInfo(CCObject* sender);
    void onDisabled(CCObject* sender);
};