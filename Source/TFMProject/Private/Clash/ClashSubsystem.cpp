// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "Clash/ClashSubsystem.h"

#include "Clash/AIClashAgentAbstract.h"
#include "Clash/ClashArrangementDataAsset.h"
#include "Cards/CardDataAsset.h"
#include "Cards/CardEffectAbstract.h"
#include "TFMProject/TFMProject.h"
#include "Units/UnitModifierAbstract.h"
#include "Units/UnitUtils.h"

void UClashSubsystem::InjectClashArrangement(const UClashArrangementDataAsset* InClashArrangement)
{
	if (!IsValid(InClashArrangement))
	{
		UE_LOG(LogClashSystem, Error, TEXT("UClashSubsystem::InjectClashArrangement - Injected Clash Arrangement is not valid."));
		return;
	}

	UE_LOG(LogClashSystem, Log, TEXT("UClashSubsystem::InjectClashArrangement - Injecting Clash Arrangement %s."), *InClashArrangement->GetName());

	CurrentClashArrangement = InClashArrangement;
	AvailableCards = CurrentClashArrangement->ClashAvailableCards;
	PlayerUnits = CurrentClashArrangement->PlayerUnits;
	EnemyUnits = CurrentClashArrangement->EnemyUnits;

	ClashRandomStream.Initialize(CurrentClashArrangement->ClashRandomSeed == -1 ? FPlatformTime::Cycles() : CurrentClashArrangement->ClashRandomSeed);
}

FClashResult UClashSubsystem::StartAIClash(const TArray<int32>& InAvailableCardsID, int32 InConsecutiveExecutionNumber)
{
	if (!IsValid(CurrentClashArrangement) || !IsValid(CurrentClashArrangement->AIClashAgent))
	{
		UE_LOG(LogClashSystem, Error, TEXT("UClashSubsystem::StartAIClash - Couldn't start clash because some Clash Arrangement data is not valid."));
		return FClashResult();
	}
	
	ConsecutiveExecutionNumber = InConsecutiveExecutionNumber;

	StartClash();

	if (PlayerUnits.IsEmpty() || EnemyUnits.IsEmpty())
	{
		UE_LOG(LogClashSystem, Error, TEXT("UClashSubsystem::StartAIClash - Couldn't start clash because units are empty."));
		return FClashResult();
	}
	
	UUnitUtils::GetCardsFromUniqueIDs(AvailableCards, InAvailableCardsID, PlayableCards);
	
	while (ClashExecution == EClashExecution::Running && CurrentTurn <= 50)
	{
		UE_LOG(LogClashSystem, Verbose, TEXT("UClashSubsystem::StartAIClash - Execute turn %i."), CurrentTurn);

		if (PlayedCards.Num() <= PlayableCards.Num())
		{
			UCardDataAsset* PickedCard = AIAgentPickCard();
			if (!IsValid(PickedCard))
			{
				AbortClash();
				return FClashResult();
			}
			
			PlayCard(PickedCard);
		}

		PassTurn();
		
		CheckClashEndCondition();		
	}

	if (CurrentTurn > 50)
	{
		ClashExecution = EClashExecution::Defeat;
	}

	FClashResult ClashResult = FClashResult(ClashExecution, CurrentTurn, PlayerUnits, EnemyUnits);

	OnClashEndedDelegate.Broadcast(ClashResult);
	return ClashResult;
}

void UClashSubsystem::StartClash()
{
	UE_LOG(LogClashSystem, Verbose, TEXT("UClashSubsystem::StartClash."));

	ClashExecution = EClashExecution::Running;
	CurrentTurn = 1;

	PlayedCards.Empty();

	PlayerUnits.Empty();
	const int32 PlayerIndex = ConsecutiveExecutionNumber % CurrentClashArrangement->PlayerUnits.Num();

	PlayerUnits.Add(CurrentClashArrangement->PlayerUnits[PlayerIndex]);
	for (FUnitStats& PlayerUnit : PlayerUnits)
	{
		PlayerUnit.RestartTransientStats();
	}

	EnemyUnits.Empty();
	const int32 EnemyIndex = ConsecutiveExecutionNumber % CurrentClashArrangement->EnemyUnits.Num();
	
	EnemyUnits.Add(CurrentClashArrangement->EnemyUnits[EnemyIndex]);
	for (FUnitStats& EnemyUnit : EnemyUnits)
	{
		EnemyUnit.RestartTransientStats();
	}
}

int32 UClashSubsystem::GetClashUnitsCombinations()
{
	if (!IsValid(CurrentClashArrangement)) return 0;

	return CurrentClashArrangement->EnemyUnits.Num() * CurrentClashArrangement->PlayerUnits.Num();
}

void UClashSubsystem::CheckClashEndCondition()
{
	if (!IsValid(CurrentClashArrangement) || ClashExecution != EClashExecution::Running) return;
	
	if (PlayerUnits.IsEmpty() && EnemyUnits.IsEmpty())
	{
		ClashExecution = EClashExecution::Draw;
		UE_LOG(LogClashSystem, Verbose, TEXT("UClashSubsystem::CheckClashEndCondition - Clash execution will come to an end due to draw condition."));
	}
	else if (PlayerUnits.IsEmpty())
	{
		ClashExecution = EClashExecution::Defeat;
		UE_LOG(LogClashSystem, Verbose, TEXT("UClashSubsystem::CheckClashEndCondition - Clash execution will come to an end due to defeat condition."));
		UE_LOG(LogClashSystem, Verbose, TEXT("UClashSubsystem - Clash turns played %i."), CurrentTurn);
		UE_LOG(LogClashSystem, Verbose, TEXT("UClashSubsystem - Player survived with %i health."), EnemyUnits[0].CurrentHealth);
	}
	else if (EnemyUnits.IsEmpty())
	{
		ClashExecution = EClashExecution::Victory;
		UE_LOG(LogClashSystem, Verbose, TEXT("UClashSubsystem::CheckClashEndCondition - Clash execution will come to an end due to win condition."));
		UE_LOG(LogClashSystem, Verbose, TEXT("UClashSubsystem - Clash turns played %i."), CurrentTurn);
		UE_LOG(LogClashSystem, Verbose, TEXT("UClashSubsystem - Player survived with %i health."), PlayerUnits[0].CurrentHealth);
	}
}

void UClashSubsystem::PassTurn()
{
	if (ClashExecution != EClashExecution::Running) return;
	
	UpdateCounters();
	CurrentTurn++;
}

void UClashSubsystem::UpdateCounters()
{
	if (EnemyUnits.IsEmpty() || PlayerUnits.IsEmpty())
	{
		UE_LOG(LogClashSystem, Error, TEXT("UClashSubsystem::UpdateCounters - Enemy units array or player units array is empty."));
	}
	
	UE_LOG(LogClashSystem, Verbose, TEXT("UClashSubsystem::UpdateCounters - Updating enemy units counters."));
	for (FUnitStats& EnemyUnit : EnemyUnits)
	{
		AdvanceUnitCounter(EnemyUnit);
		
		if (EnemyUnit.CurrentCounter == 0)
		{
			ExecuteUnitAttack(EnemyUnit, PlayerUnits[0]);
			EnemyUnit.CurrentCounter = EnemyUnit.Counter;
			OnPlayerFrontUnitReceivedDamage();
		}
	}
	
	UE_LOG(LogClashSystem, Verbose, TEXT("UClashSubsystem::UpdateCounters - Updating player units counters."));
	for (FUnitStats& PlayerUnit : PlayerUnits)
	{
		AdvanceUnitCounter(PlayerUnit);

		if (PlayerUnit.CurrentCounter == 0)
		{
			ExecuteUnitAttack(PlayerUnit, EnemyUnits[0]);
			PlayerUnit.CurrentCounter = PlayerUnit.Counter;
			OnEnemyFrontUnitReceivedDamage();
		}
	}
}

void UClashSubsystem::AdvanceUnitCounter(FUnitStats& UnitStats)
{
	// Searching for and activating unit modifiers that trigger on turn ended
	auto UnitModifiersEndTurn = UnitStats.UnitModifiers.FilterByPredicate([](const UUnitModifierAbstract* UnitModifier)
	{
		return UnitModifier->AppliedUnitModifier.UnitModifier.ModifierTriggerPhase == EUnitModifierTriggerPhase::TurnEnded;
	});
	
	UnitModifiersEndTurn.Sort([](const UUnitModifierAbstract& A, const UUnitModifierAbstract& B)
	{
		return A.AppliedUnitModifier.UnitModifier.ModifierPriority < B.AppliedUnitModifier.UnitModifier.ModifierPriority;
	});
	
	for (auto UnitModifierEndTurn : UnitModifiersEndTurn)
	{
		if (!IsValid(UnitModifierEndTurn)) continue;
		
		UnitModifierEndTurn->ActivateModifier(this, UnitStats);
	}

	for (int32 i = UnitModifiersEndTurn.Num() - 1; i >= 0; i--)
	{
		UUnitModifierAbstract* UnitModifier = UnitModifiersEndTurn[i];
	
		if (UnitModifier->ShouldBeRemoved())
		{
			UE_LOG(LogClashSystem, Verbose, TEXT("UClashSubsystem::AdvanceUnitCounter - The modifier %s has been removed."), *UnitModifier->GetName());
			UnitStats.UnitModifiers.Remove(UnitModifier);
		}
	}

	// Checking for Counter Reduction modifiers that affect to the counters
	auto CounterReductionModifier = UUnitUtils::GetUnitModifierByTriggerPhase(EUnitModifierTriggerPhase::CounterReduction, UnitStats);
	
	if (IsValid(CounterReductionModifier))
	{
		UE_LOG(LogClashSystem, Verbose, TEXT("UClashSubsystem::AdvanceUnitCounter - Unit counters weren't update due to a unit modifier that trigger in the counter reduction phase."));
		CounterReductionModifier->ActivateModifier(this, UnitStats);
		return;
	}

	UnitStats.CurrentCounter -= 1;
	UE_LOG(LogClashSystem, Verbose, TEXT("UClashSubsystem::AdvanceUnitCounter - Unit counters updated to %i."), UnitStats.CurrentCounter);
}

void UClashSubsystem::ExecuteUnitAttack(FUnitStats& AttackerStats, FUnitStats& DefenderStats)
{
	UE_LOG(LogClashSystem, Verbose, TEXT("UClashSubsystem::ExecuteUnitAttack - Attacker base attack %i. Defender current health %i."), AttackerStats.AttackDamage, DefenderStats.CurrentHealth);
	
	// ON ATTACKING AND ON BEING ATTACKED MODIFIERS
	CalculateDefenderReceivedDamage(AttackerStats, DefenderStats);
	
	// DAMAGE UNIT
	DefenderStats.CurrentHealth = FMath::Max(0, DefenderStats.CurrentHealth + DefenderStats.CalculatedReceivedAttack * -1);

	UE_LOG(LogClashSystem, Verbose, TEXT("UClashSubsystem::ExecuteUnitAttack - Calculated defender received damage %i. New health %i."), DefenderStats.CalculatedReceivedAttack, DefenderStats.CurrentHealth);
	
	UpdateModifiersAfterAttack(AttackerStats);
	UpdateModifiersAfterAttack(DefenderStats);
}

void UClashSubsystem::CalculateDefenderReceivedDamage(FUnitStats& AttackerStats, FUnitStats& DefenderStats)
{
	// On attacking: calculates base damage
	AttackerStats.CalculatedTurnAttack = AttackerStats.AttackDamage;
	UE_LOG(LogClashSystem, Verbose, TEXT("UClashSubsystem::CalculateDefenderReceivedDamage - Calculating atackers damage applying %i attacking modifiers."), AttackerStats.UnitModifiers.Num());

	auto UnitModifiersAttacking= AttackerStats.UnitModifiers.FilterByPredicate([](const UUnitModifierAbstract* UnitModifier)
	{
		return UnitModifier->AppliedUnitModifier.UnitModifier.ModifierTriggerPhase == EUnitModifierTriggerPhase::Attacking;
	});
	
	UnitModifiersAttacking.Sort([](const UUnitModifierAbstract& A, const UUnitModifierAbstract& B)
	{
		return A.AppliedUnitModifier.UnitModifier.ModifierPriority < B.AppliedUnitModifier.UnitModifier.ModifierPriority;
	});

	for (auto UnitModifierAttacking : UnitModifiersAttacking)
	{
		if (!IsValid(UnitModifierAttacking)) continue;
		
		UnitModifierAttacking->ActivateModifier(this, AttackerStats);
	}

	// No damage has been done
	if (AttackerStats.CalculatedTurnAttack == 0)
	{
		UE_LOG(LogClashSystem, Warning, TEXT("UClashSubsystem::CalculateDefenderReceivedDamage - Calculated turn attack after attacking modifiers is 0, which is strange."));
		return;
	}

	// On being attacked: calculates defense (if there are modifiers of this class)
	DefenderStats.CalculatedReceivedAttack = AttackerStats.CalculatedTurnAttack;
	UE_LOG(LogClashSystem, Verbose, TEXT("UClashSubsystem::CalculateDefenderReceivedDamage - Calculating defender received attack applying %i being attacked modifiers."), DefenderStats.UnitModifiers.Num());
	
	auto UnitModifiersBeingAttacked= DefenderStats.UnitModifiers.FilterByPredicate([](const UUnitModifierAbstract* UnitModifier)
	{
		return UnitModifier->AppliedUnitModifier.UnitModifier.ModifierTriggerPhase == EUnitModifierTriggerPhase::BeingAttacked;
	});
	
	UnitModifiersBeingAttacked.Sort([](const UUnitModifierAbstract& A, const UUnitModifierAbstract& B)
	{
		return A.AppliedUnitModifier.UnitModifier.ModifierPriority < B.AppliedUnitModifier.UnitModifier.ModifierPriority;
	});

	for (auto UnitModifierBeingAttacked : UnitModifiersBeingAttacked)
	{
		if (!IsValid(UnitModifierBeingAttacked)) continue;
		
		UnitModifierBeingAttacked->ActivateModifier(this, DefenderStats);

		// If damage is already 0, we don't apply more being attacked modifiers
		if (DefenderStats.CalculatedReceivedAttack <= 0) break;
	}
}

void UClashSubsystem::UpdateModifiersAfterAttack(FUnitStats& UnitStats)
{
	UE_LOG(LogClashSystem, Verbose, TEXT("UClashSubsystem::UpdateModifiersAfterAttack."));
	
	for (int32 i = UnitStats.UnitModifiers.Num() - 1; i >= 0; i--)
	{
		UUnitModifierAbstract* UnitModifier = UnitStats.UnitModifiers[i];
	
		if (UnitModifier->ShouldBeRemoved())
		{
			UE_LOG(LogClashSystem, Verbose, TEXT("UClashSubsystem::UpdateModifiersAfterAttack - The modifier %s has been removed."), *UnitModifier->GetName());
			UnitStats.UnitModifiers.Remove(UnitModifier);
		}
	}
}

void UClashSubsystem::OnPlayerFrontUnitReceivedDamage()
{
	if (PlayerUnits.IsEmpty())
	{
		UE_LOG(LogClashSystem, Error, TEXT("UClashSubsystem::OnPlayerFrontUnitReceivedDamage - Player units array is empty."));
		return;
	}

	if (PlayerUnits[0].CurrentHealth <= 0)
	{
		UE_LOG(LogClashSystem, Verbose, TEXT("UClashSubsystem::OnPlayerFrontUnitReceivedDamage - Player's current health is 0, removing player unit."));
		PlayerUnits.RemoveAt(0);
	}
}

void UClashSubsystem::OnEnemyFrontUnitReceivedDamage()
{
	if (EnemyUnits.IsEmpty())
	{
		UE_LOG(LogClashSystem, Error, TEXT("UClashSubsystem::OnEnemyFrontUnitReceivedDamage - Enemy units array is empty."));
		return;
	}

	if (EnemyUnits[0].CurrentHealth <= 0)
	{
		UE_LOG(LogClashSystem, Verbose, TEXT("UClashSubsystem::OnEnemyFrontUnitReceivedDamage - Enemy's current health is 0, removing enemy unit."));
		EnemyUnits.RemoveAt(0);
	}
}

UCardDataAsset* UClashSubsystem::AIAgentPickCard()
{
	// TODO: AGENT SELECTS TARGET?
	UCardDataAsset* PickedCard = CurrentClashArrangement->AIClashAgent->PickCard(this, PlayableCards);

	if (!IsValid(PickedCard))
	{
		UE_LOG(LogClashSystem, Error, TEXT("UClashSubsystem::AIAgentPickCard -- PickedCard was not found in the list of cards."));
	}
	else
	{
		UE_LOG(LogClashSystem, VeryVerbose, TEXT("UClashSubsystem::AIAgentPickCard - Picked Card ID by AI Agent was %i, with name %s and %i card effects."), PickedCard->UniqueID, *PickedCard->Name.ToString(), PickedCard->Effects.Num());
	}
	
	return PickedCard;
}

void UClashSubsystem::PlayCard(UCardDataAsset* InCardDataAsset)
{
	if (!IsValid(InCardDataAsset)) return;
	
	PlayedCards.Add(InCardDataAsset);

	// Reduce health + check victory condition
	if (InCardDataAsset->AttackPoints > 0)
	{
		EnemyUnits[0].CurrentHealth = EnemyUnits[0].CurrentHealth - InCardDataAsset->AttackPoints;

		if (EnemyUnits[0].CurrentHealth <= 0)
		{
			ClashExecution = EClashExecution::Victory;
			return;
		}
	}
	
	for (int32 i = 0; i < InCardDataAsset->Effects.Num(); i++)
	{
		UCardEffectAbstract* CardEffect = InCardDataAsset->Effects[i];
			
		if (!IsValid(CardEffect))
		{
			UE_LOG(LogClashSystem, Warning, TEXT("UClashSubsystem::StartAIClash - A not valid effect at %i index was found in the card %s."), i, *InCardDataAsset->Name.ToString());
			continue;
		}
			
		if (InCardDataAsset->bTargetsPlayer)
		{
			UE_LOG(LogClashSystem, Verbose, TEXT("UClashSubsystem::StartAIClash - Applying card effect %s to player unit."), *CardEffect->GetName());
			CardEffect->ApplyEffectToUnit(PlayerUnits[0]);
		}
		else
		{
			UE_LOG(LogClashSystem, Verbose, TEXT("UClashSubsystem::StartAIClash - Applying card effect %s to enemy unit."), *CardEffect->GetName());
			CardEffect->ApplyEffectToUnit(EnemyUnits[0]);
		}
	}
}

void UClashSubsystem::AbortClash()
{
	UE_LOG(LogClashSystem, Error, TEXT("UClashSubsystem::AbortClash."));
	
	ClashExecution = EClashExecution::Failed;
}
