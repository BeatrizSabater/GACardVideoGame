// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "UnitModifierAbstract.h"
#include "UnitUtils.generated.h"

class UCardDataAsset;
struct FUnitStats;
class UUnitModifierAbstract;
/**
 * 
 */
UCLASS()
class TFMPROJECT_API UUnitUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static UUnitModifierAbstract* GetUnitModifierByClass(TSubclassOf<UUnitModifierAbstract> UnitModifierClass, const FUnitStats& UnitStats);

	UFUNCTION(BlueprintCallable)
	static UUnitModifierAbstract* GetUnitModifierByTriggerPhase(EUnitModifierTriggerPhase TriggerPhase, const FUnitStats& UnitStats);

	UFUNCTION()
	static void GetCardsFromUniqueIDs(const TArray<UCardDataAsset*>& SearchCardList, const TArray<int32>& InCardsUniqueIDs, TArray<UCardDataAsset*>& OutCards);

	UFUNCTION()
	static void GetUniqueIDsFromCards(const TArray<UCardDataAsset*>& CardList, TArray<int32>& OutCardsUniqueIDs);
};
