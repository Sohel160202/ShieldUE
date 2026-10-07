#include "ShieldUEAntiTamperComponent.h"

#include "ShieldUE.h"
#include "ShieldUEMultiplayerSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"

UShieldUEAntiTamperComponent::UShieldUEAntiTamperComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UShieldUEAntiTamperComponent::BeginPlay()
{
	Super::BeginPlay();
	RegisterWithSubsystem();
}

void UShieldUEAntiTamperComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			if (UShieldUEMultiplayerSubsystem* Subsystem = GameInstance->GetSubsystem<UShieldUEMultiplayerSubsystem>())
			{
				Subsystem->UnregisterAntiTamperComponent(this);
			}
		}
	}
	Super::EndPlay(EndPlayReason);
}

void UShieldUEAntiTamperComponent::RegisterWithSubsystem()
{
	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			if (UShieldUEMultiplayerSubsystem* Subsystem = GameInstance->GetSubsystem<UShieldUEMultiplayerSubsystem>())
			{
				Subsystem->RegisterAntiTamperComponent(this);
			}
		}
	}
}

bool UShieldUEAntiTamperComponent::HasServerAuthority() const
{
	return GetOwner() && GetOwner()->HasAuthority();
}

float UShieldUEAntiTamperComponent::GetSuspicionScore() const
{
	return SuspicionScore;
}

bool UShieldUEAntiTamperComponent::IsRestricted() const
{
	return bRestricted;
}

bool UShieldUEAntiTamperComponent::ValidateInt32(FName RuleName, int32 ClientValue, int32 ServerValue, int32 AllowedTolerance)
{
	if (bServerOnlyValidation && !HasServerAuthority()) return false;
	if (FMath::Abs(ClientValue - ServerValue) <= FMath::Max(0, AllowedTolerance)) return true;

	ReportViolation(RuleName, EShieldUEViolationType::ValueMismatch, 1.0f,
		FString::Printf(TEXT("Int32 mismatch: client=%d server=%d"), ClientValue, ServerValue));
	DispatchValidationFailure();
	return false;
}

bool UShieldUEAntiTamperComponent::ValidateInt64(FName RuleName, int64 ClientValue, int64 ServerValue, int64 AllowedTolerance)
{
	if (bServerOnlyValidation && !HasServerAuthority()) return false;
	const uint64 Difference = ClientValue >= ServerValue
		? static_cast<uint64>(ClientValue - ServerValue)
		: static_cast<uint64>(ServerValue - ClientValue);
	if (Difference <= static_cast<uint64>(FMath::Max<int64>(0, AllowedTolerance))) return true;

	ReportViolation(RuleName, EShieldUEViolationType::ValueMismatch, 1.0f,
		FString::Printf(TEXT("Int64 mismatch: client=%lld server=%lld"), ClientValue, ServerValue));
	DispatchValidationFailure();
	return false;
}

bool UShieldUEAntiTamperComponent::ValidateFloat(FName RuleName, float ClientValue, float ServerValue, float AllowedTolerance)
{
	if (bServerOnlyValidation && !HasServerAuthority()) return false;
	if (FMath::IsFinite(ClientValue) && FMath::IsFinite(ServerValue) && FMath::Abs(ClientValue - ServerValue) <= FMath::Max(0.0f, AllowedTolerance)) return true;

	ReportViolation(RuleName, EShieldUEViolationType::ValueMismatch, 1.0f,
		FString::Printf(TEXT("Float mismatch: client=%f server=%f"), ClientValue, ServerValue));
	DispatchValidationFailure();
	return false;
}

bool UShieldUEAntiTamperComponent::ValidateRateLimit(FName RuleName, int32 CallsInWindow, int32 MaximumCalls)
{
	if (bServerOnlyValidation && !HasServerAuthority()) return false;
	if (CallsInWindow <= MaximumCalls) return true;

	ReportViolation(RuleName, EShieldUEViolationType::RateLimitExceeded, 1.0f,
		FString::Printf(TEXT("Rate limit exceeded: calls=%d maximum=%d"), CallsInWindow, MaximumCalls));
	DispatchValidationFailure();
	return false;
}

void UShieldUEAntiTamperComponent::ReportViolation(FName RuleName, EShieldUEViolationType Type, float Severity, const FString& Message)
{
	if (bServerOnlyValidation && !HasServerAuthority()) return;

	SuspicionScore += FMath::Max(0.0f, Severity);
	if (SuspicionScore >= RestrictionThreshold) bRestricted = true;

	FShieldUEViolationEvent Event;
	Event.RuleName = RuleName;
	Event.Type = Type;
	Event.Severity = Severity;
	Event.Message = Message;
	Event.SuspicionScoreAfterEvent = SuspicionScore;
	Event.bServerAuthority = HasServerAuthority();
	OnViolationDetected.Broadcast(Event);

	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			if (UShieldUEMultiplayerSubsystem* Subsystem = GameInstance->GetSubsystem<UShieldUEMultiplayerSubsystem>())
			{
				Subsystem->RecordViolation(this, Event);
			}
		}
	}
}

void UShieldUEAntiTamperComponent::ReportValueRecovered(FName RuleName, const FString& Message)
{
	FShieldUEViolationEvent Event;
	Event.RuleName = RuleName;
	Event.Type = EShieldUEViolationType::ValueMismatch;
	Event.Message = Message;
	Event.SuspicionScoreAfterEvent = SuspicionScore;
	OnValueRecovered.Broadcast(Event);
}

void UShieldUEAntiTamperComponent::ResetIntegrityState()
{
	SuspicionScore = 0.0f;
	bRestricted = false;
}

void UShieldUEAntiTamperComponent::DispatchValidationFailure()
{
	OnValidationFailed.Broadcast();
}
