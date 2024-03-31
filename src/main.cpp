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
			/*
			auto winSize = CCDirector::sharedDirector()->getWinSize();
auto demonIcon = CCSprite::createWithSpriteFrameName("GJ_hammerIcon_001.png");
demonIcon->setPosition({winSize.width - 18, winSize.height - 132});
demonIcon->setAnchorPoint({0.5, 0.5});
demonIcon->setScale({0.5});
demonIcon->setID("cp-icon");

// Fetch the username synchronously
std::string username = GJAccountManager::get()->m_username;

// Fetch the text asynchronously from the URL using the username
web::AsyncWebRequest()
  .fetch("https://ksr.ps.fhgdps.com/incl/misc/getAccountCP.php?username=" + username)
  .text()
  .then([=](std::string const& usernameParam) {
        auto demonText = CCLabelBMFont::create(usernameParam.c_str(), "bigFont.fnt");

        // Check if the demons-label node exists before accessing its position
        auto demonsLabelNode = this->getChildByID("demons-label");
        if (demonsLabelNode) {
            demonText->setPosition(CCPoint(demonIcon->getPositionX() - 12, demonsLabelNode->getPositionY() - 15));
        } else {
            // Handle the case where demons-label node does not exist
            // For example, log a warning or set a default position
            std::cerr << "Warning: demons-label node not found." << std::endl;
            demonText->setPosition(CCPoint(demonIcon->getPositionX() - 12, 0)); // Set a default position
        }

        demonText->setScale(0.34);
        demonText->setAnchorPoint({1, 0.5});
        demonText->setID("cp-label");

        this->addChild(demonText);
        this->addChild(demonIcon);
        this->updateLayout();
    })
  .expect([](std::string const& error) {
        std::cerr << "Failed to fetch username parameter: " << error << std::endl;
    });

	*/

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