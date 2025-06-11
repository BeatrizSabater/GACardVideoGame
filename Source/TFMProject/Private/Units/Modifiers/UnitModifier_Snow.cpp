// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "Units/Modifiers/UnitModifier_Snow.h"

void UUnitModifier_Snow::ActivateModifier(UObject* WorldContextObject, FUnitStats& UnitStats)
{
	AppliedUnitModifier.ModifierPoints -= 1;

	if (AppliedUnitModifier.ModifierPoints == 0)
	{
		bShouldBeRemoved = true;
	}
}
