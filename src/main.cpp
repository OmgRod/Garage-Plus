#include <chrono>
#include <iostream>
#include <Geode/Geode.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/binding/GJGarageLayer.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/Notification.hpp>
#include "GPFeedbackLayer.hpp"
// #include "MyLayer.hpp"

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

        auto demons = Mod::get()->getSettingValue<bool>("demons");
        auto cp = Mod::get()->getSettingValue<bool>("cp");
        auto tapLockHint = Mod::get()->getSettingValue<bool>("no-lock-hint");
        auto topBtns = Mod::get()->getSettingValue<bool>("top-buttons");
        auto advStats = Mod::get()->getSettingValue<bool>("advanced-stats");
        auto feedback = Mod::get()->getSettingValue<bool>("feedback");

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        if (demons) {
            // Credits to Capeling for this code (Demons in Garage)

            auto demonIcon = CCSprite::create("GaragePlus_demonIcon.png"_spr);

            if (advStats) {
                auto menu = CCMenu::create();
                menu->setPosition({winSize.width - 18, winSize.height - 117});

                auto demonsBtn = CCMenuItemSpriteExtra::create(demonIcon, menu, menu_selector(MyLayer::demonInfo));
                demonsBtn->setID("demons-icon");
                menu->addChild(demonsBtn);

                CCDictionary* stats = GameStatsManager::sharedState()->m_playerStats;
                CCLabelBMFont* demonText = CCLabelBMFont::create(std::to_string(stats->valueForKey("5")->intValue()).c_str(), "bigFont.fnt");
                demonText->setPosition(CCPoint(menu->getPositionX() - 12, this->getChildByID("diamond-shards-label")->getPositionY() - 15));
                demonText->setScale(0.34);
                demonText->setAnchorPoint({1, 0.5});
                demonText->setID("demons-label");
                this->addChild(demonText);

                menu->setID("demons-icon");
                this->addChild(menu);
            } else {
                auto demonIconGD = CCSprite::createWithSpriteFrameName("GJ_demonIcon_001.png");

                demonIconGD->setPosition({winSize.width - 18, winSize.height - 117});
                demonIconGD->setAnchorPoint({0.5, 0.5});
                demonIconGD->setScale(0.5);
                demonIconGD->setID("demons-icon");

                CCDictionary* stats = GameStatsManager::sharedState()->m_playerStats;
                CCLabelBMFont* demonText = CCLabelBMFont::create(std::to_string(stats->valueForKey("5")->intValue()).c_str(), "bigFont.fnt");
                demonText->setPosition(CCPoint(demonIconGD->getPositionX() - 12, this->getChildByID("diamond-shards-label")->getPositionY() - 15));
                demonText->setScale(0.34);
                demonText->setAnchorPoint({1, 0.5});
                demonText->setID("demons-label");
                this->addChild(demonText);

                this->addChild(demonIconGD);
            }
        }

        if (advStats) {
            auto starsIcon = this->getChildByID("stars-icon");
            auto moonsIcon = this->getChildByID("moons-icon");

            auto menu = CCMenu::create();
            menu->setPosition({starsIcon->getPositionX(), this->getChildByID("stars-icon")->getPositionY()});
            menu->setID("clickable-stats");

            auto starsBtn = CCMenuItemSpriteExtra::create(starsIcon, menu, menu_selector(MyLayer::starsInfo));
            starsBtn->setID("stars-btn");
            menu->addChild(starsBtn);

            auto moonsBtn = CCMenuItemSpriteExtra::create(moonsIcon, menu, menu_selector(MyLayer::moonsInfo));
            moonsBtn->setPositionY(starsBtn->getPositionY() - 15);
            moonsBtn->setID("moons-btn");
            menu->addChild(moonsBtn);

            this->removeChildByID("stars-icon");
            this->removeChildByID("moons-icon");

            menu->setID("stars-moons-advstats");

            this->addChild(menu);
        }

        if (cp) {
            // Credits to Capeling for this code (CP in Garage)

            auto menu = CCMenu::create();
            menu->setPosition({0.f, 0.f});
            this->addChild(menu);  // Ensure the menu is added to the layer

            auto cpIconSprite = CCSprite::create("GaragePlus_cpIcon.png"_spr);
            auto cpIcon = CCMenuItemSpriteExtra::create(
                cpIconSprite,
                this,
                menu_selector(GJGarageLayerModified::refreshCP)  // Correctly bind the member function
            );

            if (demons || this->getChildByID("demons-icon")) {
                cpIcon->setPosition({winSize.width - 18, winSize.height - 132});
            } else {
                cpIcon->setPosition({winSize.width - 18, winSize.height - 117});
            }
            cpIcon->setAnchorPoint({0.5, 0.5});
            cpIcon->setID("cp-icon");
            menu->addChild(cpIcon);
            menu->setID("cp-menu");

            refreshCP(nullptr);
        }

        if (tapLockHint) {
            this->removeChildByID("tap-more-hint");

            if (topBtns) {

                // Shift elements down for space

                this->getChildByID("category-menu")->setPositionY(this->getChildByID("category-menu")->getPositionY() - (winSize.height / 12));
                this->getChildByID("floor-line")->setPositionY(this->getChildByID("floor-line")->getPositionY() - (winSize.height / 12));
                this->getChildByID("player-icon")->setPositionY(this->getChildByID("player-icon")->getPositionY() - (winSize.height / 12));
                if (Loader::get()->isModLoaded("weebify.separate_dual_icons")) {
                    this->getChildByID("player2-icon")->setPositionY(this->getChildByID("player2-icon")->getPositionY() - (winSize.height / 12));
                    log::debug("Separate Dual Icons mod loaded D:");
                }
                this->getChildByID("username-label")->setPositionY(this->getChildByID("username-label")->getPositionY() - (winSize.height / 12));
                try {
    // Retrieve the pointer to the "username-lock" child
    auto usernameLock = this->getChildByID("username-lock");
    
    // Check if the pointer is valid
    if (usernameLock == nullptr) {
        throw std::runtime_error("Error: username-lock not found");
    }
    
    // Retrieve the current position Y
    float currentPosY = usernameLock->getPositionY();
    
    // Calculate the new position Y
    float newPosY = currentPosY - (winSize.height / 12);
    
    // Set the new position Y
    usernameLock->setPositionY(newPosY);

} catch (const std::exception& e) {
    // Handle the error appropriately
    log::error("Exception caught: {}", e.what());
}

                // Top buttons bar

                auto buttonsMenu = CCMenu::create();
                buttonsMenu->setID("top-buttons");
                buttonsMenu->setPosition(CCPoint(winSize.width * 0.575, winSize.height * 0.925));
                buttonsMenu->setContentSize(CCPoint(winSize.width * 0.35, 0.f));
                buttonsMenu->setLayout(RowLayout::create());
                this->addChild(buttonsMenu);

                auto modSettingsIcon = CCSprite::create("GaragePlus_modSettingsBtn.png"_spr);
                auto modSettingsBtn = CCMenuItemSpriteExtra::create(modSettingsIcon, this, menu_selector(MyLayer::onModSettings));
                modSettingsBtn->setID("mod-settings");
                buttonsMenu->addChild(modSettingsBtn);

                auto gameSettingsIcon = CCSprite::create("GaragePlus_gameSettingsBtn.png"_spr);
                auto gameSettingsBtn = CCMenuItemSpriteExtra::create(gameSettingsIcon, this, menu_selector(MenuLayer::onOptions));
                gameSettingsBtn->setID("game-settings");
                buttonsMenu->addChild(gameSettingsBtn);

                if (feedback) {
                    auto feedbackIcon = CCSprite::create("GaragePlus_feedbackBtn.png"_spr);
                    auto feedbackBtn = CCMenuItemSpriteExtra::create(feedbackIcon, this, menu_selector(MyLayer::onFeedbackBtn));
                    feedbackBtn->setID("feedback");
                    buttonsMenu->addChild(feedbackBtn);
                }

                // auto creatorBtnIcon = CCSprite::create("GaragePlus_creatorBtn.png"_spr);
                // auto creatorBtn = CCMenuItemSpriteExtra::create(
                //     creatorBtnIcon, this, menu_selector(MyLayer::onDisabled) // MyLayer::onClick
                // );
                // creatorBtn->setID("creator");
                // buttonsMenu->addChild(creatorBtn);

                auto profileIcon = CCSprite::create("GaragePlus_profileBtn.png"_spr);
                auto profileBtn = CCMenuItemSpriteExtra::create(profileIcon, this, menu_selector(MenuLayer::onMyProfile));
                profileBtn->setID("profile");
                buttonsMenu->addChild(profileBtn);

                // auto demonIconsIcon = CCSprite::create("GaragePlus_demonKeysBtn.png"_spr);
                // auto demonIconsBtn = CCMenuItemSpriteExtra::create(demonIconsIcon, this, menu_selector(MyLayer::onDisabled));
                // demonIconsBtn->setID("demon-icons-btn");
                // buttonsMenu->addChild(demonIconsBtn);

                buttonsMenu->updateLayout();
            }
        }

        return true;
    }

void refreshCP(CCObject* sender) {
    // Check if demonIcon exists
    auto demonIcon = this->getChildByID("cp-menu")->getChildByID("cp-icon");
    this->removeChildByID("cp-label");
    if (!demonIcon) {
        log::debug("Error: 'cp-icon' not found.");
        return;
    }

    // Fetch user information from the server
    int accID = GJAccountManager::get()->m_accountID;
    if (accID != 0) {
        std::string url = "https://www.boomlings.com/database/getGJUserInfo20.php";
        std::string secret = "Wmfd2893gb7";
        std::string targetAccountID = std::to_string(accID);

        web::WebRequest request;
        request.bodyString("secret=" + secret + "&targetAccountID=" + targetAccountID);
        request.userAgent("");

        // Callback function for handling web request response
        m_fields->m_listener.bind([=](web::WebTask::Event* e) {
            if (web::WebResponse* res = e->getValue()) {
                if (res->ok()) {
                    std::string responseBody = res->string().unwrap();
                    size_t start_pos = responseBody.find(":8:");
                    if (start_pos != std::string::npos) {
                        size_t end_pos = responseBody.find(":", start_pos + 3);
                        if (end_pos != std::string::npos) {
                            std::string pointsStr = responseBody.substr(start_pos + 3, end_pos - start_pos - 3);
                            try {
                                int creatorPoints = std::stoi(pointsStr);
                                // Create and add CP label
                                auto cpText = CCLabelBMFont::create(pointsStr.c_str(), "bigFont.fnt");
                                cpText->setPosition({demonIcon->getPositionX() - 12.f, demonIcon->getPositionY()});
                                cpText->setScale(0.34);
                                cpText->setAnchorPoint({1, 0.5});
                                cpText->setID("cp-label");
                                this->addChild(cpText);
                                this->updateLayout();

                                // to all people in #dev-chat, i removed it
                                // and i un-abbreviated it
                                //  AND I BELIEVE I FIXED THE LOGGING STUFF
                                // im so stupid ngl

                                // geode::Notification::create("CP updated.", geode::NotificationIcon::Success, 2.5)->show();
                            } catch (const std::invalid_argument& e) {
                                log::error("Failed to convert creator points to integer: {}", e.what());
                                geode::Notification::create("An error occurred while updating Creator Points.", geode::NotificationIcon::Error, 2.5)->show();
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

        // Set filter for the listener
        m_fields->m_listener.setFilter(request.post(url));
    } else {
        log::debug("Invalid account ID.");
    }
    demonIcon->setVisible(true);
}
};