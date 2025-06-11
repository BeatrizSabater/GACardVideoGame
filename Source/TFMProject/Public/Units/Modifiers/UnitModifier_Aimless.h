// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "Units/UnitModifierAbstract.h"
#include "UnitModifier_Aimless.generated.h"

/**
 * 
 */
UCLASS()
class TFMPROJECT_API UUnitModifier_Aimless : public UUnitModifierAbstract
{
	GENERATED_BODY()
	
public:
	
	virtual void ActivateModifier(UObject* WorldContextObject, FUnitStats& UnitStats) override;
};
