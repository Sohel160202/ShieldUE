#pragma once

#include "Components/ActorComponent.h"
#include "ShieldUEMultiplayerTypes.h"
#include "ShieldUEAntiTamperComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShieldUEViolationSignature, const FShieldUEViolationEvent&, Event);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShieldUEValidationFailedSignature);

UCLASS(ClassGroup = (ShieldUE), meta = (BlueprintSpawnableComponent))
class SHIELDUE_API UShieldUEAntiTamperComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UShieldUEAntiTamperComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Multiplayer")
	bool bServerOnlyValidation = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Multiplayer")
	float RestrictionThreshold = 10.0f;

	UPROPERTY(BlueprintAssignable, Category = "ShieldUE|Multiplayer|Events")
	FShieldUEViolationSignature OnViolationDetected;

	UPROPERTY(BlueprintAssignable, Category = "ShieldUE|Multiplayer|Events")
	FShieldUEViolationSignature OnValueRecovered;

	UPROPERTY(BlueprintAssignable, Category = "ShieldUE|Multiplayer|Events")
	FShieldUEValidationFailedSignature OnValidationFailed;

	UFUNCTION(BlueprintPure, Category = "ShieldUE|Multiplayer")
	bool HasServerAuthority() const;

	UFUNCTION(BlueprintPure, Category = "ShieldUE|Multiplayer")
	float GetSuspicionScore() const;

	UFUNCTION(BlueprintPure, Category = "ShieldUE|Multiplayer")
	bool IsRestricted() const;

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Multiplayer")
	bool ValidateInt32(FName RuleName, int32 ClientValue, int32 ServerValue, int32 AllowedTolerance = 0);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Multiplayer")
	bool ValidateInt64(FName RuleName, int64 ClientValue, int64 ServerValue, int64 AllowedTolerance = 0);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Multiplayer")
	bool ValidateFloat(FName RuleName, float ClientValue, float ServerValue, float AllowedTolerance = 0.0f);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Multiplayer")
	bool ValidateRateLimit(FName RuleName, int32 CallsInWindow, int32 MaximumCalls);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Multiplayer")
	void ReportViolation(FName RuleName, EShieldUEViolationType Type, float Severity, const FString& Message);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Multiplayer")
	void ReportValueRecovered(FName RuleName, const FString& Message);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Multiplayer")
	void ResetIntegrityState();

private:
	UPROPERTY()
	float SuspicionScore = 0.0f;

	UPROPERTY()
	bool bRestricted = false;

	UFUNCTION()
	void RegisterWithSubsystem();

	void DispatchValidationFailure();
};
