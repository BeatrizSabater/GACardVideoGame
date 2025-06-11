// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "GeneticAlgorithmReport.h"
#include "Subsystems/WorldSubsystem.h"
#include "GAReporterSubsystem.generated.h"

class UGAOptimizerSubsystem;
class UGAConfigDataAsset;


UCLASS()
class TFMPROJECT_API UGAReporterSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintPure)
	const FGAReport& GetCurrentRunReport() const { return CurrentRunReport; }
	
	UFUNCTION(BlueprintCallable)
	bool LoadReport(const FString& SlotName = "GARunHistory", int32 UserIndex = 0);

private:
	
	bool bIsRunningReport = false;

	FGAReport CurrentRunReport;
	FGAGenerationReport ActiveGenerationReport;

	UPROPERTY(Transient)
	TObjectPtr<UGAOptimizerSubsystem> OptimizerSubsystemRef;

	UFUNCTION()
	void HandleOptimizationStarted(const UGAConfigDataAsset* RunParams);
	UFUNCTION()
	void HandleOptimizationFinished(const FGAIndividual& OverallBestIndividual, bool bCompletedSuccessfully);
	UFUNCTION()
	void HandleGenerationStarted(const int32 GenerationNumber);
	UFUNCTION()
	void HandleParentsSelected(const TArray<FGAIndividual>& SelectedParents);
	UFUNCTION()
	void HandleCrossoverPerformed(const TArray<FGACrossoverReportEntry>& CrossoverEvents);
	UFUNCTION()
	void HandleMutationPerformed(const TArray<FGAMutationReportEntry>& MutationEvents);
	UFUNCTION()
	void HandleGenerationFinished(int32 GenerationNumber, const FGAIndividual& BestOfGeneration, float AverageFitness, const TArray<FGAIndividual>& FinalPopulation);
	
	UFUNCTION()
	bool SaveReport(const FString& SlotName = "GARunHistory", int32 UserIndex = 0);
	
	void SubscribeToDelegates();
	void UnsubscribeFromDelegates();
	void ResetReport();

};
