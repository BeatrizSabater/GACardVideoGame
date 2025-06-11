// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "Units/UnitStats.h"


void FUnitStats::RestartTransientStats()
{
	CurrentHealth = Health;
	CurrentCounter = Counter;
	CalculatedTurnAttack = 0;
	CalculatedReceivedAttack = 0;
}
