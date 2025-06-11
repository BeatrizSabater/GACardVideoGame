// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "Units/Modifiers/UnitModifier_Shroom.h"

#include "TFMProject/TFMProject.h"


void UUnitModifier_Shroom::ActivateModifier(UObject* WorldContextObject, FUnitStats& UnitStats)
{
	if (AppliedUnitModifier.ModifierPoints == 0)
	{
		UE_LOG(LogUnitModifiers, Warning, TEXT("UUnitModifier_Shroom::ActivateModifier - Shroom modifier was activated when Modifier Points were 0."));
		return;
	}

	UE_LOG(LogUnitModifiers, VeryVerbose, TEXT("UUnitModifier_Shroom::ActivateModifier - Activated modifier %s. Remaining modifier points: %i."), *this->GetName(), AppliedUnitModifier.ModifierPoints - 1);

	const int32 PreviousHealth = UnitStats.CurrentHealth;

	UnitStats.CurrentHealth = FMath::Max(0 , PreviousHealth - AppliedUnitModifier.ModifierPoints);

	UE_LOG(LogUnitModifiers, VeryVerbose, TEXT("UUnitModifier_Shroom::ActivateModifier - Unit health set to %i. This was subtracted from the previous health %i."), UnitStats.CurrentHealth, PreviousHealth);
	
	AppliedUnitModifier.ModifierPoints -= 1;

	if (AppliedUnitModifier.ModifierPoints == 0)
	{
		bShouldBeRemoved = true;
		UE_LOG(LogUnitModifiers, VeryVerbose, TEXT("UUnitModifier_Shroom::ActivateModifier - Shroom modifier points reached 0, marking for remove."));
	}
}
