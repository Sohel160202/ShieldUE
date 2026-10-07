#include "ShieldUEMultiplayerSubsystem.h"

#include "ShieldUE.h"
#include "ShieldUEAntiTamperComponent.h"

void UShieldUEMultiplayerSubsystem::Deinitialize()
{
	RegisteredComponents.Reset();
	Super::Deinitialize();
}

void UShieldUEMultiplayerSubsystem::RegisterAntiTamperComponent(UShieldUEAntiTamperComponent* Component)
{
	if (IsValid(Component) && !RegisteredComponents.Contains(Component))
	{
		RegisteredComponents.Add(Component);
	}
}

void UShieldUEMultiplayerSubsystem::UnregisterAntiTamperComponent(UShieldUEAntiTamperComponent* Component)
{
	RegisteredComponents.Remove(Component);
}

bool UShieldUEMultiplayerSubsystem::IsServerAuthority() const
{
	const UWorld* World = GetWorld();
	return World && World->GetAuthGameMode() != nullptr;
}

bool UShieldUEMultiplayerSubsystem::IsComponentRegistered(const UShieldUEAntiTamperComponent* Component) const
{
	return IsValid(Component) && RegisteredComponents.Contains(Component);
}

float UShieldUEMultiplayerSubsystem::GetSuspicionScore(const UShieldUEAntiTamperComponent* Component) const
{
	return IsValid(Component) ? Component->GetSuspicionScore() : 0.0f;
}

void UShieldUEMultiplayerSubsystem::ClearSuspicionScore(UShieldUEAntiTamperComponent* Component)
{
	if (IsValid(Component))
	{
		Component->ResetIntegrityState();
	}
}

void UShieldUEMultiplayerSubsystem::RecordViolation(UShieldUEAntiTamperComponent* Component, FShieldUEViolationEvent Event)
{
	if (!IsValid(Component)) return;

	Event.bServerAuthority = IsServerAuthority();
	Event.TimeSeconds = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
	UE_LOG(LogShieldUE, Warning, TEXT("ShieldUE multiplayer violation: Rule=%s Type=%d Severity=%.2f Message=%s"),
		*Event.RuleName.ToString(), static_cast<int32>(Event.Type), Event.Severity, *Event.Message);
}
