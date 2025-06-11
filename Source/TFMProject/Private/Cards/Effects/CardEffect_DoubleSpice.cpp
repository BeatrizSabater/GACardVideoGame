// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "Cards/Effects/CardEffect_DoubleSpice.h"

#include "Units/UnitModifierAbstract.h"
#include "Units/UnitUtils.h"
#include "Units/Modifiers/UnitModifier_Spice.h"

bool UCardEffect_DoubleSpice::ApplyEffectToUnit(FUnitStats& UnitStats)
{
	UUnitModifierAbstract* UnitModifierAbstract = UUnitUtils::GetUnitModifierByClass(UUnitModifier_Spice::StaticClass(), UnitStats);

	if (IsValid(UnitModifierAbstract))
	{
		UnitModifierAbstract->AppliedUnitModifier.ModifierPoints = UnitModifierAbstract->AppliedUnitModifier.ModifierPoints * 2;
	}

	return true;
}
