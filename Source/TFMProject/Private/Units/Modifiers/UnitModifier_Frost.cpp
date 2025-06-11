// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "Units/Modifiers/UnitModifier_Frost.h"

#include "TFMProject/TFMProject.h"


void UUnitModifier_Frost::ActivateModifier(UObject* WorldContextObject, FUnitStats& UnitStats)
{
	if (AppliedUnitModifier.ModifierPoints == 0 || UnitStats.CalculatedTurnAttack == 0)
	{
		UE_LOG(LogUnitModifiers, Warning, TEXT("UUnitModifier_Frost::ActivateModifier - Frost modifier was activated when Modifier Points were 0 or Calculated Turn Attack was already 0."));
		return;
	}

	UE_LOG(LogUnitModifiers, VeryVerbose, TEXT("UUnitModifier_Frost::ActivateModifier - Activated modifier %s. Remaining modifier points 0, Frost always is marked as remove after an activation."), *this->GetName());

	const int32 PreviousCalculatedTurnAttack = UnitStats.CalculatedTurnAttack;

	UnitStats.CalculatedTurnAttack = FMath::Max(0 , PreviousCalculatedTurnAttack - AppliedUnitModifier.ModifierPoints);

	UE_LOG(LogUnitModifiers, VeryVerbose,
		TEXT("UUnitModifier_Frost::ActivateModifier - Unit calculated turn attack set to %i. This was subtracted from the previous calculated turn attack %i."),
		UnitStats.CalculatedTurnAttack, PreviousCalculatedTurnAttack);

	AppliedUnitModifier.ModifierPoints = 0;
	bShouldBeRemoved = true;
}
