// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "Units/Modifiers/UnitModifier_Spice.h"

#include "TFMProject/TFMProject.h"

void UUnitModifier_Spice::ActivateModifier(UObject* WorldContextObject, FUnitStats& UnitStats)
{
	if (AppliedUnitModifier.ModifierPoints == 0 || UnitStats.CalculatedTurnAttack == 0)
	{
		UE_LOG(LogUnitModifiers, Warning, TEXT("UUnitModifier_Spice::ActivateModifier - Spice modifier was activated when Modifier Points were 0 or Calculated Turn Attack was already 0."));
		return;
	}

	UE_LOG(LogUnitModifiers, VeryVerbose, TEXT("UUnitModifier_Spice::ActivateModifier - Activated modifier %s. Remaining modifier points 0, Spice always is marked as remove after an activation."), *this->GetName());

	const int32 PreviousCalculatedTurnAttack = UnitStats.CalculatedTurnAttack;
	
	UnitStats.CalculatedTurnAttack = PreviousCalculatedTurnAttack + AppliedUnitModifier.ModifierPoints; ;

	UE_LOG(LogUnitModifiers, VeryVerbose,
		TEXT("UUnitModifier_Spice::ActivateModifier - Unit calculated turn attack set to %i. This was sum up from the previous calculated turn attack %i."),
		UnitStats.CalculatedTurnAttack, PreviousCalculatedTurnAttack);

	AppliedUnitModifier.ModifierPoints = 0;
	bShouldBeRemoved = true;
}
