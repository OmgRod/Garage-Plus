#ifndef GP_FEEDBACK_LAYER_HPP
#define GP_FEEDBACK_LAYER_HPP

#include <chrono>
#include <Geode/Geode.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/binding/GJGarageLayer.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/Notification.hpp>

using namespace geode::prelude;

class GPFeedbackLayer : public CCLayer {
public:
    static CCScene* scene();
    static CCScene* garageScene();
    static GPFeedbackLayer* create();

    virtual void keyBackClicked();
    void onFeedbackClick(CCObject* sender);
    bool init();
    void onClick(CCObject* sender);
    void onInfoClick(CCObject* sender);

private:
    geode::EventListener<web::WebTask> m_listener;
};

#endif // GP_FEEDBACK_LAYER_HPP
