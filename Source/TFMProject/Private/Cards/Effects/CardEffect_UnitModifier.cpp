// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "Cards/Effects/CardEffect_UnitModifier.h"

#include "Units/UnitModifierAbstract.h"
#include "Units/UnitStats.h"
#include "Units/UnitUtils.h"

bool UCardEffect_UnitModifier::ApplyEffectToUnit(FUnitStats& UnitStats)
{
	FAppliedUnitModifier AppliedUnitModifier = FAppliedUnitModifier(UnitModifierRow, UnitModifierEffectPoints);
	
	UUnitModifierAbstract* UnitModifierAbstract = UUnitUtils::GetUnitModifierByClass(AppliedUnitModifier.UnitModifier.ModifierClass, UnitStats);

	if (IsValid(UnitModifierAbstract))
	{
		UnitModifierAbstract->AppliedUnitModifier.ModifierPoints = UnitModifierAbstract->AppliedUnitModifier.ModifierPoints + AppliedUnitModifier.ModifierPoints;
	}
	else
	{
		UnitModifierAbstract = NewObject<UUnitModifierAbstract>(this, AppliedUnitModifier.UnitModifier.ModifierClass);
		UnitModifierAbstract->AppliedUnitModifier = AppliedUnitModifier;
		UnitStats.UnitModifiers.Emplace(UnitModifierAbstract);
	}
	
	return true;
}
