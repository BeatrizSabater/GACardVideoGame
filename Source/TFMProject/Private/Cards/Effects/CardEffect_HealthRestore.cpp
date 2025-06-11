// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "Cards/Effects/CardEffect_HealthRestore.h"

#include "Units/UnitStats.h"

bool UCardEffect_HealthRestore::ApplyEffectToUnit(FUnitStats& UnitStats)
{
	UnitStats.CurrentHealth += HealthToRestore;

	return true;
}
