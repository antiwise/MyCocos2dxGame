//
//  SlimeKing.cpp
//  Voxel_Explorer
//
//  Created by wang haibo on 15/10/22.
//
//

#include "SlimeKing.hpp"
#include "LevelResourceManager.h"
#include "OutlineEffect3D.h"
#include "VoxelExplorer.h"
#include "SewerBossLevel.hpp"
#include "FakeShadow.hpp"
#include "UtilityHelper.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;

SlimeKing* SlimeKing::create(BaseBoss::BossType type)
{
    std::string model = LevelResourceManager::getInstance()->getMonsterModelRes(BOSS_MODEL_NAMES[type]);
    auto boss = new (std::nothrow) SlimeKing();
    if (boss && boss->initWithFile(model) && boss->createFakeShadow())
    {
        boss->m_Type = type;
        boss->setCameraMask((unsigned int)CameraFlag::USER1);
        boss->setLightMask((unsigned int)LightFlag::LIGHT0);
        
        OutlineEffect3D* outline = OutlineEffect3D::create();
        outline->setOutlineColor(Vec3(1.0f, 1.0f, 1.0f));
        outline->setOutlineWidth(0.03f);
        boss->addEffect(outline, 1);
        
        boss->autorelease();
        return boss;
    }
    CC_SAFE_DELETE(boss);
    return nullptr;
}
SlimeKing::SlimeKing()
{
    m_Type = BT_SLIMEKING;
    
    m_nFOV = 7;
}
SlimeKing::~SlimeKing()
{
}
void SlimeKing::onEnterSkill1Stage()
{
    SewerBossLevel* level = dynamic_cast<SewerBossLevel*>(VoxelExplorer::getInstance()->getCurrentLevel());
    if(level)
    {
        level->createSummoningMonstersBySlimeKing(getPosInMap(), 1);
    }
}
void SlimeKing::onEnterSkill2Stage()
{
    if(m_pBossProperty)
        m_pBossProperty->addAttackAddBuffer(PlayerBuffer::PB_POISONING);
    SewerBossLevel* level = dynamic_cast<SewerBossLevel*>(VoxelExplorer::getInstance()->getCurrentLevel());
    if(level)
    {
        level->createSummoningMonstersBySlimeKing(getPosInMap(), 2);
    }
    if(getEffectCount() > 0)
    {
        OutlineEffect3D* outline = dynamic_cast<OutlineEffect3D*>(getEffect(0));
        if(outline)
        {
            Vec3 color = Vec3(Color3B::ORANGE.r/255.0f, Color3B::ORANGE.g/255.0f, Color3B::ORANGE.b/255.0f);
            outline->setOutlineColor(color);
        }
    }
}
void SlimeKing::onEnterSkill3Stage()
{
    SewerBossLevel* level = dynamic_cast<SewerBossLevel*>(VoxelExplorer::getInstance()->getCurrentLevel());
    if(level)
    {
        level->createSummoningMonstersBySlimeKing(getPosInMap(), 3);
    }
    if(getEffectCount() > 0)
    {
        OutlineEffect3D* outline = dynamic_cast<OutlineEffect3D*>(getEffect(0));
        if(outline)
        {
            Vec3 color = Vec3(Color3B::RED.r/255.0f, Color3B::RED.g/255.0f, Color3B::RED.b/255.0f);
            outline->setOutlineColor(color);
        }
    }
}
void SlimeKing::onEnterDeath()
{
    if(m_pBossProperty)
        m_pBossProperty->removeAttackAddBuffer(PlayerBuffer::PB_POISONING);
    if(getEffectCount() > 0)
    {
        OutlineEffect3D* outline = dynamic_cast<OutlineEffect3D*>(getEffect(0));
        if(outline)
        {
            Color3B outlineColor = UtilityHelper::randomColor();
            outline->setOutlineColor(Vec3(outlineColor.r/255.0f, outlineColor.g/255.0f, outlineColor.b/255.0f));
        }
    }
    setPosition3D(Vec3(Vec3(m_LastPosInMap.x*TerrainTile::CONTENT_SCALE, -0.5f*TerrainTile::CONTENT_SCALE, -m_LastPosInMap.y*TerrainTile::CONTENT_SCALE)));
    this->stopAllActions();
    removeTerrainTileFlag(TileInfo::ATTACKABLE);
    removeTerrainTileFlagByPos(TileInfo::ATTACKABLE, m_NextPos);
    this->setVisible(false);
    if(m_pFakeShadow)
        m_pFakeShadow->setVisible(false);
    VoxelExplorer::getInstance()->addParticle3DEffect(getPosition3D(), P3D_EFFECT_TYPE::P3D_PLAYER_DEATH);
    VoxelExplorer::getInstance()->generatePickItemByBoss(getPosInMap(), (int)m_pBossProperty->getValueCopper().GetLongValue());
    
    SewerBossLevel* level = dynamic_cast<SewerBossLevel*>(VoxelExplorer::getInstance()->getCurrentLevel());
    if(level)
        level->clearBossRoom();
    
    std::string soundName = LevelResourceManager::getInstance()->getMonsterSoundEffectRes(BOSS_MODEL_NAMES[m_Type], "DEATH");
    SimpleAudioEngine::getInstance()->playEffect(soundName.c_str());
}
bool SlimeKing::isPlayerInsideBossRoom()
{
    return VoxelExplorer::getInstance()->checkBossRoomDoorClosed();
}
void SlimeKing::attackedByPlayer(bool miss)
{
    if(isPlayerInsideBossRoom())
        BaseBoss::attackedByPlayer(miss);
    else
        BaseBoss::attackedByPlayer(true);
}