#include <Geode/Geode.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class $modify(MenuLayerModified, MenuLayer) {
    void onGarage(CCObject*) {
        // Deprecation Warning

        if (!Mod::get()->getSavedValue<bool>("deprecation-warning")) {
            geode::createQuickPopup(
                "Garage Plus",
                "<cr>WARNING:</c> I, <cb>OmgRod</c>, am <cy>discontinuing</c> the mod (no more updates will come out)\nHowever, I am working on splitting this mod into several different mods. Thank you for downloading this mod!\n\n- OmgRod",
                "Don't show this again", "OK",
                [&](auto, bool btn2) {
                    if (!btn2) {
                        Mod::get()->setSavedValue("deprecation-warning", true);
                    }
                    auto scene = GJGarageLayer::scene();
                    auto transition = CCTransitionFade::create(0.5f, scene);
                    CCDirector::sharedDirector()->pushScene(transition);
                }
            );
        }

        auto scene = GJGarageLayer::scene();
        auto transition = CCTransitionFade::create(0.5f, scene);
        CCDirector::sharedDirector()->pushScene(transition);
    }
};