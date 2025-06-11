// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "Units/Modifiers/UnitModifier_Shell.h"

#include "TFMProject/TFMProject.h"

void UUnitModifier_Shell::ActivateModifier(UObject* WorldContextObject, FUnitStats& UnitStats)
{
	if (AppliedUnitModifier.ModifierPoints == 0 || UnitStats.CalculatedReceivedAttack == 0)
	{
		UE_LOG(LogUnitModifiers, Warning, TEXT("UUnitModifier_Shell::ActivateModifier - Shell modifier was activated when Modifier Points were 0 or Calculated Received Attack was already 0."));
		return;
	}
	
	const int32 PreviousCalculatedReceivedAttack = UnitStats.CalculatedReceivedAttack;

	const int32 NewCalculatedReceivedAttack =  FMath::Max(0 , PreviousCalculatedReceivedAttack - AppliedUnitModifier.ModifierPoints);

	UnitStats.CalculatedReceivedAttack = NewCalculatedReceivedAttack;

	AppliedUnitModifier.ModifierPoints = FMath::Max(0 , AppliedUnitModifier.ModifierPoints - PreviousCalculatedReceivedAttack);

	UE_LOG(LogUnitModifiers, VeryVerbose, TEXT("UUnitModifier_Shell::ActivateModifier - Activated modifier %s. Remaining modifier points: %i."), *this->GetName(), AppliedUnitModifier.ModifierPoints);
	
	UE_LOG(LogUnitModifiers, VeryVerbose,
		TEXT("UUnitModifier_Shell::ActivateModifier - Unit calculated received attack set to %i. This was subtracted from the previous calculated received attack %i."),
		NewCalculatedReceivedAttack, PreviousCalculatedReceivedAttack);

	if (AppliedUnitModifier.ModifierPoints == 0)
	{
		bShouldBeRemoved = true;
		UE_LOG(LogUnitModifiers, VeryVerbose, TEXT("UUnitModifier_Shell::ActivateModifier - Shell modifier points reached 0, marking for remove."));
	}
}
