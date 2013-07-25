/*********************************************************************************************************
*
* HelloWorldScenne.h
* DemoGame1
*
* Created by Nguyen Van Nha on 23/7.
*
* Copyright Vinova Pte. Ltd. All rights reserved.
************************************************************************************************************
*/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

class HelloWorld : public cocos2d::CCLayerColor
{
public:
	//constructor and deconstructor
	HelloWorld();
	~HelloWorld();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();

	//when untouch the screen
	virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

	//add some enemies 
	void addTarget ();
    //action when target movefinish
	void spriteMoveFinished(CCNode* psender);
	//call function addtarget
	void gameLogic(float pdt);
	//draw back scene
	void update(float pdt);
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

protected:
	CCArray* mtargets;
	CCArray* mprojectiles;
	int nprojectileDestroyed;
};

#endif // __HELLOWORLD_SCENE_H__
