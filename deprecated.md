# Advanced Stats

mod.json

```

		"advanced-stats": {
            "name": "Advanced Stats",
            "description": "Displays advenced stats when clicking icons such as demons and stars (like when you open your profile and click them.)",
            "type": "bool",
            "default": true
        },
```

main.cpp

```

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

```