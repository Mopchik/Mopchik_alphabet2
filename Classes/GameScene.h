#ifndef MOPCHIK_GAMESCENE_H
#define MOPCHIK_GAMESCENE_H

#include <cocos2d.h>

class GameScene : public cocos2d::Scene
{
public:
    virtual void applicationDidEnterBackground() = 0;
    virtual void applicationWillEnterForeground() = 0;
};

#endif //MOPCHIK_GAMESCENE_H
