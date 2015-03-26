//
//  Enemies.cpp
//  Geometry-Wars
//
//  Created by wang haibo on 15/3/9.
//
//

#include "Enemies.h"
#include "GameController.h"
#include "ParticleSystemHelper.h"
#include "ActorsManager.h"
USING_NS_CC;
Enemy::Enemy()
{
    m_type = AT_ENEMY;
    m_EnemyType = ET_UNKNOWN;
}
Enemy::~Enemy()
{
}
void Enemy::update(float delta)
{
    if(m_curState == ActorState::AS_TRACK)
    {
        if(GameController::getInstance()->getPlayer())
        {
            Vec2 playerPos = GameController::getInstance()->getPlayerPos();
            Vec2 dir = playerPos - getPosition();
            setDirection(dir);
        }
        else
            setDirection(Vec2::ZERO);
        
        m_Orientation = m_Direction;
    }
    
    float mainRotationZ = CC_RADIANS_TO_DEGREES(Vec2::angle(m_Direction, Vec2::UNIT_Y));
    if (m_Direction.x < 0)
        mainRotationZ = -mainRotationZ;
    setRotation(mainRotationZ);
    
    if(!m_bBounce)
    {
        if (m_Direction == Vec2::ZERO) {
            m_Velocity *= 0.99f;
            if(m_Velocity.length() <0.001f)
                m_Velocity = Vec2::ZERO;
        }
        else
        {
            m_Velocity += m_Direction*m_fAccel*delta;
            if(m_Velocity.length() >= m_fMaxSpeed)
            {
                m_Velocity.normalize();
                m_Velocity = m_Velocity*m_fMaxSpeed;
            }
        }
    }
    Vec2 pos = getPosition();
    Vec2 newPos = pos + m_Velocity;
    setPosition(newPos);
}
cocos2d::Color3B Enemy::getColor()
{
    if (m_pModel)
        return m_pModel->getColor();
    return Color3B::WHITE;
}
void Enemy::setColor(const cocos2d::Color3B& color)
{
    if(m_pModel)
        m_pModel->setColor(color);
}
void Enemy::beginTrack()
{
    m_fAccel = cocos2d::random(4.0f, 5.0f);
    m_fMaxSpeed = cocos2d::random(2.0f, 3.0f);
    setActorState(ActorState::AS_TRACK);
}
Circle::Circle()
{
    m_EnemyType = ET_CIRCLE;
    m_fAccel = 40.0f;
    m_fMaxSpeed = 10.0f;
}
Circle::~Circle()
{
}
void Circle::onEnterDead()
{
    ParticleSystemHelper::spawnExplosion(ExplosionType::ET_EXPLOSION_WHITE, getPosition());
}

/////ColorCircle
ColorCircle::ColorCircle()
{
    m_EnemyType = ET_CIRCLE_COLORED;
    m_fAccel = cocos2d::random(4.0f, 5.0f);
    m_fMaxSpeed = cocos2d::random(2.0f, 3.0f);
}
ColorCircle::~ColorCircle()
{
}
void ColorCircle::onEnterDead()
{
    ParticleSystemHelper::spawnExplosion(ExplosionType::ET_EXPLOSION_WHITE, getPosition());
    GameController::getInstance()->getEnemiesGenerator()->generateEnemiesByExplosion(Enemy::ET_CIRCLE, getPosition(), 2);
}

/////Triangle
Triangle::Triangle()
{
    m_EnemyType = ET_TRIANGLE;
    m_fAccel = cocos2d::random(4.0f, 5.0f);
    m_fMaxSpeed = cocos2d::random(2.0f, 3.0f);
}
Triangle::~Triangle()
{
}
void Triangle::onEnterDead()
{
    ParticleSystemHelper::spawnExplosion(ExplosionType::ET_EXPLOSION_YELLOW, getPosition());
}
void Triangle::beginTrack()
{
    m_fAccel = cocos2d::random(8.0f, 10.0f);
    m_fMaxSpeed = cocos2d::random(4.0f, 6.0f);
    ScaleTo* scaleTo1 = ScaleBy::create(0.2f, 0.8f);
    ScaleTo* scaleTo2 = ScaleBy::create(0.2f, 1.25f);
    Sequence* sequence = Sequence::createWithTwoActions(scaleTo1, scaleTo2);
    RepeatForever* repeatForever = RepeatForever::create(sequence);
    runAction(repeatForever);
    setActorState(ActorState::AS_TRACK);
}

ColorTriangle::ColorTriangle()
{
    m_EnemyType = ET_TRIANGLE_COLORED;
    m_fTrackTime = 0.0f;
    m_fChargeTime = 0.0f;
    m_fIdleTime = 0.0f;
}
ColorTriangle::~ColorTriangle()
{
}
void ColorTriangle::update(float delta)
{
    if(m_curState == AS_TRACK)
    {
        m_fTrackTime += delta;
        float dist = GameController::getInstance()->getPlayerPos().distance(getPosition());
        if(m_fTrackTime >= 5.0f && dist >= GameController::getInstance()->getBoundSize().width*0.15f)
        {
            m_fTrackTime = 0.0f;
            setActorState(ActorState::AS_IDLE);
        }
        if(GameController::getInstance()->getPlayer())
        {
            Vec2 playerPos = GameController::getInstance()->getPlayerPos();
            Vec2 dir = playerPos - getPosition();
            setDirection(dir);
        }
        else
            setDirection(Vec2::ZERO);
        
        m_Orientation = m_Direction;
    }
    else if(m_curState == ActorState::AS_IDLE)
    {
        m_fIdleTime += delta;
        float dist = GameController::getInstance()->getPlayerPos().distance(getPosition());
        if(m_fIdleTime >= 2.0f)
        {
            m_fIdleTime = 0.0f;
            if(dist >= GameController::getInstance()->getBoundSize().width*0.1f)
                setActorState(ActorState::AS_CHARGE);
            else
                setActorState(ActorState::AS_TRACK);
        }
    }
    else if(m_curState == ActorState::AS_CHARGE)
    {
        m_fChargeTime += delta;
        if(m_fChargeTime >= 1.0f)
        {
            m_fChargeTime = 0.0f;
            setActorState(ActorState::AS_TRACK);
        }
    }
    
    float mainRotationZ = CC_RADIANS_TO_DEGREES(Vec2::angle(m_Direction, Vec2::UNIT_Y));
    if (m_Direction.x < 0)
        mainRotationZ = -mainRotationZ;
    setRotation(mainRotationZ);
    
    if(!m_bBounce)
    {
        if (m_Direction == Vec2::ZERO) {
            m_Velocity *= 0.99f;
            if(m_Velocity.length() <0.001f)
                m_Velocity = Vec2::ZERO;
        }
        else
        {
            m_Velocity += m_Direction*m_fAccel*delta;
            if(m_Velocity.length() >= m_fMaxSpeed)
            {
                m_Velocity.normalize();
                m_Velocity = m_Velocity*m_fMaxSpeed;
            }
        }
    }
    if(m_curState != ActorState::AS_IDLE)
    {
        Vec2 pos = getPosition();
        Vec2 newPos = pos + m_Velocity;
        setPosition(newPos);
    }
}
void ColorTriangle::onEnterCharge()
{
    Vec2 dir = GameController::getInstance()->getPlayerPos() - getPosition();
    dir.normalize();
    setDirection(dir);
    m_fAccel = 100.0f;
    m_fMaxSpeed = 20.0f;
}
void ColorTriangle::onExitCharge()
{
    setDirection(Vec2::ZERO);
    m_fAccel = cocos2d::random(8.0f, 10.0f);
    m_fMaxSpeed = cocos2d::random(4.0f, 6.0f);
}

///躲避攻击，射击发射子弹
Diamond::Diamond()
{
    m_EnemyType = ET_DIAMOND;
    m_fAccel = cocos2d::random(2.0f, 3.0f);
    m_fMaxSpeed = cocos2d::random(1.0f, 2.0f);
    m_bScheduledFire = false;
}
Diamond::~Diamond()
{
}
void Diamond::beginTrack()
{
    ScaleTo* scaleTo1 = ScaleBy::create(0.5f, 0.5f, 1.0f);
    ScaleTo* scaleTo2 = ScaleBy::create(0.5f, 2.0f, 1.0f);
    Sequence* sequence = Sequence::createWithTwoActions(scaleTo1, scaleTo2);
    RepeatForever* repeatForever = RepeatForever::create(sequence);
    runAction(repeatForever);
    setActorState(ActorState::AS_TRACK);
}
void Diamond::onEnterDead()
{
    ParticleSystemHelper::spawnExplosion(ExplosionType::ET_EXPLOSION_GREEN, getPosition());
}
void Diamond::onEnterTrack()
{
    if(!m_bScheduledFire)
    {
        schedule(CC_SCHEDULE_SELECTOR(Diamond::fire), 1.0f, -1, 0);
        m_bScheduledFire = true;
    }
}
void Diamond::onExitTrack()
{
    if(m_bScheduledFire)
    {
        unschedule(CC_SCHEDULE_SELECTOR(Diamond::fire));
        m_bScheduledFire = false;
    }
}
void Diamond::onEnterFlee()
{
    m_fAccel = cocos2d::random(4.0f, 5.0f);
    m_fMaxSpeed = cocos2d::random(4.0f, 6.0f);
    if(!m_bScheduledFire)
    {
        schedule(CC_SCHEDULE_SELECTOR(Diamond::fire), 2.0f, -1, 0);
        m_bScheduledFire = true;
    }
    
}
void Diamond::onExitFlee()
{
    m_fAccel = cocos2d::random(2.0f, 3.0f);
    m_fMaxSpeed = cocos2d::random(1.0f, 2.0f);
    if(m_bScheduledFire)
    {
        unschedule(CC_SCHEDULE_SELECTOR(Diamond::fire));
        m_bScheduledFire = false;
    }
}

void Diamond::fire(float delta)
{
    if(m_curState == ActorState::AS_UNDERCONTROL)
        return;
    ActorsManager::spawnBullet(GameActor::AT_ENEMY_BULLET, getFireWorldPos(), getOrientation(),MAX(m_fMaxSpeed*2.0f,10.0f),"bullet2.png", Color3B(64,255,1), 0.2f, 1.0f);
}
Vec2 Diamond::getFireWorldPos()
{
    Vec2 ret = getPosition();
    ret += m_Orientation*m_fRadius*0.8f;
    return ret;
}
void Diamond::update(float delta)
{
    if(m_curState == ActorState::AS_TRACK)
    {
        if(GameController::getInstance()->getPlayer())
        {
            Vec2 playerPos = GameController::getInstance()->getPlayerPos();
            Vec2 dir = playerPos - getPosition();
            setDirection(dir);
        }
        else
            setDirection(Vec2::ZERO);
        float dist = GameController::getInstance()->getPlayerPos().distance(getPosition());
        if(dist <= GameController::getInstance()->getBoundSize().width*0.2f)
            setActorState(ActorState::AS_FLEE);
        
        m_Orientation = m_Direction;
    }
    else if(m_curState == ActorState::AS_FLEE)
    {
        if(GameController::getInstance()->getPlayer())
        {
            Vec2 playerPos = GameController::getInstance()->getPlayerPos();
            Vec2 dir = playerPos - getPosition();
            setDirection(-dir);
        }
        else
            setDirection(Vec2::ZERO);
        float dist = GameController::getInstance()->getPlayerPos().distance(getPosition());
        if(dist >= GameController::getInstance()->getBoundSize().width*0.4f)
            setActorState(ActorState::AS_TRACK);
        
        m_Orientation = -m_Direction;
    }
    
    float mainRotationZ = CC_RADIANS_TO_DEGREES(Vec2::angle(m_Direction, Vec2::UNIT_Y));
    if (m_Direction.x < 0)
        mainRotationZ = -mainRotationZ;
    setRotation(mainRotationZ);
    
    if(!m_bBounce)
    {
        if (m_Direction == Vec2::ZERO) {
            m_Velocity *= 0.99f;
            if(m_Velocity.length() <0.001f)
                m_Velocity = Vec2::ZERO;
        }
        else
        {
            m_Velocity += m_Direction*m_fAccel*delta;
            if(m_Velocity.length() >= m_fMaxSpeed)
            {
                m_Velocity.normalize();
                m_Velocity = m_Velocity*m_fMaxSpeed;
            }
        }
    }
    Vec2 pos = getPosition();
    Vec2 newPos = pos + m_Velocity;
    setPosition(newPos);
}

///躲避玩家，甚至躲避玩家发射的子弹, 分裂攻击 死后有几率掉落武器强化道具
ColorDiamond::ColorDiamond()
{
    m_EnemyType = ET_DIAMOND_COLORED;
}
ColorDiamond::~ColorDiamond()
{
}
void ColorDiamond::fire(float delta)
{
    if(m_curState == ActorState::AS_UNDERCONTROL)
        return;
    ActorsManager::spawnBullet(GameActor::AT_ENEMY_BULLET, getFireWorldPos(), getOrientation(),MAX(m_fMaxSpeed*2.0f,10.0f),"bullet2.png", Color3B(64,255,1), 0.2f, 1.0f);
    Vec2 orient = getOrientation();
    orient.rotate(Vec2::ZERO, M_PI*0.125f);
    ActorsManager::spawnBullet(GameActor::AT_ENEMY_BULLET, getFireWorldPos(), orient, MAX(m_fMaxSpeed*2.0f,10.0f),"bullet2.png", Color3B(64,255,1), 0.2f, 1.0f);
    orient.rotate(Vec2::ZERO, M_PI*0.125f);
    ActorsManager::spawnBullet(GameActor::AT_ENEMY_BULLET, getFireWorldPos(), orient, MAX(m_fMaxSpeed*2.0f,10.0f),"bullet2.png", Color3B(64,255,1), 0.2f, 1.0f);
    orient = getOrientation();
    orient.rotate(Vec2::ZERO, -M_PI*0.125f);
    ActorsManager::spawnBullet(GameActor::AT_ENEMY_BULLET, getFireWorldPos(), orient, MAX(m_fMaxSpeed*2.0f,10.0f),"bullet2.png", Color3B(64,255,1), 0.2f, 1.0f);
    orient.rotate(Vec2::ZERO, -M_PI*0.125f);
    ActorsManager::spawnBullet(GameActor::AT_ENEMY_BULLET, getFireWorldPos(), orient, MAX(m_fMaxSpeed*2.0f,10.0f),"bullet2.png", Color3B(64,255,1), 0.2f, 1.0f);
}

///慢速追踪，五星旋转弹幕
Star::Star()
{
    m_EnemyType = ET_STAR;
    m_fAccel = 2.0f;
    m_fMaxSpeed = 2.0f;
    m_bScheduledFire = false;
}
Star::~Star()
{
}
void Star::update(float delta)
{
    if(m_curState == AS_TRACK)
    {
        m_fTrackTime += delta;
        float dist = GameController::getInstance()->getPlayerPos().distance(getPosition());
        if(m_fTrackTime >= 5.0f && dist >= GameController::getInstance()->getBoundSize().width*0.15f)
        {
            m_fTrackTime = 0.0f;
            setActorState(ActorState::AS_IDLE);
        }
        if(GameController::getInstance()->getPlayer())
        {
            Vec2 playerPos = GameController::getInstance()->getPlayerPos();
            Vec2 dir = playerPos - getPosition();
            setDirection(dir);
        }
        else
            setDirection(Vec2::ZERO);
        
        m_Orientation = m_Direction;
    }
    else if(m_curState == ActorState::AS_IDLE)
    {
        m_fIdleTime += delta;
        if(m_fIdleTime >= 3.0f)
        {
            m_fIdleTime = 0.0f;
            setActorState(ActorState::AS_TRACK);
        }
    }
    if(!m_bBounce)
    {
        if (m_Direction == Vec2::ZERO) {
            m_Velocity *= 0.99f;
            if(m_Velocity.length() <0.001f)
                m_Velocity = Vec2::ZERO;
        }
        else
        {
            m_Velocity += m_Direction*m_fAccel*delta;
            if(m_Velocity.length() >= m_fMaxSpeed)
            {
                m_Velocity.normalize();
                m_Velocity = m_Velocity*m_fMaxSpeed;
            }
        }
    }
    if(m_curState != ActorState::AS_IDLE)
    {
        Vec2 pos = getPosition();
        Vec2 newPos = pos + m_Velocity;
        setPosition(newPos);
    }

}
void Star::onEnterIdle()
{
    EaseSineIn* easeIn1 = EaseSineIn::create(RotateBy::create(0.5f, 180));
    Repeat* repeat = Repeat::create(easeIn1, 6);
    runAction(repeat);
    if(!m_bScheduledFire)
    {
        schedule(CC_SCHEDULE_SELECTOR(Star::fire), 0.5f, -1, 0.5);
        m_bScheduledFire = true;
    }
}
void Star::onExitIdle()
{
    stopAllActions();
    if(m_bScheduledFire)
    {
        unschedule(CC_SCHEDULE_SELECTOR(Diamond::fire));
        m_bScheduledFire = false;
    }
}

void Star::onEnterTrack()
{
    RotateBy* rotateBy = RotateBy::create(1.0f, 90);
    RepeatForever* repeatForever = RepeatForever::create(rotateBy);
    runAction(repeatForever);
}
void Star::onExitTrack()
{
    stopAllActions();
}
void Star::onEnterDead()
{
    ParticleSystemHelper::spawnExplosion(ExplosionType::ET_EXPLOSION_RED, getPosition());
}
void Star::beginTrack()
{
    m_fAccel = 3.0f;
    m_fMaxSpeed = 3.0f;
    setActorState(ActorState::AS_TRACK);
    
}
void Star::fire(float delta)
{
    if(m_curState != ActorState::AS_IDLE)
        return;
    
    Vec2 pos = getPosition();
    Vec2 orient = m_Orientation*m_fRadius*0.8f;
    pos += orient;
    ActorsManager::spawnBullet(GameActor::AT_ENEMY_BULLET, pos, orient,2.0f,"bullet2.png", Color3B(254,64,64), 0.5f, 0.5f);
    pos = getPosition();
    orient.rotate(Vec2::ZERO, M_PI*0.4f);
    pos += orient;
    ActorsManager::spawnBullet(GameActor::AT_ENEMY_BULLET, pos, orient,2.0f,"bullet2.png", Color3B(254,64,64), 0.5f, 0.5f);
    pos = getPosition();
    orient.rotate(Vec2::ZERO, M_PI*0.4f);
    pos += orient;
    ActorsManager::spawnBullet(GameActor::AT_ENEMY_BULLET, pos, orient,2.0f,"bullet2.png", Color3B(254,64,64), 0.5f, 0.5f);
    pos = getPosition();
    orient.rotate(Vec2::ZERO, M_PI*0.4f);
    pos += orient;
    ActorsManager::spawnBullet(GameActor::AT_ENEMY_BULLET, pos, orient,2.0f,"bullet2.png", Color3B(254,64,64), 0.5f, 0.5f);
    pos = getPosition();
    orient.rotate(Vec2::ZERO, M_PI*0.4f);
    pos += orient;
    ActorsManager::spawnBullet(GameActor::AT_ENEMY_BULLET, pos, orient,2.0f,"bullet2.png", Color3B(254,64,64), 0.5f, 0.5f);
}

ColorStar::ColorStar()
{
    m_EnemyType = ET_STAR_COLORED;
    m_pNode1    = nullptr;
    m_pNode2    = nullptr;
    m_pNode3    = nullptr;
    m_pNode4    = nullptr;
    m_pNode5    = nullptr;
}
ColorStar::~ColorStar()
{
}
void ColorStar::update(float delta)
{
    if(m_curState == AS_TRACK)
    {
        m_fTrackTime += delta;
        float dist = GameController::getInstance()->getPlayerPos().distance(getPosition());
        if(m_fTrackTime >= 3.0f && dist >= GameController::getInstance()->getBoundSize().width*0.15f)
        {
            m_fTrackTime = 0.0f;
            setActorState(ActorState::AS_IDLE);
        }
        if(GameController::getInstance()->getPlayer())
        {
            Vec2 playerPos = GameController::getInstance()->getPlayerPos();
            Vec2 dir = playerPos - getPosition();
            setDirection(dir);
        }
        else
            setDirection(Vec2::ZERO);
        
        m_Orientation = m_Direction;
    }
    else if(m_curState == ActorState::AS_IDLE)
    {
        m_fIdleTime += delta;
        if(m_fIdleTime >= 1.0f)
        {
            m_fIdleTime = 0.0f;
            setActorState(ActorState::AS_TRACK);
        }
    }
    if(!m_bBounce)
    {
        if (m_Direction == Vec2::ZERO) {
            m_Velocity *= 0.99f;
            if(m_Velocity.length() <0.001f)
                m_Velocity = Vec2::ZERO;
        }
        else
        {
            m_Velocity += m_Direction*m_fAccel*delta;
            if(m_Velocity.length() >= m_fMaxSpeed)
            {
                m_Velocity.normalize();
                m_Velocity = m_Velocity*m_fMaxSpeed;
            }
        }
    }
    if(m_curState != ActorState::AS_IDLE)
    {
        Vec2 pos = getPosition();
        Vec2 newPos = pos + m_Velocity;
        setPosition(newPos);
    }
    
}
void ColorStar::onEnterIdle()
{
    RotateBy* rotateBy = RotateBy::create(0.5f, 720);
    Repeat* repeat = Repeat::create(rotateBy, 2);
    runAction(repeat);
    if(!m_bScheduledFire)
    {
        schedule(CC_SCHEDULE_SELECTOR(Star::fire), 0.05f, -1, 0.5);
        m_bScheduledFire = true;
    }

}
void ColorStar::onExitIdle()
{
    stopAllActions();
    if(m_bScheduledFire)
    {
        unschedule(CC_SCHEDULE_SELECTOR(Diamond::fire));
        m_bScheduledFire = false;
    }
}

void ColorStar::beginTrack()
{
    m_fAccel = 4.0f;
    m_fMaxSpeed = 4.0f;
    setActorState(ActorState::AS_TRACK);
}
void ColorStar::fire(float delta)
{
    if(m_curState != ActorState::AS_IDLE)
        return;
    
    Vec2 pos = getPosition();
    Vec2 node1Pos = m_pNode1->getPosition();
    node1Pos = convertToWorldSpace(node1Pos);
    Vec2 orient1 = node1Pos - pos;
    orient1.normalize();
    ActorsManager::spawnBullet(GameActor::AT_ENEMY_BULLET, node1Pos, orient1, 2.0f,"bullet2.png", Color3B(254,64,64), 0.5f, 0.6f);
    
    Vec2 node2Pos = m_pNode2->getPosition();
    node2Pos = convertToWorldSpace(node2Pos);
    Vec2 orient2 = node2Pos - pos;
    orient2.normalize();
    ActorsManager::spawnBullet(GameActor::AT_ENEMY_BULLET, node2Pos, orient2, 2.0f,"bullet2.png", Color3B(254,64,64), 0.5f, 0.6f);
    
    Vec2 node3Pos = m_pNode3->getPosition();
    node3Pos = convertToWorldSpace(node3Pos);
    Vec2 orient3 = node3Pos - pos;
    orient3.normalize();
    ActorsManager::spawnBullet(GameActor::AT_ENEMY_BULLET, node3Pos, orient3, 2.0f,"bullet2.png", Color3B(254,64,64), 0.5f, 0.6f);
    
    Vec2 node4Pos = m_pNode4->getPosition();
    node4Pos = convertToWorldSpace(node4Pos);
    Vec2 orient4 = node4Pos - pos;
    orient4.normalize();
    ActorsManager::spawnBullet(GameActor::AT_ENEMY_BULLET, node4Pos, orient4, 2.0f,"bullet2.png", Color3B(254,64,64), 0.5f, 0.6f);
    
    Vec2 node5Pos = m_pNode5->getPosition();
    node5Pos = convertToWorldSpace(node5Pos);
    Vec2 orient5 = node5Pos - pos;
    orient5.normalize();
    ActorsManager::spawnBullet(GameActor::AT_ENEMY_BULLET, node5Pos, orient5, 2.0f,"bullet2.png", Color3B(254,64,64), 0.5f, 0.6f);}

void ColorStar::initFirePos()
{
    Vec2 pos = getPosition();
    Vec2 orient = m_Orientation*m_fRadius*0.8f;
    m_pNode1 = Node::create();
    m_pNode1->setPosition(orient);
    addChild(m_pNode1);
    
    pos = getPosition();
    orient.rotate(Vec2::ZERO, M_PI*0.4f);
    m_pNode2 = Node::create();
    m_pNode2->setPosition(orient);
    addChild(m_pNode2);
    
    pos = getPosition();
    orient.rotate(Vec2::ZERO, M_PI*0.4f);
    m_pNode3 = Node::create();
    m_pNode3->setPosition(orient);
    addChild(m_pNode3);
    
    pos = getPosition();
    orient.rotate(Vec2::ZERO, M_PI*0.4f);
    m_pNode4 = Node::create();
    m_pNode4->setPosition(orient);
    addChild(m_pNode4);
    
    pos = getPosition();
    orient.rotate(Vec2::ZERO, M_PI*0.4f);
    m_pNode5 = Node::create();
    m_pNode5->setPosition(orient);
    addChild(m_pNode5);
}