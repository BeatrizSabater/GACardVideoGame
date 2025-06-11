// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Units/UnitStats.h"
#include "ClashSubsystem.generated.h"

class UCardDataAsset;
class UClashArrangementDataAsset;

UENUM(BlueprintType)
enum class EClashExecution : uint8
{
	None,
	Running,
	Failed,
	Victory,
	Defeat,
	Draw
};

USTRUCT(BlueprintType)
struct TFMPROJECT_API FClashResult
{
	GENERATED_BODY()

	FClashResult(){}

	FClashResult(EClashExecution InClashExecutionResult, int32 InPlayedTurns, const TArray<FUnitStats>& InAlivePlayerUnits, const TArray<FUnitStats>& InAliveEnemyUnits)
	: ClashExecutionResult(InClashExecutionResult), PlayedTurns(InPlayedTurns), AlivePlayerUnits(InAlivePlayerUnits), AliveEnemyUnits(InAliveEnemyUnits)
	{}

	UPROPERTY(BlueprintReadOnly)
	EClashExecution ClashExecutionResult = EClashExecution::Failed;
	
	UPROPERTY(BlueprintReadOnly)
	int32 PlayedTurns = 0;

	UPROPERTY(BlueprintReadOnly)
	TArray<FUnitStats> AlivePlayerUnits;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FUnitStats> AliveEnemyUnits;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClashEndedSignature, const FClashResult&, ClashResult);

UCLASS()
class TFMPROJECT_API UClashSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void InjectClashArrangement(const UClashArrangementDataAsset* InClashArrangement);
	
	UFUNCTION(BlueprintCallable)
	FClashResult StartAIClash(const TArray<int32>& InAvailableCardsID, int32 InConsecutiveExecutionNumber = 0);

	UFUNCTION(BlueprintCallable)
	const TArray<UCardDataAsset*>& GetAvailableCards() const { return AvailableCards; }
	
	UFUNCTION(BlueprintCallable)
	const TArray<UCardDataAsset*>& GetPlayedCards() const { return PlayedCards; }

	UFUNCTION(BlueprintCallable)
	const TArray<FUnitStats>& GetPlayerUnits() const { return PlayerUnits; }

	UFUNCTION(BlueprintCallable)
	const TArray<FUnitStats>& GetEnemyUnits() const { return EnemyUnits; }

	int32 GetClashUnitsCombinations();

	UPROPERTY(BlueprintAssignable)
	FOnClashEndedSignature OnClashEndedDelegate;

	UFUNCTION(BlueprintCallable)
	const FRandomStream& GetClashRandomStream() const { return ClashRandomStream; };

private:

	UPROPERTY()
	const UClashArrangementDataAsset* CurrentClashArrangement = nullptr;

	void CheckClashEndCondition();

	EClashExecution ClashExecution = EClashExecution::None;
	int32 CurrentTurn = 1;
	TArray<UCardDataAsset*> AvailableCards;
	TArray<UCardDataAsset*> PlayableCards;
	TArray<UCardDataAsset*> PlayedCards;
	TArray<FUnitStats> PlayerUnits;
	TArray<FUnitStats> EnemyUnits;

	void PassTurn();

	void UpdateCounters();
	void AdvanceUnitCounter(FUnitStats& UnitStats);
	void ExecuteUnitAttack(FUnitStats& AttackerStats, FUnitStats& DefenderStats);
	void CalculateDefenderReceivedDamage(FUnitStats& AttackerStats, FUnitStats& DefenderStats);
	void UpdateModifiersAfterAttack(FUnitStats& UnitStats);
	void OnPlayerFrontUnitReceivedDamage();
	void OnEnemyFrontUnitReceivedDamage();

	void StartClash();

	UCardDataAsset* AIAgentPickCard();
	void PlayCard(UCardDataAsset* InCardDataAsset);

	void AbortClash();

	FRandomStream ClashRandomStream;

	int32 ConsecutiveExecutionNumber = 0;

};
