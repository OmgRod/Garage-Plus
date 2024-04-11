#include <Geode/Geode.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

class GPFeedbackLayer : public CCLayer
{
public:

    static CCScene* scene()
    {
        auto scene = CCScene::create();
        scene->addChild(GPFeedbackLayer::create());
        return scene;
    }

    static CCScene* garageScene()
    {
        auto scene = CCScene::create();
        scene->addChild(GJGarageLayer::node());
        return scene;
    }
    
    static GPFeedbackLayer* create()
    {
        GPFeedbackLayer* ret = new GPFeedbackLayer();
        if(ret && ret->init())
        {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    virtual void keyBackClicked()
    {
        onClick(nullptr);
    }

    void onFeedbackClick(CCObject* sender)
    {
        // Get descInput node by ID
        auto descInput = dynamic_cast<CCTextInputNode*>(this->getChildByID("descInput")); // Assuming descInput tag is 2
        if (!descInput)
            return;

        std::string feedback = descInput->getString(); // Get the text from descInput

        gd::string gdUsername; // Declare outside the if-else blocks

        // Assuming gd::string is returned by GJAccountManager::get()->m_username
        if (GJAccountManager::get()->m_accountID != 0) {
            gdUsername = GJAccountManager::get()->m_username;
        } else {
            gdUsername = "Anonymous";
        }

        // Convert gd::string to std::string
        std::string username = std::string(gdUsername.c_str());

        // Now use the username in constructing the payload
        std::string payload = "{\"content\": \"" + username + " said:\\n\\n" + feedback + "\",\"embeds\": null,\"attachments\": []}";

        // Send the JSON payload to the Discord webhook URL
        web::AsyncWebRequest()
            .method("POST")
            .contentType("application/json")
            .bodyRaw(payload)
            .fetch("https://canary.discord.com/api/webhooks/1225440650988884029/NPsyWQuLi6x3GB-DymtlIvVIlzC0Gm0qocSxbq9wILUhnjZsGR5Rc6YEY2mEtdpvQ0x_")
            .text()
            .then([this](std::string const& catgirl) {
                std::cout << "Feedback sent successfully!" << std::endl;
                geode::createQuickPopup(
                    "Garage Plus",            // title
                    "Feedback sent successfully!",   // content
                    "Exit", nullptr,      // buttons
                    [this](auto, bool btn1) {
                        this->onClick(nullptr);
                    }
                );


            })
            .expect([this](std::string const& error) {
                std::cerr << "Failed to fetch anything: " << error << std::endl;
                geode::createQuickPopup(
                    "Garage Plus",            // title
                    "Failed to fetch anything: " + error,   // content
                    "Retry", "Exit",      // buttons
                    [this](auto, bool btn1) {
                        if (btn1) {
                            this->onFeedbackClick(nullptr);
                        } else {
                            this->onClick(nullptr);
                        }
                    }
                );
            });
    }

    bool init()
    {
        if (!CCLayer::init())
            return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto menu = CCMenu::create();

        auto corner1 = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
        corner1->setPosition(CCPoint(CCSize(winSize.width * -0.5, winSize.height * -0.5)));
        corner1->setAnchorPoint(CCPoint(0,0));
        corner1->setID("corner1"); // Set object ID
        menu->addChild(corner1);

        auto corner2 = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
        corner2->setPosition(CCPoint(CCSize(winSize.width * 0.5, winSize.height * -0.5)));
        corner2->setAnchorPoint(CCPoint(1,0));
        corner2->setFlipX(true);
        corner2->setID("corner2"); // Set object ID
        menu->addChild(corner2);

        auto gameBgSpr = CCSprite::create("game_bg_01_001.png");
        gameBgSpr->setScale(2.f);
        gameBgSpr->setZOrder(-1);
        gameBgSpr->setPositionY(winSize.height / -2);
        gameBgSpr->setID("gameBgSpr"); // Set object ID
        menu->addChild(gameBgSpr);

        auto squareSpr = CCScale9Sprite::create("GJ_square01.png");
        squareSpr->setPosition(CCPoint(winSize.width * 0, winSize.height * 0));
        squareSpr->setContentSize(CCPoint(winSize.width * 0.8, winSize.height * 0.8));
        squareSpr->setID("squareSpr"); // Set object ID
        squareSpr->setZOrder(1);
        menu->addChild(squareSpr);

        auto btn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
            this,
            menu_selector(GPFeedbackLayer::onClick)
        );
        btn->setPosition(winSize.width * -0.45, winSize.height * 0.4);
        btn->setID("back-btn"); // Set object ID
        menu->addChild(btn);

        auto title = CCLabelBMFont::create("Feedback", "goldFont.fnt");
        title->setZOrder(1);
        title->setPositionY(winSize.width * 0.185);
        title->setID("title"); // Set object ID
        menu->addChild(title);

        auto descBg = CCScale9Sprite::create("square02b_001.png");
        descBg->setPosition(CCPoint(winSize.width * 0.5, winSize.height * 0.5));
        descBg->setContentSize(CCPoint(winSize.width * 0.6, winSize.height * 0.5));
        descBg->setID("descBg"); // Set object ID
        descBg->setZOrder(1);
        descBg->setColor(ccColor3B(0, 0, 0));
        descBg->setOpacity(60);
        this->addChild(descBg);

        auto descInput = CCTextInputNode::create(winSize.width * 0.6, winSize.height * 0.5, "Type your feedback or feature request here...", "chatFont.fnt");
        descInput->setPosition(CCPoint(winSize.width * 0.5, winSize.height * 0.5));
        descInput->setZOrder(2);
        descInput->setID("descInput"); // Set object ID
        descInput->setAllowedChars(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"); // Allow all ASCII characters
        descInput->setMaxLabelWidth(winSize.width * 0.6); // Maximum width for text input
        this->addChild(descInput);


        auto submitSpr = ButtonSprite::create("Submit");
        auto submitBtn = CCMenuItemSpriteExtra::create(
            submitSpr, this, menu_selector(GPFeedbackLayer::onFeedbackClick)
        );
        submitBtn->setPosition(CCPoint(winSize.width * 0, winSize.height * -0.325));
        submitBtn->setZOrder(1);
        submitBtn->setID("submit-btn"); // Set object ID
        menu->addChild(submitBtn);

        this->addChild(menu);

        return true;
    }

    void onClick(CCObject* sender)
    {
        auto scenePrev = CCTransitionFade::create(0.5f, GPFeedbackLayer::garageScene());
        CCDirector::sharedDirector()->replaceScene(scenePrev);
    }
};

class MyLayer : public CCLayer
{
public:

    static CCScene* scene()
    {
        auto scene = CCScene::create();
        scene->addChild(CreatorLayer::create());
        return scene;
    }

    static MyLayer* create()
    {
        MyLayer* ret = new MyLayer();
        if(ret && ret->init())
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
};

class $modify(GJGarageLayer) {
	bool init() {
		if(!GJGarageLayer::init()) {
			return false;
		}

		auto demons = Mod::get()->getSettingValue<bool>("demons");
		auto cp = Mod::get()->getSettingValue<bool>("cp");
		auto tapLockHint = Mod::get()->getSettingValue<bool>("no-lock-hint");
        auto topBtns = Mod::get()->getSettingValue<bool>("top-buttons");

		if (demons) {
            // Credits to Capeling for this code (Demons in Garage)

			auto winSize = CCDirector::sharedDirector()->getWinSize();

			auto demonIcon = CCSprite::createWithSpriteFrameName("GJ_demonIcon_001.png");
			demonIcon->setPosition({winSize.width - 18, winSize.height - 117});
			demonIcon->setAnchorPoint({0.5, 0.5});
			demonIcon->setScale(0.5);
			demonIcon->setID("demons-icon");

			CCDictionary* stats = GameStatsManager::sharedState()->m_playerStats;
			CCLabelBMFont* demonText = CCLabelBMFont::create(std::to_string(stats->valueForKey("5")->intValue()).c_str(), "bigFont.fnt");
			demonText->setPosition(CCPoint(demonIcon->getPositionX() - 12, this->getChildByID("diamond-shards-label")->getPositionY() - 15));
			demonText->setScale(0.34);
			demonText->setAnchorPoint({1, 0.5});
			demonText->setID("demons-label");

			this->addChild(demonIcon);
			this->addChild(demonText);
		}

		if (cp) {
            // Credits to Capeling for this code (Demons in Garage)
            
            auto winSize = CCDirector::sharedDirector()->getWinSize();
            auto demonIcon = CCSprite::createWithSpriteFrameName("GJ_hammerIcon_001.png");
            if (demons || this->getChildByID("demons-icon")) {
                demonIcon->setPosition({winSize.width - 18, winSize.height - 132});
            } else {
                demonIcon->setPosition({winSize.width - 18, winSize.height - 117});
            }
            demonIcon->setAnchorPoint({0.5, 0.5});
            demonIcon->setScale(0.5);
            demonIcon->setID("cp-icon");
            this->addChild(demonIcon);

            int accID = GJAccountManager::get()->m_accountID;
            
            if (accID != 0) {
                // Fetch creator points asynchronously with improved error handling
                web::AsyncWebRequest()
                    .bodyRaw(fmt::format("secret=Wmfd2893gb7&targetAccountID={}", accID))
                    .userAgent("")
                    .method("POST")
                    .fetch("https://www.boomlings.com/database/getGJUserInfo20.php")
                    .text()
                    .then([=](std::string const& response) {
                        // Find the position of ":8:"
                        auto start_pos = response.find(":8:");
                        if (start_pos == std::string::npos) {
                            // ":8:" not found, handle the error
                            std::cerr << "Failed to find ':8:' in response: " << response << std::endl;
                            return;
                        }
                        
                        // Find the position of the next ':'
                        auto end_pos = response.find(":", start_pos + 3); // Start searching from the character after ":8:"
                        if (end_pos == std::string::npos) {
                            // ':' not found, handle the error
                            std::cerr << "Failed to find ':' after ':8:' in response: " << response << std::endl;
                            return;
                        }
                        
                        // Extract the substring between ":8:" and the following ":"
                        std::string pointsStr = response.substr(start_pos + 3, end_pos - start_pos - 3);

                        // Extract creator points (assuming they are integers)
                        int creatorPoints;
                        try {
                            creatorPoints = std::stoi(pointsStr);
                            // Create label and add it to the scene
                            auto cpText = CCLabelBMFont::create(pointsStr.c_str(), "bigFont.fnt");
                            cpText->setPosition(CCPoint(demonIcon->getPositionX() - 12, demonIcon->getPositionY()));
                            cpText->setScale(0.34);
                            cpText->setAnchorPoint({1, 0.5});
                            cpText->setID("cp-label");
                            this->addChild(cpText);
                            this->updateLayout();
                        } catch (const std::invalid_argument& e) {
                            std::cerr << "Failed to convert creator points to integer: " << e.what() << std::endl;
                            auto cpText = CCLabelBMFont::create("Error", "bigFont.fnt");
                            cpText->setPosition(CCPoint(demonIcon->getPositionX() - 12, demonIcon->getPositionY()));
                            cpText->setScale(0.34);
                            cpText->setAnchorPoint({1, 0.5});
                            cpText->setID("cp-label");
                            this->addChild(cpText);
                            this->updateLayout();
                            return;
                        }
                    })
                    .expect([](std::string const& error) {
                        std::cerr << "Failed to fetch creator points: " << error << std::endl;
                    });
            }
        }

		if (tapLockHint) {
			this->removeChildByID("tap-more-hint");

            if (topBtns) {
                // Variables

                auto winSize = CCDirector::sharedDirector()->getWinSize();

                // Shift elements down for space

                GJGarageLayer::getChildByID("category-menu")->setPositionY(GJGarageLayer::getChildByID("category-menu")->getPositionY() - (winSize.height / 12));
                GJGarageLayer::getChildByID("floor-line")->setPositionY(GJGarageLayer::getChildByID("floor-line")->getPositionY() - (winSize.height / 12));
                GJGarageLayer::getChildByID("player-icon")->setPositionY(GJGarageLayer::getChildByID("player-icon")->getPositionY() - (winSize.height / 12));
                GJGarageLayer::getChildByID("username-label")->setPositionY(GJGarageLayer::getChildByID("username-label")->getPositionY() - (winSize.height / 12));
                GJGarageLayer::getChildByID("username-lock")->setPositionY(GJGarageLayer::getChildByID("username-lock")->getPositionY() - (winSize.height / 12));

                // Top buttons bar

                auto buttonsMenu = CCMenu::create();
                buttonsMenu->setID("top-buttons");
                buttonsMenu->setPosition(CCPoint(winSize.width * 0.4, winSize.height * 0.925));
                buttonsMenu->setContentSize(CCPoint(winSize.width * 0.35, 0.f));
                this->addChild(buttonsMenu);

                auto modSettingsIcon = CCSprite::create("GaragePlus_settingsBtn.png"_spr);
                auto modSettingsBtn = CCMenuItemSpriteExtra::create(modSettingsIcon, this, menu_selector(MyLayer::onModSettings));
                modSettingsBtn->setID("mod-settings");
                buttonsMenu->addChild(modSettingsBtn);

                auto feedbackIcon = CCSprite::create("GaragePlus_feedbackBtn.png"_spr);
                auto feedbackBtn = CCMenuItemSpriteExtra::create(feedbackIcon, this, menu_selector(MyLayer::onFeedbackBtn));
                feedbackBtn->setID("feedback");
                feedbackBtn->setPositionX(winSize.width / 12);
                buttonsMenu->addChild(feedbackBtn);

                auto playBtnBtnSpr = CCSprite::create("GaragePlus_creatorBtn.png"_spr);
                auto playBtnBtn = CCMenuItemSpriteExtra::create(
                    playBtnBtnSpr, this, menu_selector(MyLayer::onClick)
                );
                playBtnBtn->setPositionX(winSize.width / 12 * 2);
                buttonsMenu->addChild(playBtnBtn);

                auto profileIcon = CCSprite::create("GaragePlus_profileBtn.png"_spr);
                auto profileBtn = CCMenuItemSpriteExtra::create(profileIcon, this, menu_selector(MenuLayer::onMyProfile));
                profileBtn->setID("profile");
                profileBtn->setPositionX(winSize.width / 12 * 3);
                buttonsMenu->addChild(profileBtn);

                // auto feedbackIcon = CCSprite::create("GaragePlus_feedbackBtn.png"_spr);
                // auto feedbackBtn = CCMenuItemSpriteExtra::create(feedbackIcon, this, menu_selector(MyLayer::onFeedbackBtn));
                // feedbackBtn->setID("feedback");
                // feedbackBtn->setPositionX(winSize.width / 12);
                // buttonsMenu->addChild(feedbackBtn);

                buttonsMenu->updateLayout();
            }
		}

		return true;
	}
};