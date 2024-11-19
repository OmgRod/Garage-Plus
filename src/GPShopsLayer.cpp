#include "GPShopsLayer.hpp"

void GPShopsLayer::KeyBackClicked() {
    CCDirector::get()->popScene();
}

CCScene* GPShopsLayer::scene() {
    auto scene = CCScene::create();
    scene->addChild(GPShopsLayer::create());
    return scene;
}

CCScene* GPShopsLayer::garageScene() {
    auto scene = CCScene::create();
    scene->addChild(GJGarageLayer::scene());
    return scene;
}

GPShopsLayer* GPShopsLayer::create() {
    GPShopsLayer* ret = new GPShopsLayer();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool GPShopsLayer::init() {
    if (!CCLayer::init())
        return false;

    setKeypadEnabled(true);

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto menu = CCMenu::create();

    auto title = CCLabelBMFont::create("Shops", "goldFont.fnt");
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

    auto shopSignBtnSpr = CCSprite::createWithSpriteFrameName("shopSign_001.png");
    auto shopSignBtn = CCMenuItemSpriteExtra::create(
        shopSignBtnSpr, this, menu_selector(GPShopsLayer::onShop1)
    );
    menu->addChild(shopSignBtn);

    this->addChild(menu);

    return true;
}

void GPShopsLayer::onClick(CCObject* sender) {
    auto scenePrev = CCTransitionFade::create(0.5f, GPShopsLayer::garageScene());
    CCDirector::sharedDirector()->replaceScene(scenePrev);
}

void GPShopsLayer::onShop1(CCObject* sender) {
    // Implement your onShop1 logic here
}