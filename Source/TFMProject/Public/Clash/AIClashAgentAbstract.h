// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AIClashAgentAbstract.generated.h"

class UCardDataAsset;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew, Abstract)
class TFMPROJECT_API UAIClashAgentAbstract : public UObject
{
	GENERATED_BODY()

public:

	virtual UCardDataAsset* PickCard(const UObject* WorldContextObject, TArray<UCardDataAsset*>& AvailableCards) PURE_VIRTUAL(UAIClashAgentAbstract::PickCard, return nullptr;);
};
