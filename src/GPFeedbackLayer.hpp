#pragma once

#include <chrono>
#include <iostream>
#include <Geode/Geode.hpp>
#include <Geode/binding/GJGarageLayer.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/Notification.hpp>

using namespace geode::prelude;

class GPFeedbackLayer : public CCLayer {
public:
    struct Fields {
        EventListener<web::WebTask> m_listener;
    };

    virtual void KeyBackClicked();

    static CCScene* scene();
    static CCScene* garageScene();
    static GPFeedbackLayer* create();

    void onFeedbackClick(CCObject* sender);
    bool init();
    void onClick(CCObject* sender);
    void onInfoClick(CCObject* sender);

private:
    geode::EventListener<web::WebTask> m_listener;
};