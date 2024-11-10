#include <Geode/Geode.hpp>
#include <Geode/modify/GameManager.hpp>
#include "../../hacks.hpp"

using namespace geode::prelude;

class $modify(GameManager) {
    bool isIconUnlocked(int _id, IconType _type) {
        if (hacks::getInstance().isHackEnabled("icon-hack")) {
            if (GameManager::isIconUnlocked(_id, _type)) return true;
            if (_id <= 0) return false;
            return true;
        } else {
            // Call the original GameManager method
            return GameManager::isIconUnlocked(_id, _type);
        }
    }

    bool isColorUnlocked(int _id, UnlockType _type) {
        if (hacks::getInstance().isHackEnabled("color-hack")) {
            if (GameManager::isColorUnlocked(_id, _type)) return true;
            return true;
        } else {
            // Call the original GameManager method
            return GameManager::isColorUnlocked(_id, _type);
        }
    }
};