#include <chrono>
#include <iostream>
#include <Geode/Geode.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/binding/GJGarageLayer.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/Notification.hpp>
#include "GPFeedbackLayer.hpp"

using namespace geode::prelude;

CCScene* GPFeedbackLayer::scene() {
    auto scene = CCScene::create();
    scene->addChild(GPFeedbackLayer::create());
    return scene;
}

CCScene* GPFeedbackLayer::garageScene() {
    auto scene = CCScene::create();
    scene->addChild(GJGarageLayer::scene());
    return scene;
}

GPFeedbackLayer* GPFeedbackLayer::create() {
    GPFeedbackLayer* ret = new GPFeedbackLayer();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void GPFeedbackLayer::keyBackClicked() {
    onClick(nullptr);
}

void GPFeedbackLayer::onFeedbackClick(CCObject* sender) {
    auto descInput = dynamic_cast<CCTextInputNode*>(this->getChildByID("descInput"));
    if (!descInput)
        return;

    std::string feedback = descInput->getString();
    std::string username = "Anonymous";

    if (GJAccountManager::get()->m_accountID != 0) {
        username = std::string(GJAccountManager::get()->m_username.c_str());
    }

    if (feedback.empty()) {
        log::error("Feedback cannot be empty.");
        geode::createQuickPopup(
            "Garage Plus",
            "Feedback cannot be empty.",
            "OK", nullptr,
            nullptr // No action on Exit button click
        );
        return;
    }

    std::string payload = "{\"username\": \"" + username + "\", \"feedback\": \"" + feedback + "\"}";

    web::WebRequest request;
    request.header("Content-Type", "application/json");
    request.bodyString(payload);
    request.timeout(std::chrono::seconds(30));

    std::string url = "https://script.google.com/macros/s/AKfycbzenL-ibrk3BxARfFrhSHW70mlt-jV_O7NNbOqq2dlO6vzsiSQ8gzvpsi7kxtMfvDgl/exec"; // Replace with your Google Apps Script web app URL

    m_listener.bind([this](web::WebTask::Event* e) {
        if (web::WebResponse* res = e->getValue()) {
            if (res->ok()) {
                log::debug("Feedback sent successfully!");
                geode::createQuickPopup(
                    "Garage Plus",
                    "Feedback sent successfully!",
                    "Exit", nullptr,
                    [this](auto, bool btn1) {
                        this->onClick(nullptr);
                    }
                );
            } else {
                log::error("Request failed with status code: {}", res->code());
            }
        } else if (web::WebProgress* progress = e->getProgress()) {
            // log::debug("Progress: ", progress->downloadProgress());
        } else if (e->isCancelled()) {
            log::error("The request was cancelled... So sad :(");
        }
    });

    m_listener.setFilter(request.post(url));
}

bool GPFeedbackLayer::init() {
    if (!CCLayer::init())
        return false;

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto menu = CCMenu::create();

    auto corner1 = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    corner1->setPosition(CCPoint(winSize.width * -0.5, winSize.height * -0.5));
    corner1->setAnchorPoint(CCPoint(0, 0));
    corner1->setID("corner1");
    menu->addChild(corner1);

    auto corner2 = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    corner2->setPosition(CCPoint(winSize.width * 0.5, winSize.height * -0.5));
    corner2->setAnchorPoint(CCPoint(1, 0));
    corner2->setFlipX(true);
    corner2->setID("corner2");
    menu->addChild(corner2);

    auto gameBgSpr = CCSprite::create("game_bg_01_001.png");
    gameBgSpr->setScale(2.f);
    gameBgSpr->setZOrder(-1);
    gameBgSpr->setPositionY(winSize.height / -2);
    gameBgSpr->setID("gameBgSpr");
    menu->addChild(gameBgSpr);

    auto squareSpr = CCScale9Sprite::create("GJ_square01.png");
    squareSpr->setPosition(CCPoint(winSize.width * 0, winSize.height * 0));
    squareSpr->setContentSize(CCPoint(winSize.width * 0.8, winSize.height * 0.8));
    squareSpr->setID("squareSpr");
    squareSpr->setZOrder(1);
    menu->addChild(squareSpr);

    auto backBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
        this,
        menu_selector(GPFeedbackLayer::onClick)
    );
    backBtn->setPosition(winSize.width * -0.45, winSize.height * 0.4);
    backBtn->setID("back-btn");
    menu->addChild(backBtn);

    auto infoBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"),
        this,
        menu_selector(GPFeedbackLayer::onInfoClick)
    );
    infoBtn->setPosition(winSize.width * 0.45, winSize.height * 0.45);
    infoBtn->setID("info-btn");
    menu->addChild(infoBtn);

    auto title = CCLabelBMFont::create("Garage Plus Feedback", "goldFont.fnt");
    title->setZOrder(1);
    title->setPositionY(winSize.width * 0.185);
    title->setID("title");
    menu->addChild(title);

    auto descBg = CCScale9Sprite::create("square02b_001.png");
    descBg->setPosition(CCPoint(winSize.width * 0.5, winSize.height * 0.5));
    descBg->setContentSize(CCPoint(winSize.width * 0.6, winSize.height * 0.5));
    descBg->setID("descBg");
    descBg->setZOrder(1);
    descBg->setColor(ccColor3B(0, 0, 0));
    descBg->setOpacity(60);
    this->addChild(descBg);

    auto descInput = CCTextInputNode::create(winSize.width * 0.6, winSize.height * 0.5, "Type your feedback or feature request here...", "chatFont.fnt");
    descInput->setPosition(CCPoint(winSize.width * 0.5, winSize.height * 0.5));
    descInput->setZOrder(2);
    descInput->setID("descInput");
    descInput->setAllowedChars(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
    descInput->setMaxLabelWidth(winSize.width * 0.6);
    this->addChild(descInput);

    auto submitSpr = ButtonSprite::create("Submit");
    auto submitBtn = CCMenuItemSpriteExtra::create(
        submitSpr, this, menu_selector(GPFeedbackLayer::onFeedbackClick)
    );
    submitBtn->setPosition(CCPoint(winSize.width * 0, winSize.height * -0.325));
    submitBtn->setZOrder(1);
    submitBtn->setID("submit-btn");
    menu->addChild(submitBtn);

    this->addChild(menu);
    return true;
}

void GPFeedbackLayer::onClick(CCObject* sender) {
    auto scenePrev = CCTransitionFade::create(0.5f, GPFeedbackLayer::garageScene());
    CCDirector::sharedDirector()->replaceScene(scenePrev);
}

void GPFeedbackLayer::onInfoClick(CCObject* sender) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    FLAlertLayer::create(
        nullptr, // No delegate needed
        "Garage Plus", // Title
        "By submitting your feedback, you agree to the collection and transmission of your username and feedback to a private Discord server accessible only to the developer of this mod, OmgRod. Your feedback will be used solely for the purpose of improving Garage Plus and will not be shared with any third parties. Thank you for your contribution!", // Message body
        "OK", nullptr // OK button text, no cancel button, no delegate
    )->show();
}