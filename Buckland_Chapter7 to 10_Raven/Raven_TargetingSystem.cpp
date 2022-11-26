#include "Raven_TargetingSystem.h"
#include "Raven_Bot.h"
#include "Raven_SensoryMemory.h"



//-------------------------------- ctor ---------------------------------------
//-----------------------------------------------------------------------------
Raven_TargetingSystem::Raven_TargetingSystem(Raven_Bot* owner):m_pOwner(owner),
                                                               m_pCurrentTarget(0)
{}



//----------------------------- Update ----------------------------------------

//-----------------------------------------------------------------------------
void Raven_TargetingSystem::Update()
{
  double ClosestDistSoFar = MaxDouble;
  m_pCurrentTarget       = 0;

  //grab a list of all the opponents the owner can sense
  std::list<Raven_Bot*> SensedBots;
  SensedBots = m_pOwner->GetSensoryMem()->GetListOfRecentlySensedOpponents();
  
  int MaxDamage = 0;
  std::list<Raven_Bot*>::const_iterator curBot = SensedBots.begin();
  for (curBot; curBot != SensedBots.end(); ++curBot)
  {
    if ((*curBot)->isAlive() && (*curBot != m_pOwner) ) //타겟이 살아 있고 자기 자신이 아닐때 확인
    {
      int damage = m_pOwner->GetSensoryMem()->GetSumDamage((*curBot)->ID());
      
      if (MaxDamage < damage)
      {
          MaxDamage = damage;
          m_pCurrentTarget = *curBot;
      }
    }
  }

  //누적 데미지가 없을 경우 (모든 봇이 HP 100일때)
  if (MaxDamage == 0)
  {
      std::list<Raven_Bot*>::const_iterator curBot = SensedBots.begin();
      for (curBot; curBot != SensedBots.end(); ++curBot)
      {
          if ((*curBot)->isAlive() && (*curBot != m_pOwner)) //타겟이 살아 있고 자기 자신이 아닐때 확인
          {
              double dist = Vec2DDistanceSq((*curBot)->Pos(), m_pOwner->Pos());

              if (dist < ClosestDistSoFar)
              {
                  ClosestDistSoFar = dist;
                  m_pCurrentTarget = *curBot;
              }
          }
      }
  }
}




bool Raven_TargetingSystem::isTargetWithinFOV()const
{
  return m_pOwner->GetSensoryMem()->isOpponentWithinFOV(m_pCurrentTarget);
}

bool Raven_TargetingSystem::isTargetShootable()const
{
  return m_pOwner->GetSensoryMem()->isOpponentShootable(m_pCurrentTarget);
}

Vector2D Raven_TargetingSystem::GetLastRecordedPosition()const
{
  return m_pOwner->GetSensoryMem()->GetLastRecordedPositionOfOpponent(m_pCurrentTarget);
}

double Raven_TargetingSystem::GetTimeTargetHasBeenVisible()const
{
  return m_pOwner->GetSensoryMem()->GetTimeOpponentHasBeenVisible(m_pCurrentTarget);
}

double Raven_TargetingSystem::GetTimeTargetHasBeenOutOfView()const
{
  return m_pOwner->GetSensoryMem()->GetTimeOpponentHasBeenOutOfView(m_pCurrentTarget);
}
