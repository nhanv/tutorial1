/***************************************************************************
*
* HelloWorldScene.cpp
* DemoGame1
*
* Created by Nguyen Van Nha on 23/7.
*
* Copyright Vinova Pte. Ltd. All rights reserved.
*****************************************************************************
*/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "GameOverLayer.h"

USING_NS_CC;

/**
*constructor
*/
HelloWorld::HelloWorld()
{
	//khoi tao so luong doi tuong ban duoc = 0
	nprojectileDestroyed = 0;
}

/**
*destructor: delete poiter mtargets and mprojectiles
*/
HelloWorld::~HelloWorld()
{
	if(mtargets)
	{
		mtargets->release();
		mtargets = NULL;
	}

	if(mprojectiles)
	{
		mprojectiles->release();
		mprojectiles = NULL;
	}
}
	

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene* scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld* layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

	//set color on background
	if (CCLayerColor::initWithColor(ccc4 (255, 255, 255, 255)))
	{
	}
    
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.wav", true);

	mtargets = new CCArray;
	mprojectiles = new CCArray;	

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage* pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // add "HelloWorld" splash screen"
	CCSprite* pSprite = CCSprite::create("Player.png", CCRectMake(0, 0, 27, 40));

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(20, visibleSize.height/2));
	
	//listern touch action
	setTouchEnabled (true);

	//add enemies into game
	this->schedule( schedule_selector (HelloWorld::gameLogic), 1.0);
	//call update function
	this->schedule( schedule_selector (HelloWorld::update) );
	// add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    
    return true;
}

/**
* when touch the screen
*/
void HelloWorld::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.wav");
	//get location touch on screen
	CCTouch* touch = (CCTouch*) (pTouches->anyObject());
	CCPoint location = touch->getLocationInView();
	//get windown size
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	//add Projectile image into game
	CCSprite* projectile = CCSprite::create("Projectile.png", CCRect(0, 0, 20, 20));
	//set tag
	projectile->setTag(2);
	mprojectiles->addObject(projectile);
	//set position
	projectile->setPosition(ccp(20, winSize.height/2));

	//don't add projectile when touch down player
	float offX = (location.x) - (projectile->getPosition().x);
	float offY = 0 - ((location.y) - (projectile->getPosition().y));

	if(offX <= 0)
		return;

	this->addChild(projectile);

	//location projectile wish shoot to
	float readX = winSize.width + (projectile->getContentSize().width/2);
	float radio = (float)offY / (float)offX;
	float readY = (readX*radio) + projectile->getPosition().y;
	CCPoint goal = ccp(readX, readY);

	//khoang cach toi muc tieu
	float offReadX = readX - projectile->getPosition().x;
	float offReadY = readY - projectile->getPosition().y;
	float length = sqrtf (offReadX*offReadX + offReadY*offReadY);

	//speed projectile
	float veloctity = 480;
	float speed = length/veloctity;
	
	//move projectile to goal
	CCFiniteTimeAction* moveTo = CCMoveTo::create (speed, goal);
	CCFiniteTimeAction* moveDone = CCCallFuncN::create(this,
									callfuncN_selector(HelloWorld::spriteMoveFinished));
	projectile->runAction(CCSequence::create(moveTo, moveDone, NULL));

}

/**
* add some enemies into game
*/
void HelloWorld::addTarget ()
{
	//CCRectMake size picture(27x40)
	CCSprite* target = CCSprite::create("Target.png", CCRectMake(0, 0, 27, 40));
	//set tag
	target->setTag(1);
	mtargets->addObject(target);
	//get sizeWindown
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	//get size target
	int minY = target->getContentSize().height/2;
	int maxY = winSize.height-target->getContentSize().height/2;

	int rangeY = maxY - minY;
	//random position target
	int actualY = (rand() % rangeY) + minY;
	target->setPosition(ccp( winSize.width+(target->getContentSize().width/2), actualY));

	//add target into game
	this->addChild(target);

	//random speed from 2 to 4
	int speed = (rand() % 2) + 2;
	//create the action move from position to min x with speed = speed
	CCFiniteTimeAction* actionMove = CCMoveTo::create((float)speed,
												ccp(0-target->getContentSize().width/2, actualY)); 
	//action when target move finish: call function spriteMoveFinished
	CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create(this,
												callfuncN_selector(HelloWorld::spriteMoveFinished));
	//action when 
	target->runAction (CCSequence::create(actionMove, actionMoveDone, NULL));
}

/**
*action when target movedone
*/
void HelloWorld::spriteMoveFinished(CCNode* psender)
{
	//remove sprite when target movefinish
	CCSprite* pSprite = (CCSprite*) psender;
	this->removeChild(pSprite, true);

	if(pSprite->getTag() == 1)
	{
		mtargets->removeObject(pSprite);
		GameOverScene *gameOverScene = GameOverScene::create();
		gameOverScene->getLayer()->getLabel()->setString("You lose");
		CCDirector::sharedDirector()->replaceScene(gameOverScene);
	}
	else if(pSprite->getTag() == 2)
	{
		mprojectiles->removeObject(pSprite);
	}
}

/**
*call function addtarget
*/
void HelloWorld::gameLogic(float pdt){
	this->addTarget();
}

/**
*Draw back scene (update)
*/
void HelloWorld::update(float pdt)
{

	//khai bao 2 mang luu projectile va target va cham de xoa
	CCArray* projectilesToDelete = new CCArray;
	CCArray* targetsToDelete = new CCArray;

	//khai bao 2 mang doi tuong de lay du lieu tu _target va _projectile
	CCObject* it = NULL;
	CCObject* jt = NULL;

	//gan doi tuong tu array _projectiles sang it
	CCARRAY_FOREACH (mprojectiles, it)
	{

		//tao doi tuong projectile dc ep kieu tu doi tuong it ve
		CCSprite* projectile = dynamic_cast<CCSprite*>(it);
		//tao doi tuong CCRect: 1 vung chu nhat bao quanh doi tuong projectile
		CCRect projectileRect = CCRectMake(
							projectile->getPosition().x - (projectile->getContentSize().width /2),
							projectile->getPosition().y - (projectile->getContentSize().height /2),
							projectile->getContentSize().width,
							projectile->getContentSize().height);

		//gan doi tuong tu  array _targets vao jt
		CCARRAY_FOREACH (mtargets, jt)
		{
			//lam lai nhu projectile
			CCSprite* target = dynamic_cast<CCSprite*>(jt);
			CCRect targetRect = CCRectMake (
							target->getPosition().x - (target->getContentSize().width /2),
							target->getPosition().y - (target->getContentSize().height /2),
							target->getContentSize().width,
							target->getContentSize().height);

			//kiem tra va cham giua projectileRect va targetRect
			if (projectileRect.intersectsRect (targetRect))
			{
				//neu xay ra va cham thi dua target vaof mang targetsToDelete, projectile tuong tu
				targetsToDelete->addObject (target);
				projectilesToDelete->addObject (projectile);
			}
		}
	}

	//lay du lieu tu targetsToDelete ma luu o tren sang jt
	CCARRAY_FOREACH(targetsToDelete, jt)
    {
		//lay doi tuong tu mang removeObject ra va remove
        CCSprite* target = dynamic_cast<CCSprite*>(jt);
        mtargets->removeObject(target);
        this->removeChild(target, true);
		nprojectileDestroyed++;

		//ban dc 5 doi tuong thi win
		if(nprojectileDestroyed >= 5)
		{
			//set text and change screen
			GameOverScene* gameOverScene = GameOverScene::create();
			gameOverScene->getLayer()->getLabel()->setString("You win");
			CCDirector::sharedDirector()->replaceScene(gameOverScene);

		}
    }

	//tuong tu nhu target, ta lam voi projectile
    CCARRAY_FOREACH(projectilesToDelete, it)
    {
        CCSprite* projectile = dynamic_cast<CCSprite*>(it);
        mprojectiles->removeObject(projectile);
        this->removeChild(projectile, true);
    }

	targetsToDelete->release();
    projectilesToDelete->release();
}

/**
*Close game when click closemenu
*/
void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
