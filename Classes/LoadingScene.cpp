#include "LoadingScene.h"
#include "MenuScene.h"

Scene* LoadingScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LoadingScene::create();
	scene->addChild(layer);
	return scene;
}

bool LoadingScene::init()
{
	Layer::init();

	auto winSize = Director::getInstance()->getWinSize();

	m_texture_num = 0; //��ǰ�Ѽ��ص�ͼƬ
	int *ptexture_num = &m_texture_num;

	auto label = Label::createWithSystemFont("Loading...", "Arial", 36);
	label->setPosition(winSize.width / 2, winSize.height / 2);
	addChild(label);

	//������ϻص�
	auto addTextureCallback = [ptexture_num](Texture2D* texture)
	{
		(*ptexture_num)++;
		log("load a texture async");
	};

	//�첽Ԥ����ͼƬ
	TextureCache::getInstance()->addImageAsync("char_bg_selected.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("char_bg_normal.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("char_bg_crush.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("char_bg_special.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("grid_bg.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("bg.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("level_bar.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("labelatlas.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("star1.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("star2.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("star3.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("bias.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("right_left.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("MenuScene/adventure_mode.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("MenuScene/level_mode.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("MenuScene/level_mode_selected.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("MenuScene/menu_bg.jpg", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("LevelScene/label_for_level.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("LevelScene/leel_locked.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("LevelScene/level_opened.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("LevelScene/level_opened_selected.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("LevelScene/level_bg.jpg", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("LevelScene/level_num.png", addTextureCallback);

	//�������ؽ��ȼ��
	schedule(schedule_selector(LoadingScene::onTextureLoading));

	//Ԥ������Ч
	//SimpleAudioEngine::getInstance()->preloadEffect("crush.ogg");
	//SimpleAudioEngine::getInstance()->preloadEffect("swapback.ogg");

	return true;
}

void LoadingScene::onTextureLoading(float dt)
{
	//һ��ͼƬ������ϣ���ô������Ϸ����
	if (m_texture_num == TOTAL_PIC)
	{
		unschedule(schedule_selector(LoadingScene::onTextureLoading));
		log("loading down!");

		//�ӹؿ������ļ��У���ȡ�ؿ�������Ϣ������Ϣ���б��͵ģ�ÿһ����Ԫ���һ���ؿ���Ϣ
		auto sharedFile = FileUtils::getInstance();
		m_levels_info = sharedFile->getValueVectorFromFile("level_config.plist");

		auto call = CallFunc::create([this](){
			auto scene = MenuScene::createScene(m_levels_info);
			Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
		});

		//�ȴ�һ���������
		this->runAction(Sequence::create(DelayTime::create(0.5), call, nullptr));
	}
}