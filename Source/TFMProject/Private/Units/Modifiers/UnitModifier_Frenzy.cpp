// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "Units/Modifiers/UnitModifier_Frenzy.h"

#include "TFMProject/TFMProject.h"


void UUnitModifier_Frenzy::ActivateModifier(UObject* WorldContextObject, FUnitStats& UnitStats)
{
	if (AppliedUnitModifier.ModifierPoints == 0 || UnitStats.CalculatedTurnAttack == 0)
	{
		UE_LOG(LogUnitModifiers, Warning, TEXT("UUnitModifier_Frenzy::ActivateModifier - Frenzy modifier was activated when Modifier Points were 0."));
		return;
	}

	UE_LOG(LogUnitModifiers, VeryVerbose, TEXT("UUnitModifier_Frenzy::ActivateModifier - Activated modifier %s."), *this->GetName());

	UE_LOG(LogUnitModifiers, VeryVerbose,
		TEXT("UUnitModifier_Frenzy::ActivateModifier - Unit calculated turn attack set to %i. This was multiplied by the ModifiersPoints (%i) from the previous calculated turn attack %i."),
		AppliedUnitModifier.ModifierPoints + 1, UnitStats.CalculatedTurnAttack * (AppliedUnitModifier.ModifierPoints + 1), UnitStats.CalculatedTurnAttack);

	UnitStats.CalculatedTurnAttack *= AppliedUnitModifier.ModifierPoints + 1;

	AppliedUnitModifier.ModifierPoints = 0;
	
	bShouldBeRemoved = true;
	
	UE_LOG(LogUnitModifiers, VeryVerbose, TEXT("UUnitModifier_Block::ActivateModifier - Block modifier points reached 0, marking for remove."));
}
