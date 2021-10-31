#include "ActorComponents/BaseAttributeSet.h"

UBaseAttributeSet::UBaseAttributeSet():
// 生命值
CurHealth(100.f)
, MaxHealth(100.f)
, HealthRegen(1.f)
// 体力值
, CurStamina(100.f)
, MaxStamina(100.f)
, StaminaRegen(10.0f)
// 魔力值
, CurMana(100.f)
, MaxMana(100.f)
, ManaRegen(1.0f)

// 基础能力值
, Power(5.0f)
, Sprit(5.0f)
, Body(5.0f)
, Agile(5.0f)

// 基础战力
, Aggressivity(5.0f)
, Defense(5.0f)
{};