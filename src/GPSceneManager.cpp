#include "GPSceneManager.hpp"

GPSceneManager::GPSceneManager() : m_fields(new Fields()) {}

GPSceneManager::~GPSceneManager() {
    delete m_fields;
}

GPSceneManager* GPSceneManager::create() {
    GPSceneManager* ret = new GPSceneManager();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool GPSceneManager::init() {
    if (!CCLayer::init())
        return false;

    return true;
}

void GPSceneManager::onKofiBtn(CCObject* sender) {
    auto scenePrev = CCTransitionFade::create(0.5f, GPKofiLayer::scene());
    CCDirector::sharedDirector()->replaceScene(scenePrev);
}

void GPSceneManager::onModSettings(CCObject* sender) {
    openSettingsPopup(Mod::get());
}

void GPSceneManager::onFeedbackBtn(CCObject* sender) {
    auto scenePrev = CCTransitionFade::create(0.5f, GPFeedbackLayer::scene());
    CCDirector::sharedDirector()->replaceScene(scenePrev);
}

void GPSceneManager::onShopsBtn(CCObject* sender) {
    // auto scenePrev = CCTransitionFade::create(0.5f, GPShopsLayer::scene());
    // CCDirector::sharedDirector()->replaceScene(scenePrev);
}

void GPSceneManager::demonInfo(CCObject* sender) {
    FLAlertLayer::create("Garage Plus", "This feature may or may not be coming soon", "OK")->show();
}

void GPSceneManager::starsInfo(CCObject* sender) {
    FLAlertLayer::create("Garage Plus", "This feature may or may not be coming soon", "OK")->show();
}

void GPSceneManager::moonsInfo(CCObject* sender) {
    FLAlertLayer::create("Garage Plus", "This feature may or may not be coming soon", "OK")->show();
}

void GPSceneManager::onDisabled(CCObject* sender) {
    FLAlertLayer::create("Garage Plus", "This feature is currently disabled possibly due to a bug.", "OK")->show();
}