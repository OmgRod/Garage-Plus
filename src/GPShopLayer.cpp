// #include <Geode/Geode.hpp>
// #include "GPShopLayer.hpp"
// #include "GPShopsLayer.hpp"

// using namespace geode::prelude;

// void GPShopLayer::onBack(CCObject*) {
//     auto scenePrev = CCTransitionFade::create(0.5f, GPShopsLayer::scene());
//     CCDirector::sharedDirector()->replaceScene(scenePrev);
// }

// void GPShopLayer::onShop1(CCObject*) {
//     GPShopLayer* ret = new GPShopLayer();
//     if (ret && ret->init(ShopType::Normal)) {
//         ret->autorelease();
//         return ret;
//     }
//     delete ret;
// }

// bool GPShopLayer::init(ShopType p0) {
//     // Initialize with required parameters
//     return GJShopLayer::init(p0);  // Ensure this matches GJShopLayer's init
// }

// CCScene* GPShopLayer::scene() {
//     auto scene = CCScene::create();
//     scene->addChild(GPShopLayer::create());
//     return scene;
// }
