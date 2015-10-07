//
//  GameScene.h
//  Voxel_Explorer
//
//  Created by wang haibo on 15/9/24.
//
//

#ifndef __Voxel_Explorer__GameScene__
#define __Voxel_Explorer__GameScene__

#include "cocos2d.h"

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    CREATE_FUNC(GameScene);
};

#endif /* defined(__Voxel_Explorer__GameScene__) */