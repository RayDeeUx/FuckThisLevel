#include <Geode/modify/LikeItemLayer.hpp>

using namespace geode::prelude;

class $modify(MyLikeItemLayer, LikeItemLayer) {
	static void onModify(auto & self) {
		(void) self.setHookPriority("LikeItemLayer::init", -3999);
	}
	bool init(LikeItemType p0, int p1, int p2) {
		if (!LikeItemLayer::init(p0, p1, p2)) return false;
		if (!Mod::get()->getSettingValue<bool>("enabled")) return true;
		if (!m_buttonMenu) return true;

		auto* likeButton = m_buttonMenu->getChildByType<CCMenuItemSpriteExtra>(1);
		auto* dislikeButton = m_buttonMenu->getChildByType<CCMenuItemSpriteExtra>(2);
		if (!likeButton || !dislikeButton) return true;

		auto* fuckYouSprite = CCSprite::create("fuckYou.png"_spr);
		auto* fuckYouCircle = CircleButtonSprite::create(fuckYouSprite);
		fuckYouCircle->setScale(1.2f);
		auto* middleFingerButton = CCMenuItemSpriteExtra::create(
			fuckYouCircle, this, menu_selector(LikeItemLayer::onDislike)
		);
		fuckYouSprite->setScale(.85f);

		m_buttonMenu->addChild(middleFingerButton);
		middleFingerButton->setPosition(0.f, -15.f);

		middleFingerButton->setID("fuck-this-level-button"_spr);

		if (Mod::get()->getSettingValue<bool>("replaceDislike")) {
			middleFingerButton->setPosition(dislikeButton->getPosition());
			dislikeButton->setPosition(9122002.f, 9122002.f);
			dislikeButton->setOpacity(0);
			dislikeButton->setScale(0.f);
			dislikeButton->setVisible(false);
			dislikeButton->m_pListener = nullptr;
			dislikeButton->m_pfnSelector = nullptr;
			return true;
		}

		likeButton->setPositionX(likeButton->getPositionX() - 17.f);
		dislikeButton->setPositionX(dislikeButton->getPositionX() + 17.f);
		return true;
	}
};