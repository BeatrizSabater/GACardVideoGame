// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "Cards/Effects/CardEffect_AttackBoost.h"

#include "Units/UnitStats.h"

bool UCardEffect_AttackBoost::ApplyEffectToUnit(FUnitStats& UnitStats)
{
	UnitStats.AttackDamage += AttackBoost;

	return true;
}
