#pragma once

#include "CoreMinimal.h"
#include "ShieldUEMultiplayerTypes.generated.h"

UENUM(BlueprintType)
enum class EShieldUEViolationType : uint8
{
	Unknown UMETA(DisplayName = "Unknown"),
	ValueMismatch UMETA(DisplayName = "Value Mismatch"),
	RangeViolation UMETA(DisplayName = "Range Violation"),
	RateLimitExceeded UMETA(DisplayName = "Rate Limit Exceeded"),
	InvalidRequest UMETA(DisplayName = "Invalid Request"),
	AuthorityViolation UMETA(DisplayName = "Authority Violation"),
	ReplayDetected UMETA(DisplayName = "Replay Detected")
};

UENUM(BlueprintType)
enum class EShieldUESuspicionAction : uint8
{
	LogOnly UMETA(DisplayName = "Log Only"),
	RejectRequest UMETA(DisplayName = "Reject Request"),
	RestrictPlayer UMETA(DisplayName = "Restrict Player"),
	KickPlayer UMETA(DisplayName = "Kick Player")
};

USTRUCT(BlueprintType)
struct SHIELDUE_API FShieldUEViolationEvent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "ShieldUE|Multiplayer")
	FName RuleName = NAME_None;

	UPROPERTY(BlueprintReadOnly, Category = "ShieldUE|Multiplayer")
	EShieldUEViolationType Type = EShieldUEViolationType::Unknown;

	UPROPERTY(BlueprintReadOnly, Category = "ShieldUE|Multiplayer")
	float Severity = 1.0f;

	UPROPERTY(BlueprintReadOnly, Category = "ShieldUE|Multiplayer")
	FString Message;

	UPROPERTY(BlueprintReadOnly, Category = "ShieldUE|Multiplayer")
	float SuspicionScoreAfterEvent = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "ShieldUE|Multiplayer")
	bool bServerAuthority = false;

	UPROPERTY(BlueprintReadOnly, Category = "ShieldUE|Multiplayer")
	float TimeSeconds = 0.0f;
};

USTRUCT(BlueprintType)
struct SHIELDUE_API FShieldUEValidationRule
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Multiplayer")
	FName RuleName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Multiplayer")
	float SuspicionWeight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Multiplayer")
	float AllowedFloatTolerance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Multiplayer")
	int32 AllowedIntegerTolerance = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Multiplayer")
	int32 MaxViolationsBeforeRestriction = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Multiplayer")
	EShieldUESuspicionAction Action = EShieldUESuspicionAction::RejectRequest;
};
