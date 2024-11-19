#pragma once

#include <chrono>
#include <iostream>
#include <Geode/Geode.hpp>
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

    virtual void KeyBackClicked();

    static CCScene* garageScene();
    static CCScene* scene();
    static GPKofiLayer* create();

    bool init();

    void onKofiBtn(CCObject* sender);

private:
    geode::EventListener<web::WebTask> m_listener;
};