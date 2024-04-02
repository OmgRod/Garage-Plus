#include <Geode/Geode.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

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
};

class $modify(GJGarageLayer) {
	bool init() {
		if(!GJGarageLayer::init()) {
			return false;
		}

		auto demons = Mod::get()->getSettingValue<bool>("demons");
		auto cp = Mod::get()->getSettingValue<bool>("cp");
		auto tapLockHint = Mod::get()->getSettingValue<bool>("no-lock-hint");
		auto creatorBtnEnabled = Mod::get()->getSettingValue<bool>("creator-btn-enabled");

		if (demons) {
			auto winSize = CCDirector::sharedDirector()->getWinSize();

			auto demonIcon = CCSprite::createWithSpriteFrameName("GJ_demonIcon_001.png");
			demonIcon->setPosition({winSize.width - 18, winSize.height - 117});
			demonIcon->setAnchorPoint({0.5, 0.5});
			demonIcon->setScale({0.5});
			demonIcon->setID("demons-icon");

			CCDictionary* stats = GameStatsManager::sharedState()->m_playerStats;
			CCLabelBMFont* demonText = CCLabelBMFont::create(std::to_string(stats->valueForKey("5")->intValue()).c_str(), "bigFont.fnt");
			demonText->setPosition(CCPoint(demonIcon->getPositionX() - 12, this->getChildByID("diamond-shards-label")->getPositionY() - 15));
			demonText->setScale({0.34});
			demonText->setAnchorPoint({1, 0.5});
			demonText->setID("demons-label");

			this->addChild(demonIcon);
			this->addChild(demonText);
		}

		if (cp) {
            auto winSize = CCDirector::sharedDirector()->getWinSize();
            auto demonIcon = CCSprite::createWithSpriteFrameName("GJ_hammerIcon_001.png");
            demonIcon->setPosition({winSize.width - 18, winSize.height - 132});
            demonIcon->setAnchorPoint({0.5, 0.5});
            demonIcon->setScale({0.5});
            demonIcon->setID("cp-icon");
            this->addChild(demonIcon);

            int accID = GJAccountManager::get()->m_accountID;

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

		if (tapLockHint) {
			this->removeChildByID("tap-more-hint");

			// auto cursor = this->getChildByID("cursor-1");
			// cursor->setPositionY(75.f);

			// auto bg = this->getChildByID("select-background");
			// bg->setPositionY(105.f);

			// auto icons = this->getChildByID("icon-selection-bar");
			// icons->setPositionY(10.f);

			// auto prevMenu = this->getChildByID("prev-page-menu");
			// prevMenu->setPositionY(105.f);

			// auto nextMenu = this->getChildByID("next-page-menu");
			// nextMenu->setPositionY(105.f);
		}

		if (creatorBtnEnabled) {
			auto shards = this->getChildByID("shards-menu");

			auto playBtnBtnSpr = CCSprite::create("GaragePlus_creatorBtn.png"_spr);
			auto playBtnBtn = CCMenuItemSpriteExtra::create(
				playBtnBtnSpr, this, menu_selector(MyLayer::onClick)
			);
			// playBtnBtn->setPosition(CCPoint(20.25, 95.f));
			// playBtnBtnSpr->setContentSize(CCSize(35.500, 37.f));
			// playBtnBtn->setAnchorPoint(CCPoint(0.625, 0.625));
			// playBtnBtnSpr->setScale(0.5);
			shards->addChild(playBtnBtn);
			shards->updateLayout();
		}

		return true;
	}
};