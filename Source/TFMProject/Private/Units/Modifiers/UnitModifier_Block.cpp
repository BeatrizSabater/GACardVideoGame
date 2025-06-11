// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "Units/Modifiers/UnitModifier_Block.h"

#include "TFMProject/TFMProject.h"

void UUnitModifier_Block::ActivateModifier(UObject* WorldContextObject, FUnitStats& UnitStats)
{
	if (AppliedUnitModifier.ModifierPoints == 0 || UnitStats.CalculatedReceivedAttack == 0)
	{
		UE_LOG(LogUnitModifiers, Warning, TEXT("UUnitModifier_Block::ActivateModifier - Block modifier was activated when Modifier Points were 0 or Calculated Received Attack was 0."));
		return;
	}

	UE_LOG(LogUnitModifiers, VeryVerbose, TEXT("UUnitModifier_Block::ActivateModifier - Activated modifier %s. Remaining modifier points: %i."), *this->GetName(), AppliedUnitModifier.ModifierPoints - 1);

	UnitStats.CalculatedReceivedAttack = 0;

	UE_LOG(LogUnitModifiers, VeryVerbose, TEXT("UUnitModifier_Block::ActivateModifier - Unit calculated received attack was set to 0."));

	AppliedUnitModifier.ModifierPoints -= 1;

	if (AppliedUnitModifier.ModifierPoints == 0)
	{
		bShouldBeRemoved = true;
		UE_LOG(LogUnitModifiers, VeryVerbose, TEXT("UUnitModifier_Block::ActivateModifier - Block modifier points reached 0, marking for remove."));
	}
}
