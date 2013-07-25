#ifndef _GAME_OVER_SCENE_H_
#define _GAME_OVER_SCENE_H_

#include "cocos2d.h"


class GameOverLayer : public cocos2d::CCLayerColor
{
public:
	//constructor function
	GameOverLayer() : mlabel(NULL) {};
	//destructor function
	~GameOverLayer();
	//init()
	bool init();

	void gameOverDone ();

	CREATE_FUNC(GameOverLayer);

	//khai bao doi tuong Label de hien thi text
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, mlabel, Label)

};

class GameOverScene : public cocos2d::CCScene
{
public:
	//constructor and destructor
	GameOverScene() : mlayer(NULL) {};
	~GameOverScene();
	//init
	bool init ();

	CREATE_FUNC(GameOverScene);
	//khai bao doi tuong Layer
	CC_SYNTHESIZE_READONLY(GameOverLayer*, mlayer, Layer);
};

#endif