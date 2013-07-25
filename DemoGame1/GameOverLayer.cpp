#include "HelloWorldScene.h"
#include "GameOverLayer.h"

using namespace cocos2d;

///////////////////////////////////////////////
//implement class GameOverScene
//destructor
GameOverScene :: ~GameOverScene()
{
	if (mlayer)
	{
		mlayer->release();
		mlayer = NULL;
	}
}

//initialize instance
bool GameOverScene::init ()
{
	//creater new object GameOverLayer and add to Director
	if(CCScene::init())
	{
		this->mlayer = GameOverLayer::create();
		this->mlayer->retain();
		this->addChild(mlayer);

		return true;
	}
	else
	{
		return false;
	}
}

/////////////////////////////////////////////////////////////////////
//implement class GameOverLayer
/**
*destructor: delete pointer mlabel
*/
GameOverLayer::~GameOverLayer()
{
	if(mlabel)
	{
		mlabel->release();
		mlabel = NULL;
	}
}

/**
*on "init" you need to initialize your instance
*/
bool GameOverLayer::init ()
{
	if (CCLayerColor::initWithColor(ccc4(255, 255, 255, 0)))
	{
		//create a lable use font artial, textsize 32, color blue and position center 
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		this->mlabel = CCLabelTTF::create("", "Artial", 32);
		mlabel->retain();
		mlabel->setColor(ccc3(0, 0 , 255));
		mlabel->setPosition(ccp (winSize.width/2, winSize.height/2));
		this->addChild(mlabel);

		this->runAction( CCSequence::create(
						CCDelayTime::create(3),
						CCCallFunc::create(this,
						callfunc_selector(GameOverLayer::gameOverDone)),
						NULL));

		return true;
	}
	else
	{
		return false;
	}
}

/**
*function gameOverDone action when gameover
*/
void GameOverLayer::gameOverDone()
{
	//cho xem goi lai tro choi
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}
