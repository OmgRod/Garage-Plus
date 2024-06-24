#include <chrono>
#include <iostream>
#include <Geode/Geode.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/binding/GJGarageLayer.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/Notification.hpp>

using namespace geode::prelude;

class MyLayer : public CCLayer
{
public:
    struct Fields {
        EventListener<web::WebTask> m_listener;
    };

private:
    Fields* m_fields;

public:
    MyLayer() : m_fields(new Fields()) {}

    ~MyLayer() {
        delete m_fields;
    }

    static CCScene* scene()
    {
        auto scene = CCScene::create();
        scene->addChild(MyLayer::create());
        return scene;
    }

    static MyLayer* create()
    {
        MyLayer* ret = new MyLayer();
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

    void onClick(CCObject* sender)
    {
        auto scenePrev = CCTransitionFade::create(0.5f, MyLayer::scene());
        CCDirector::sharedDirector()->replaceScene(scenePrev);
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