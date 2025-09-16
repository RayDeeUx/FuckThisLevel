#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LikeItemLayer.hpp>

using namespace geode::prelude;

int nodeIDsMajorVersion = -1;
int nodeIDsMinorVersion = -1;
bool calledAlready = false;

class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;

		if (calledAlready) return true;
		calledAlready = true;

		auto nodeIDsVersion = Loader::get()->getLoadedMod("geode.node-ids")->getVersion();
		nodeIDsMajorVersion = static_cast<int>(nodeIDsVersion.getMajor());
		nodeIDsMinorVersion = static_cast<int>(nodeIDsVersion.getMinor());

		return true;
	}
};

class $modify(MyLikeItemLayer, LikeItemLayer) {
	static void onModify(auto & self) {
		(void) self.setHookPriority("LikeItemLayer::init", -3999);
	}
	bool init(LikeItemType p0, int p1, int p2) {
		if (!LikeItemLayer::init(p0, p1, p2)) return false;
		if (!Mod::get()->getSettingValue<bool>("enabled")) return true;
		if (!m_buttonMenu) return true;

		auto* likeButtonNode = m_mainLayer->querySelector("action-menu > like-button");
		auto* dislikeButtonNode = m_mainLayer->querySelector("action-menu > dislike-button");
		auto* likeDislikeParent = m_mainLayer->getChildByID("action-menu");
		if (!likeButtonNode || !dislikeButtonNode) {
			if (nodeIDsMinorVersion < 22 && nodeIDsMajorVersion < 2) {
				likeButtonNode = m_buttonMenu->getChildByType<CCMenuItemSpriteExtra>(1);
				dislikeButtonNode = m_buttonMenu->getChildByType<CCMenuItemSpriteExtra>(2);
				likeDislikeParent = m_buttonMenu;
			} else return true;
		}

		auto* likeButton = static_cast<CCMenuItemSpriteExtra*>(likeButtonNode);
		auto* dislikeButton = static_cast<CCMenuItemSpriteExtra*>(dislikeButtonNode);

		auto* fuckYouSprite = CCSprite::create("fuckYou.png"_spr);
		auto* fuckYouCircle = CircleButtonSprite::create(fuckYouSprite);
		fuckYouCircle->setScale(1.2f);
		auto* middleFingerButton = CCMenuItemSpriteExtra::create(
			fuckYouCircle, this, menu_selector(MyLikeItemLayer::onFuckYou)
		);
		fuckYouSprite->setScale(.85f);
		middleFingerButton->setID("fuck-this-level-button"_spr);

		if (Mod::get()->getSettingValue<bool>("fuckYouCounter")) {
			CCLabelBMFont* fuckYouCountLabel = CCLabelBMFont::create(fmt::format("\"Fuck You\" Count: {}", Mod::get()->getSavedValue<int64_t>("fuck-you-count", 0)).c_str(), "bigFont.fnt");
			fuckYouCountLabel->limitLabelWidth(135.f, 1.f, .0001f);
			fuckYouCountLabel->setID("fuck-you-label"_spr);
			m_mainLayer->addChild(fuckYouCountLabel);
			fuckYouCountLabel->setPosition(CCDirector::get()->getWinSize().width / 2.f, 92.f);
		}

		if (nodeIDsMinorVersion > 21 && nodeIDsMajorVersion > 0) {
			likeDislikeParent->addChild(middleFingerButton);
			if (Mod::get()->getSettingValue<bool>("replaceDislike")) {
				dislikeButton->setOpacity(0);
				dislikeButton->setScale(0.f);
				dislikeButton->setVisible(false);
				dislikeButton->setEnabled(false);
				dislikeButton->m_pListener = nullptr;
				dislikeButton->m_pfnSelector = nullptr;
			} else {
				dislikeButton->setZOrder(likeButton->getZOrder() + 1);
			}
			likeDislikeParent->updateLayout();
			return true;
		}

		m_buttonMenu->addChild(middleFingerButton);
		middleFingerButton->setPosition(0.f, -15.f);


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

		likeButtonNode->setPositionX(likeButtonNode->getPositionX() - 17.f);
		dislikeButtonNode->setPositionX(dislikeButtonNode->getPositionX() + 17.f);
		return true;
	}
	void onFuckYou(CCObject* sender) {
		auto fuckYouCount = Mod::get()->getSavedValue<int64_t>("fuck-you-count", 0);
		Mod::get()->setSavedValue<int64_t>("fuck-you-count", fuckYouCount + 1);
		(void) Mod::get()->saveData();
		LikeItemLayer::onDislike(nullptr);
	}
};