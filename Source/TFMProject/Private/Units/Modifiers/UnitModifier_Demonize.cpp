// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "Units/Modifiers/UnitModifier_Demonize.h"

#include "TFMProject/TFMProject.h"


void UUnitModifier_Demonize::ActivateModifier(UObject* WorldContextObject, FUnitStats& UnitStats)
{
	if (AppliedUnitModifier.ModifierPoints == 0 || UnitStats.CalculatedReceivedAttack == 0)
	{
		UE_LOG(LogUnitModifiers, Warning, TEXT("UUnitModifier_Demonize::ActivateModifier - Demonize modifier was activated when Modifier Points were 0 or Calculated Received Attack was already 0."));
		return;
	}

	UE_LOG(LogUnitModifiers, VeryVerbose, TEXT("UUnitModifier_Demonize::ActivateModifier - Activated modifier %s. Remaining modifier points: %i."), *this->GetName(), AppliedUnitModifier.ModifierPoints - 1);

	UE_LOG(LogUnitModifiers, VeryVerbose,
		TEXT("UUnitModifier_Demonize::ActivateModifier - Unit calculated received set to %i. This was duplicated from the previous calculated received attack %i."),
		UnitStats.CalculatedReceivedAttack * 2, UnitStats.CalculatedReceivedAttack);

	UnitStats.CalculatedReceivedAttack *= 2;
	
	AppliedUnitModifier.ModifierPoints -= 1;

	if (AppliedUnitModifier.ModifierPoints == 0)
	{
		bShouldBeRemoved = true;
		UE_LOG(LogUnitModifiers, VeryVerbose, TEXT("UUnitModifier_Demonize::ActivateModifier - Demonize modifier points reached 0, marking for remove."));
	}
}
