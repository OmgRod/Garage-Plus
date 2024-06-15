#include <chrono>
#include <iostream>
#include <Geode/Geode.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/binding/GJGarageLayer.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/Notification.hpp>

using namespace geode::prelude;

class GPFeedbackLayer : public CCLayer {
public:
    static CCScene* scene() {
        auto scene = CCScene::create();
        scene->addChild(GPFeedbackLayer::create());
        return scene;
    }

    static CCScene* garageScene() {
        auto scene = CCScene::create();
        scene->addChild(GJGarageLayer::scene());
        return scene;
    }

    static GPFeedbackLayer* create() {
        GPFeedbackLayer* ret = new GPFeedbackLayer();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    virtual void keyBackClicked() {
        onClick(nullptr);
    }

    void onFeedbackClick(CCObject* sender) {
        auto descInput = dynamic_cast<CCTextInputNode*>(this->getChildByID("descInput"));
        if (!descInput)
            return;

        std::string feedback = descInput->getString();
        std::string username = "Anonymous";

        if (GJAccountManager::get()->m_accountID != 0) {
            username = std::string(GJAccountManager::get()->m_username.c_str());
        }

        std::string payload = "{\"content\": \"" + username + " said:\\n\\n" + feedback + "\",\"embeds\": null,\"attachments\": []}";

        web::WebRequest request;
        // request.method("POST");
        request.header("Content-Type", "application/json");
        request.bodyString(payload);
        request.timeout(std::chrono::seconds(30));

        std::string url = "https://canary.discord.com/api/webhooks/1225440650988884029/NPsyWQuLi6x3GB-DymtlIvVIlzC0Gm0qocSxbq9wILUhnjZsGR5Rc6YEY2mEtdpvQ0x_";

        m_listener.bind([this](web::WebTask::Event* e) {
            if (web::WebResponse* res = e->getValue()) {
                if (res->ok()) {
                    std::cout << "Feedback sent successfully!" << std::endl;
                    geode::createQuickPopup(
                        "Garage Plus",
                        "Feedback sent successfully!",
                        "Exit", nullptr,
                        [this](auto, bool btn1) {
                            this->onClick(nullptr);
                        }
                    );
                } else {
                    std::cerr << "Request failed with status code: " << res->code() << std::endl;
                }
            } else if (web::WebProgress* progress = e->getProgress()) {
                // std::cout << "Progress: " << progress->downloadProgress() << std::endl;
            } else if (e->isCancelled()) {
                std::cerr << "The request was cancelled... So sad :(" << std::endl;
            }
        });

        m_listener.setFilter(request.post(url));
    }

    bool init() {
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

        auto title = CCLabelBMFont::create("Feedback", "goldFont.fnt");
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

    void onClick(CCObject* sender) {
        auto scenePrev = CCTransitionFade::create(0.5f, GPFeedbackLayer::garageScene());
        CCDirector::sharedDirector()->replaceScene(scenePrev);
    }

    void onInfoClick(CCObject* sender) {
        FLAlertLayer::create(
    nullptr, // No delegate needed
    "Garage Plus", // Title
    "By submitting your feedback, you agree to the collection and transmission of your username and feedback to a private Discord server accessible only to the developer of this mod, OmgRod. This data will be used solely for improving the mod.\n\nYou may disable the feedback button in this mod's settings at any time to add extra space.\n\nYour data will be stored securely and will not be shared with third parties.\n\nThank you for your feedback!\n\n-OmgRod", // Description
    "OK", // Button 1 (OK)
    nullptr, // Button 2 (no second button)
    400.0f, // Width (adjust as necessary)
    true, // Scrolling enabled
    300.0f, // Height (adjust as necessary)
    1.0f // Text scale (adjust as necessary)
)->show();

    }

private:
    geode::EventListener<web::WebTask> m_listener;
};

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
    std::cerr << "Exception caught: " << e.what() << std::endl;
}

                // Top buttons bar

                auto buttonsMenu = CCMenu::create();
                buttonsMenu->setID("top-buttons");
                buttonsMenu->setPosition(CCPoint(winSize.width * 0.575, winSize.height * 0.925));
                buttonsMenu->setContentSize(CCPoint(winSize.width * 0.35, 0.f));
                buttonsMenu->setLayout(RowLayout::create());
                this->addChild(buttonsMenu);

                auto modSettingsIcon = CCSprite::create("GaragePlus_settingsBtn.png"_spr);
                auto modSettingsBtn = CCMenuItemSpriteExtra::create(modSettingsIcon, this, menu_selector(MyLayer::onModSettings));
                modSettingsBtn->setID("mod-settings");
                buttonsMenu->addChild(modSettingsBtn);

                if (feedback) {
                    auto feedbackIcon = CCSprite::create("GaragePlus_feedbackBtn.png"_spr);
                    auto feedbackBtn = CCMenuItemSpriteExtra::create(feedbackIcon, this, menu_selector(MyLayer::onFeedbackBtn));
                    feedbackBtn->setID("feedback");
                    buttonsMenu->addChild(feedbackBtn);
                }

                auto creatorBtnIcon = CCSprite::create("GaragePlus_creatorBtn.png"_spr);
                auto creatorBtn = CCMenuItemSpriteExtra::create(
                    creatorBtnIcon, this, menu_selector(MyLayer::onDisabled) // MyLayer::onClick
                );
                creatorBtn->setID("creator");
                buttonsMenu->addChild(creatorBtn);

                auto profileIcon = CCSprite::create("GaragePlus_profileBtn.png"_spr);
                auto profileBtn = CCMenuItemSpriteExtra::create(profileIcon, this, menu_selector(MenuLayer::onMyProfile));
                profileBtn->setID("profile");
                buttonsMenu->addChild(profileBtn);

                // auto demonIconsIcon = CCSprite::create("GaragePlus_demonKeysBtn.png"_spr);
                // auto demonIconsBtn = CCMenuItemSpriteExtra::create(demonIconsIcon, this, menu_selector(nullptr));
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
        std::cout << "Error: 'cp-icon' not found." << std::endl;
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

                                geode::Notification::create("CP updated.", geode::NotificationIcon::Success, 2.5)->show();
                            } catch (const std::invalid_argument& e) {
                                std::cerr << "Failed to convert creator points to integer: " << e.what() << std::endl;
                                geode::Notification::create("An error occurred while updating CP.", geode::NotificationIcon::Error, 2.5)->show();
                            }
                        } else {
                            std::cerr << "Failed to find ':' after ':8:' in response: " << responseBody << std::endl;
                            geode::Notification::create("An error occurred while updating CP.", geode::NotificationIcon::Error, 2.5)->show();
                        }
                    } else {
                        std::cerr << "Failed to find ':8:' in response: " << responseBody << std::endl;
                        geode::Notification::create("An error occurred while updating CP.", geode::NotificationIcon::Error, 2.5)->show();
                    }
                } else {
                    std::cerr << "Request failed with status code: " << res->code() << std::endl;
                    geode::Notification::create("An error occurred while updating CP.", geode::NotificationIcon::Error, 2.5)->show();
                }
            } else if (e->isCancelled()) {
                std::cerr << "The request was cancelled." << std::endl;
                geode::Notification::create("An error occurred while updating CP.", geode::NotificationIcon::Error, 2.5)->show();
            }
        });

        // Set filter for the listener
        m_fields->m_listener.setFilter(request.post(url));
    } else {
        std::cout << "Invalid account ID." << std::endl;
    }
    demonIcon->setVisible(true);
}
};