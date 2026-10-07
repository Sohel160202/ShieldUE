#pragma once

#include "CoreMinimal.h"
#include "SecureValueTypes.generated.h"

UENUM(BlueprintType)
enum class ESecureValueRecoveryMode : uint8
{
	RestoreLastValid UMETA(DisplayName = "Restore Last Valid"),
	ClampToRange UMETA(DisplayName = "Clamp To Range"),
	ResetToDefault UMETA(DisplayName = "Reset To Default")
};

UENUM(BlueprintType)
enum class ESecureValueTamperReason : uint8
{
	None UMETA(DisplayName = "None"),
	NotInitialized UMETA(DisplayName = "Not Initialized"),
	IntegrityMismatch UMETA(DisplayName = "Integrity Mismatch"),
	ShadowMismatch UMETA(DisplayName = "Shadow Mismatch"),
	RangeViolation UMETA(DisplayName = "Range Violation")
};

USTRUCT(BlueprintType, meta = (DisplayName = "Secure Float Rules"))
struct SHIELDUE_API FSecureFloatRules
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules", meta = (ToolTip = "Enable minimum and maximum limits for this secure float value."))
	bool bUseRange = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules", meta = (ToolTip = "Minimum allowed value when range protection is enabled."))
	float MinValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules", meta = (ToolTip = "Maximum allowed value when range protection is enabled."))
	float MaxValue = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules", meta = (ToolTip = "Default fallback value used during initialization or reset."))
	float DefaultValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules", meta = (ToolTip = "How ShieldUE should recover when tampering is detected."))
	ESecureValueRecoveryMode RecoveryMode = ESecureValueRecoveryMode::RestoreLastValid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules", meta = (ToolTip = "Enable periodic runtime rekeying for this value."))
	bool bEnableAutoRekey = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules", meta = (ClampMin = "0.0", ToolTip = "How often the secure value should generate a new runtime key."))
	float RekeyIntervalSeconds = 1.0f;
};

USTRUCT(BlueprintType, meta = (DisplayName = "Secure Int Rules"))
struct SHIELDUE_API FSecureInt32Rules
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules", meta = (ToolTip = "Enable minimum and maximum limits for this secure integer value."))
	bool bUseRange = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules", meta = (ToolTip = "Minimum allowed integer value when range protection is enabled."))
	int32 MinValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules", meta = (ToolTip = "Maximum allowed integer value when range protection is enabled."))
	int32 MaxValue = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules", meta = (ToolTip = "Default fallback integer value used during initialization or reset."))
	int32 DefaultValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules", meta = (ToolTip = "How ShieldUE should recover when tampering is detected."))
	ESecureValueRecoveryMode RecoveryMode = ESecureValueRecoveryMode::RestoreLastValid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules", meta = (ToolTip = "Enable periodic runtime rekeying for this value."))
	bool bEnableAutoRekey = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules", meta = (ClampMin = "0.0", ToolTip = "How often the secure value should generate a new runtime key."))
	float RekeyIntervalSeconds = 1.0f;
};

USTRUCT(BlueprintType, meta = (DisplayName = "Secure Bool Rules"))
struct SHIELDUE_API FSecureBoolRules
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules", meta = (ToolTip = "Default fallback boolean value used during initialization or reset."))
	bool DefaultValue = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules", meta = (ToolTip = "How ShieldUE should recover when tampering is detected."))
	ESecureValueRecoveryMode RecoveryMode = ESecureValueRecoveryMode::RestoreLastValid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules", meta = (ToolTip = "Enable periodic runtime rekeying for this value."))
	bool bEnableAutoRekey = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules", meta = (ClampMin = "0.0", ToolTip = "How often the secure value should generate a new runtime key."))
	float RekeyIntervalSeconds = 1.0f;
};

USTRUCT(BlueprintType, meta = (DisplayName = "Secure Float"))
struct SHIELDUE_API FSecureFloat
{
	GENERATED_BODY()

private:
	UPROPERTY()
	uint32 EncodedValueBits = 0;

	UPROPERTY()
	uint32 ShadowBits = 0;

	UPROPERTY()
	uint32 Key = 0;

	UPROPERTY()
	uint32 Integrity = 0;

	UPROPERTY()
	float LastValidValue = 0.0f;

	UPROPERTY()
	uint32 TamperCount = 0;

	UPROPERTY()
	uint8 EncodingPattern = 0;

	UPROPERTY()
	bool bInitialized = false;

	UPROPERTY(Transient)
	float TimeSinceLastRekey = 0.0f;

	UPROPERTY()
	ESecureValueTamperReason LastTamperReason = ESecureValueTamperReason::None;

	UPROPERTY(EditAnywhere, Category = "ShieldUE")
	FSecureFloatRules Rules;

public:
	FSecureFloat();

	void Initialize(float InitialValue, const FSecureFloatRules& InRules);
	void Set(float NewValue);
	float Get();
	bool Validate();
	void Rekey();
	void UpdateProtection(float DeltaSeconds);
	void ResetToDefault();
	void CorruptForTesting();

	bool IsInitialized() const { return bInitialized; }
	uint32 GetTamperCount() const { return TamperCount; }
	float GetLastValidValue() const { return LastValidValue; }
	ESecureValueTamperReason GetLastTamperReason() const { return LastTamperReason; }
	bool ValidateRules(FString& OutError) const;

	void SetRules(const FSecureFloatRules& InRules);
	const FSecureFloatRules& GetRules() const { return Rules; }

private:
	void EncodeValue(float InValue);
	float DecodeMainValueUnchecked() const;
	float DecodeShadowValueUnchecked() const;

	bool IsWithinAllowedRange(float InValue) const;
	float RecoverValue();
	void RegisterTamper(ESecureValueTamperReason Reason);

	uint32 ComputeIntegrity(uint32 InEncoded, uint32 InShadow, uint32 InKey) const;
	uint32 GenerateRuntimeKey() const;
};

USTRUCT(BlueprintType, meta = (DisplayName = "Secure Int"))
struct SHIELDUE_API FSecureInt32
{
	GENERATED_BODY()

private:
	UPROPERTY()
	uint32 EncodedValue = 0;

	UPROPERTY()
	uint32 ShadowValue = 0;

	UPROPERTY()
	uint32 Key = 0;

	UPROPERTY()
	uint32 Integrity = 0;

	UPROPERTY()
	int32 LastValidValue = 0;

	UPROPERTY()
	uint32 TamperCount = 0;

	UPROPERTY()
	uint8 EncodingPattern = 0;

	UPROPERTY()
	bool bInitialized = false;

	UPROPERTY(Transient)
	float TimeSinceLastRekey = 0.0f;

	UPROPERTY()
	ESecureValueTamperReason LastTamperReason = ESecureValueTamperReason::None;

	UPROPERTY(EditAnywhere, Category = "ShieldUE")
	FSecureInt32Rules Rules;

public:
	FSecureInt32();

	void Initialize(int32 InitialValue, const FSecureInt32Rules& InRules);
	void Set(int32 NewValue);
	int32 Get();
	bool Validate();
	void Rekey();
	void UpdateProtection(float DeltaSeconds);
	void ResetToDefault();
	void CorruptForTesting();

	bool IsInitialized() const { return bInitialized; }
	uint32 GetTamperCount() const { return TamperCount; }
	int32 GetLastValidValue() const { return LastValidValue; }
	ESecureValueTamperReason GetLastTamperReason() const { return LastTamperReason; }
	bool ValidateRules(FString& OutError) const;

	void SetRules(const FSecureInt32Rules& InRules);
	const FSecureInt32Rules& GetRules() const { return Rules; }

private:
	void EncodeValue(int32 InValue);
	int32 DecodeMainValueUnchecked() const;
	int32 DecodeShadowValueUnchecked() const;

	bool IsWithinAllowedRange(int32 InValue) const;
	int32 RecoverValue();
	void RegisterTamper(ESecureValueTamperReason Reason);

	uint32 ComputeIntegrity(uint32 InEncoded, uint32 InShadow, uint32 InKey) const;
	uint32 GenerateRuntimeKey() const;
};

USTRUCT(BlueprintType, meta = (DisplayName = "Secure Bool"))
struct SHIELDUE_API FSecureBool
{
	GENERATED_BODY()

private:
	UPROPERTY()
	uint8 EncodedValue = 0;

	UPROPERTY()
	uint8 ShadowValue = 0;

	UPROPERTY()
	uint8 Key = 0;

	UPROPERTY()
	uint32 Integrity = 0;

	UPROPERTY()
	bool LastValidValue = false;

	UPROPERTY()
	uint32 TamperCount = 0;

	UPROPERTY()
	bool bInitialized = false;

	UPROPERTY(Transient)
	float TimeSinceLastRekey = 0.0f;

	UPROPERTY()
	ESecureValueTamperReason LastTamperReason = ESecureValueTamperReason::None;

	UPROPERTY(EditAnywhere, Category = "ShieldUE")
	FSecureBoolRules Rules;

public:
	FSecureBool();

	void Initialize(bool InitialValue, const FSecureBoolRules& InRules);
	void Set(bool NewValue);
	bool Get();
	bool Validate();
	void Rekey();
	void UpdateProtection(float DeltaSeconds);
	void ResetToDefault();
	void CorruptForTesting();

	bool IsInitialized() const { return bInitialized; }
	uint32 GetTamperCount() const { return TamperCount; }
	bool GetLastValidValue() const { return LastValidValue; }
	ESecureValueTamperReason GetLastTamperReason() const { return LastTamperReason; }
	bool ValidateRules(FString& OutError) const;

	void SetRules(const FSecureBoolRules& InRules);
	const FSecureBoolRules& GetRules() const { return Rules; }

private:
	void EncodeValue(bool InValue);
	bool DecodeMainValueUnchecked() const;
	bool DecodeShadowValueUnchecked() const;

	bool RecoverValue();
	void RegisterTamper(ESecureValueTamperReason Reason);

	uint32 ComputeIntegrity(uint8 InEncoded, uint8 InShadow, uint8 InKey) const;
	uint8 GenerateRuntimeKey() const;
};

USTRUCT(BlueprintType, meta = (DisplayName = "Secure Int64 Rules"))
struct SHIELDUE_API FSecureInt64Rules
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules")
	bool bUseRange = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules")
	int64 MinValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules")
	int64 MaxValue = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules")
	int64 DefaultValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules")
	ESecureValueRecoveryMode RecoveryMode = ESecureValueRecoveryMode::RestoreLastValid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules")
	bool bEnableAutoRekey = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules", meta = (ClampMin = "0.0"))
	float RekeyIntervalSeconds = 1.0f;
};

USTRUCT(BlueprintType, meta = (DisplayName = "Secure Double Rules"))
struct SHIELDUE_API FSecureDoubleRules
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules")
	bool bUseRange = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules")
	double MinValue = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules")
	double MaxValue = 100.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules")
	double DefaultValue = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules")
	ESecureValueRecoveryMode RecoveryMode = ESecureValueRecoveryMode::RestoreLastValid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules")
	bool bEnableAutoRekey = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules", meta = (ClampMin = "0.0"))
	float RekeyIntervalSeconds = 1.0f;
};

USTRUCT(BlueprintType, meta = (DisplayName = "Secure Byte Rules"))
struct SHIELDUE_API FSecureByteRules
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules")
	bool bUseRange = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules")
	uint8 MinValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules")
	uint8 MaxValue = 255;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules")
	uint8 DefaultValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules")
	ESecureValueRecoveryMode RecoveryMode = ESecureValueRecoveryMode::RestoreLastValid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules")
	bool bEnableAutoRekey = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldUE|Rules", meta = (ClampMin = "0.0"))
	float RekeyIntervalSeconds = 1.0f;
};

USTRUCT(BlueprintType, meta = (DisplayName = "Secure Int64"))
struct SHIELDUE_API FSecureInt64
{
	GENERATED_BODY()

private:
	UPROPERTY() uint64 EncodedValue = 0;
	UPROPERTY() uint64 ShadowValue = 0;
	UPROPERTY() uint64 Key = 0;
	UPROPERTY() uint32 Integrity = 0;
	UPROPERTY() int64 LastValidValue = 0;
	UPROPERTY() uint32 TamperCount = 0;
	UPROPERTY() uint8 EncodingPattern = 0;
	UPROPERTY() bool bInitialized = false;
	UPROPERTY(Transient) float TimeSinceLastRekey = 0.0f;
	UPROPERTY() ESecureValueTamperReason LastTamperReason = ESecureValueTamperReason::None;
	UPROPERTY(EditAnywhere, Category = "ShieldUE") FSecureInt64Rules Rules;

public:
	FSecureInt64();
	void Initialize(int64 InitialValue, const FSecureInt64Rules& InRules);
	void Set(int64 NewValue);
	int64 Get();
	bool Validate();
	void Rekey();
	void UpdateProtection(float DeltaSeconds);
	void ResetToDefault();
	void CorruptForTesting();
	bool IsInitialized() const { return bInitialized; }
	uint32 GetTamperCount() const { return TamperCount; }
	int64 GetLastValidValue() const { return LastValidValue; }
	ESecureValueTamperReason GetLastTamperReason() const { return LastTamperReason; }
	bool ValidateRules(FString& OutError) const;
	void SetRules(const FSecureInt64Rules& InRules);
	const FSecureInt64Rules& GetRules() const { return Rules; }

private:
	void EncodeValue(int64 InValue);
	int64 DecodeMainValueUnchecked() const;
	int64 DecodeShadowValueUnchecked() const;
	bool IsWithinAllowedRange(int64 InValue) const;
	int64 RecoverValue();
	void RegisterTamper(ESecureValueTamperReason Reason);
	uint32 ComputeIntegrity(uint64 InEncoded, uint64 InShadow, uint64 InKey) const;
	uint64 GenerateRuntimeKey() const;
};

USTRUCT(BlueprintType, meta = (DisplayName = "Secure Double"))
struct SHIELDUE_API FSecureDouble
{
	GENERATED_BODY()

private:
	UPROPERTY() uint64 EncodedValueBits = 0;
	UPROPERTY() uint64 ShadowBits = 0;
	UPROPERTY() uint64 Key = 0;
	UPROPERTY() uint32 Integrity = 0;
	UPROPERTY() double LastValidValue = 0.0;
	UPROPERTY() uint32 TamperCount = 0;
	UPROPERTY() uint8 EncodingPattern = 0;
	UPROPERTY() bool bInitialized = false;
	UPROPERTY(Transient) float TimeSinceLastRekey = 0.0f;
	UPROPERTY() ESecureValueTamperReason LastTamperReason = ESecureValueTamperReason::None;
	UPROPERTY(EditAnywhere, Category = "ShieldUE") FSecureDoubleRules Rules;

public:
	FSecureDouble();
	void Initialize(double InitialValue, const FSecureDoubleRules& InRules);
	void Set(double NewValue);
	double Get();
	bool Validate();
	void Rekey();
	void UpdateProtection(float DeltaSeconds);
	void ResetToDefault();
	void CorruptForTesting();
	bool IsInitialized() const { return bInitialized; }
	uint32 GetTamperCount() const { return TamperCount; }
	double GetLastValidValue() const { return LastValidValue; }
	ESecureValueTamperReason GetLastTamperReason() const { return LastTamperReason; }
	bool ValidateRules(FString& OutError) const;
	void SetRules(const FSecureDoubleRules& InRules);
	const FSecureDoubleRules& GetRules() const { return Rules; }

private:
	void EncodeValue(double InValue);
	double DecodeMainValueUnchecked() const;
	double DecodeShadowValueUnchecked() const;
	bool IsWithinAllowedRange(double InValue) const;
	double RecoverValue();
	void RegisterTamper(ESecureValueTamperReason Reason);
	uint32 ComputeIntegrity(uint64 InEncoded, uint64 InShadow, uint64 InKey) const;
	uint64 GenerateRuntimeKey() const;
};

USTRUCT(BlueprintType, meta = (DisplayName = "Secure Byte"))
struct SHIELDUE_API FSecureByte
{
	GENERATED_BODY()

private:
	UPROPERTY() uint8 EncodedValue = 0;
	UPROPERTY() uint8 ShadowValue = 0;
	UPROPERTY() uint8 Key = 0;
	UPROPERTY() uint32 Integrity = 0;
	UPROPERTY() uint8 LastValidValue = 0;
	UPROPERTY() uint32 TamperCount = 0;
	UPROPERTY() bool bInitialized = false;
	UPROPERTY(Transient) float TimeSinceLastRekey = 0.0f;
	UPROPERTY() ESecureValueTamperReason LastTamperReason = ESecureValueTamperReason::None;
	UPROPERTY(EditAnywhere, Category = "ShieldUE") FSecureByteRules Rules;

public:
	FSecureByte();
	void Initialize(uint8 InitialValue, const FSecureByteRules& InRules);
	void Set(uint8 NewValue);
	uint8 Get();
	bool Validate();
	void Rekey();
	void UpdateProtection(float DeltaSeconds);
	void ResetToDefault();
	void CorruptForTesting();
	bool IsInitialized() const { return bInitialized; }
	uint32 GetTamperCount() const { return TamperCount; }
	uint8 GetLastValidValue() const { return LastValidValue; }
	ESecureValueTamperReason GetLastTamperReason() const { return LastTamperReason; }
	bool ValidateRules(FString& OutError) const;
	void SetRules(const FSecureByteRules& InRules);
	const FSecureByteRules& GetRules() const { return Rules; }

private:
	void EncodeValue(uint8 InValue);
	uint8 DecodeMainValueUnchecked() const;
	uint8 DecodeShadowValueUnchecked() const;
	bool IsWithinAllowedRange(uint8 InValue) const;
	uint8 RecoverValue();
	void RegisterTamper(ESecureValueTamperReason Reason);
	uint32 ComputeIntegrity(uint8 InEncoded, uint8 InShadow, uint8 InKey) const;
	uint8 GenerateRuntimeKey() const;
};
