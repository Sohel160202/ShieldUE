#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "ShieldUEMultiplayerTypes.h"
#include "ShieldUEMultiplayerSubsystem.generated.h"

class UShieldUEAntiTamperComponent;

UCLASS()
class SHIELDUE_API UShieldUEMultiplayerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Multiplayer")
	void RegisterAntiTamperComponent(UShieldUEAntiTamperComponent* Component);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Multiplayer")
	void UnregisterAntiTamperComponent(UShieldUEAntiTamperComponent* Component);

	UFUNCTION(BlueprintPure, Category = "ShieldUE|Multiplayer")
	bool IsServerAuthority() const;

	UFUNCTION(BlueprintPure, Category = "ShieldUE|Multiplayer")
	bool IsComponentRegistered(const UShieldUEAntiTamperComponent* Component) const;

	UFUNCTION(BlueprintPure, Category = "ShieldUE|Multiplayer")
	float GetSuspicionScore(const UShieldUEAntiTamperComponent* Component) const;

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Multiplayer")
	void ClearSuspicionScore(UShieldUEAntiTamperComponent* Component);

	void RecordViolation(UShieldUEAntiTamperComponent* Component, FShieldUEViolationEvent Event);

private:
	UPROPERTY()
	TArray<TObjectPtr<UShieldUEAntiTamperComponent>> RegisteredComponents;
};
