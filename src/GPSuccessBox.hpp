#include <chrono>
#include <iostream>
#include <Geode/Geode.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/binding/GJGarageLayer.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/Notification.hpp>

using namespace geode::prelude;

class GPSuccessBox : public FLAlertLayer {
public:
    struct Fields {
        EventListener<web::WebTask> m_listener;
    };

    static CCScene* scene() {
        auto scene = CCScene::create();
        scene->addChild(GPSuccessBox::create());
        return scene;
    }

    static GPSuccessBox* create() {
        GPSuccessBox* ret = new GPSuccessBox();
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

        // Do smth here idk

        return true;
    }

private:
    geode::EventListener<web::WebTask> m_listener;
};