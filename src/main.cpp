#include <chrono>
#include <iostream>

#include <Geode/Geode.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/binding/GJGarageLayer.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/Notification.hpp>

#include <capeling.garage-stats-menu/include/StatsDisplayAPI.h>

#include "GPFeedbackLayer.hpp"
#include "GPSceneManager.hpp"

using namespace geode::prelude;

class $modify(GJGarageLayerModified, GJGarageLayer) {
    struct Fields {
        EventListener<web::WebTask> m_listener;
    };

    static void onModify(auto& self) {
        self.setHookPriority("GJGarageLayer::init", -1);
    }

    bool init() {
        if (!GJGarageLayer::init()) {
            return false;
        }

        auto cp = Mod::get()->getSettingValue<bool>("cp");
        auto tapLockHint = Mod::get()->getSettingValue<bool>("no-lock-hint");
        auto topBtns = Mod::get()->getSettingValue<bool>("top-buttons");
        auto feedback = Mod::get()->getSettingValue<bool>("feedback");
        auto demonKeys = Mod::get()->getSettingValue<bool>("demon-keys");

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        if (cp) {
            GJGarageLayerModified::refreshCP(nullptr, false);
        }

        if (demonKeys) {
            auto statMenu = this->getChildByID("capeling.garage-stats-menu/stats-menu");
            
            auto myStatItem = StatsDisplayAPI::getNewItem(
                "demon-keys"_spr,
                CCSprite::createWithSpriteFrameName("GJ_bigKey_001.png"),
                GameStatsManager::sharedState()->getStat("21"),
                0.375
            );

            if (statMenu) {
                statMenu->addChild(myStatItem);
                statMenu->updateLayout();
            }
        }

        if (tapLockHint) {
            this->removeChildByID("tap-more-hint");

            if (topBtns) {

                this->getChildByID("category-menu")->setPositionY(this->getChildByID("category-menu")->getPositionY() - (winSize.height / 12));
                this->getChildByID("floor-line")->setPositionY(this->getChildByID("floor-line")->getPositionY() - (winSize.height / 12));
                this->getChildByID("player-icon")->setPositionY(this->getChildByID("player-icon")->getPositionY() - (winSize.height / 12));
                if (Loader::get()->isModLoaded("weebify.separate_dual_icons")) {
                    this->getChildByID("player2-icon")->setPositionY(this->getChildByID("player2-icon")->getPositionY() - (winSize.height / 12));
                    this->getChildByID("player-buttons-menu")->setPositionY(this->getChildByID("player-buttons-menu")->getPositionY() - (winSize.height / 12));
                    this->getChildByID("arrow-1")->setPositionY(this->getChildByID("arrow-1")->getPositionY() - (winSize.height / 12));
                    this->getChildByID("arrow-2")->setPositionY(this->getChildByID("arrow-2")->getPositionY() - (winSize.height / 12));
                    log::debug("Separate Dual Icons mod loaded D:");
                }
                this->getChildByID("username-label")->setPositionY(this->getChildByID("username-label")->getPositionY() - (winSize.height / 12));
                
                auto usernameLock = this->getChildByID("username-lock");

                if (usernameLock != nullptr) {
                    float currentPosY = usernameLock->getPositionY();
                    float newPosY = currentPosY - (winSize.height / 12);
                    usernameLock->setPositionY(newPosY);
                }

                auto buttonsMenu = CCMenu::create();
                buttonsMenu->setID("top-buttons");
                buttonsMenu->setPosition(CCPoint(winSize.width * 0.575, winSize.height * 0.925));
                buttonsMenu->setContentSize(CCPoint(winSize.width * 0.35, 0.f));
                buttonsMenu->setLayout(RowLayout::create());
                this->addChild(buttonsMenu);

                auto modSettingsIcon = CCSprite::create("GaragePlus_modSettingsBtn.png"_spr);
                auto modSettingsBtn = CCMenuItemSpriteExtra::create(modSettingsIcon, this, menu_selector(GPSceneManager::onModSettings));
                modSettingsBtn->setID("mod-settings");
                buttonsMenu->addChild(modSettingsBtn);

                auto gameSettingsIcon = CCSprite::create("GaragePlus_gameSettingsBtn.png"_spr);
                auto gameSettingsBtn = CCMenuItemSpriteExtra::create(gameSettingsIcon, this, menu_selector(MenuLayer::onOptions));
                gameSettingsBtn->setID("game-settings");
                buttonsMenu->addChild(gameSettingsBtn);

                if (feedback) {
                    auto feedbackIcon = CCSprite::create("GaragePlus_feedbackBtn.png"_spr);
                    auto feedbackBtn = CCMenuItemSpriteExtra::create(feedbackIcon, this, menu_selector(GPSceneManager::onFeedbackBtn));
                    feedbackBtn->setID("feedback");
                    buttonsMenu->addChild(feedbackBtn);
                }

                /* auto creatorBtnIcon = CCSprite::create("GaragePlus_creatorBtn.png"_spr);
                auto creatorBtn = CCMenuItemSpriteExtra::create(
                    creatorBtnIcon, this, menu_selector(MenuLayer::onCreator)
                );
                creatorBtn->setID("creator");
                buttonsMenu->addChild(creatorBtn); */

                auto profileIcon = CCSprite::create("GaragePlus_profileBtn.png"_spr);
                auto profileBtn = CCMenuItemSpriteExtra::create(profileIcon, this, menu_selector(MenuLayer::onMyProfile));
                profileBtn->setID("profile");
                buttonsMenu->addChild(profileBtn);

                buttonsMenu->updateLayout();
            }
        }

        return true;
    }

    void refreshCPWrapper(CCObject* sender) {
        this->refreshCP(sender, true);
    }

    void refreshCP(CCObject* sender, bool notifySuccess = false) {

        auto statMenu = this->getChildByID("capeling.garage-stats-menu/stats-menu");
        statMenu->removeChildByID("omgrod.garage_plus/creator-points-container");

        int accID = GJAccountManager::get()->m_accountID;
        if (accID != 0) {
            std::string url = "https://www.boomlings.com/database/getGJUserInfo20.php";
            std::string secret = "Wmfd2893gb7";
            std::string targetAccountID = std::to_string(accID);

            web::WebRequest request;
            request.bodyString("secret=" + secret + "&targetAccountID=" + targetAccountID);
            request.userAgent("");

            m_fields->m_listener.bind([=](web::WebTask::Event* e) {
                if (web::WebResponse* res = e->getValue()) {
                    if (res->ok()) {
                        std::string responseBody = res->string().unwrap();
                        size_t start_pos = responseBody.find(":8:");
                        if (start_pos != std::string::npos) {
                            size_t end_pos = responseBody.find(":", start_pos + 3);
                            if (end_pos != std::string::npos) {
                                auto myStatItem = StatsDisplayAPI::getNewItem(
                                    "creator-points"_spr, 
                                    CCSprite::create("GaragePlus_cpIcon.png"_spr), 
                                    std::stoi(responseBody.substr(start_pos + 3, end_pos - start_pos - 3)), 
                                    1.f
                                );

                                if (statMenu) {
                                    auto btn = CCMenuItemSpriteExtra::create(
                                        CCSprite::create("GaragePlus_cpIcon.png"_spr),
                                        this,
                                        menu_selector(GJGarageLayerModified::refreshCPWrapper)
                                    );
                                    btn->setID("creator-points-icon");

                                    myStatItem->removeChildByID("creator-points-icon");
                                    myStatItem->addChild(btn);
                                    statMenu->addChild(myStatItem);
                                    statMenu->updateLayout();

                                    if (notifySuccess) {
                                        geode::Notification::create("Creator Points successfully updated.", geode::NotificationIcon::Success, 2.5)->show();
                                    }
                                }
                            } else {
                                log::error("Failed to find ':' after ':8:' in response: {}", responseBody);
                                geode::Notification::create("An error occurred while updating Creator Points.", geode::NotificationIcon::Error, 2.5)->show();
                            }
                        } else {
                            log::error("Failed to find ':8:' in response: {}", responseBody);
                            geode::Notification::create("An error occurred while updating Creator Points.", geode::NotificationIcon::Error, 2.5)->show();
                        }
                    } else {
                        log::error("Request failed with status code: {}", res->code());
                        geode::Notification::create("An error occurred while updating Creator Points.", geode::NotificationIcon::Error, 2.5)->show();
                    }
                } else if (e->isCancelled()) {
                    log::error("The request was cancelled.");
                    geode::Notification::create("An error occurred while updating Creator Points.", geode::NotificationIcon::Error, 2.5)->show();
                }
            });

            m_fields->m_listener.setFilter(request.post(url));
        } else {
            log::debug("Invalid account ID.");
        }
    }
};
