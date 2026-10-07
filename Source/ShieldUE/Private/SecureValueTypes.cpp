#include "SecureValueTypes.h"
#include "ShieldUE.h"
#include "HAL/PlatformTime.h"
#include "Misc/DateTime.h"

namespace ShieldUEPrivate
{
	static uint32 FloatToBits(float Value)
	{
		uint32 Bits = 0;
		FMemory::Memcpy(&Bits, &Value, sizeof(float));
		return Bits;
	}

	static float BitsToFloat(uint32 Bits)
	{
		float Value = 0.0f;
		FMemory::Memcpy(&Value, &Bits, sizeof(uint32));
		return Value;
	}

	static uint64 DoubleToBits(double Value)
	{
		uint64 Bits = 0;
		FMemory::Memcpy(&Bits, &Value, sizeof(double));
		return Bits;
	}

	static double BitsToDouble(uint64 Bits)
	{
		double Value = 0.0;
		FMemory::Memcpy(&Value, &Bits, sizeof(uint64));
		return Value;
	}

	static uint32 RotateLeft32(uint32 Value, int32 Shift)
	{
		return (Value << Shift) | (Value >> (32 - Shift));
	}

	static uint32 RotateRight32(uint32 Value, int32 Shift)
	{
		return (Value >> Shift) | (Value << (32 - Shift));
	}

	static uint64 RotateLeft64(uint64 Value, int32 Shift)
	{
		if (Shift == 0)
		{
			return Value;
		}
		return (Value << Shift) | (Value >> (64 - Shift));
	}

	static uint64 RotateRight64(uint64 Value, int32 Shift)
	{
		if (Shift == 0)
		{
			return Value;
		}
		return (Value >> Shift) | (Value << (64 - Shift));
	}
}

/* =========================
   FSecureFloat V2 Core (stabilized)
   ========================= */

FSecureFloat::FSecureFloat()
{
	EncodedValueBits = 0;
	ShadowBits = 0;
	Key = 0;
	Integrity = 0;
	LastValidValue = 0.0f;
	TamperCount = 0;
	EncodingPattern = 0;
	bInitialized = false;
	TimeSinceLastRekey = 0.0f;
	LastTamperReason = ESecureValueTamperReason::None;
}

bool FSecureFloat::ValidateRules(FString& OutError) const
{
	OutError.Reset();

	if (!FMath::IsFinite(Rules.MinValue) || !FMath::IsFinite(Rules.MaxValue) || !FMath::IsFinite(Rules.DefaultValue))
	{
		OutError = TEXT("Float rules cannot contain NaN or infinite values.");
		return false;
	}

	if (Rules.bUseRange && Rules.MinValue > Rules.MaxValue)
	{
		OutError = TEXT("MinValue cannot be greater than MaxValue.");
		return false;
	}

	if (Rules.bUseRange && (Rules.DefaultValue < Rules.MinValue || Rules.DefaultValue > Rules.MaxValue))
	{
		OutError = TEXT("DefaultValue must be inside the configured range.");
		return false;
	}

	if (Rules.bEnableAutoRekey && Rules.RekeyIntervalSeconds <= 0.0f)
	{
		OutError = TEXT("RekeyIntervalSeconds must be greater than zero when automatic re-keying is enabled.");
		return false;
	}

	if (static_cast<uint8>(Rules.RecoveryMode) > static_cast<uint8>(ESecureValueRecoveryMode::ResetToDefault))
	{
		OutError = TEXT("RecoveryMode is invalid.");
		return false;
	}

	return true;
}

void FSecureFloat::SetRules(const FSecureFloatRules& InRules)
{
	Rules = InRules;
	FString RuleError;
	if (!ValidateRules(RuleError))
	{
		UE_LOG(LogShieldUE, Warning, TEXT("Invalid Secure Float rules: %s"), *RuleError);
	}
}

void FSecureFloat::Initialize(float InitialValue, const FSecureFloatRules& InRules)
{
	Rules = InRules;
	FString RuleError;
	if (!ValidateRules(RuleError))
	{
		UE_LOG(LogShieldUE, Warning, TEXT("Invalid Secure Float rules: %s"), *RuleError);
	}
	Key = GenerateRuntimeKey();
	EncodingPattern = static_cast<uint8>(FMath::RandRange(0, 2));
	bInitialized = true;
	TimeSinceLastRekey = 0.0f;
	LastTamperReason = ESecureValueTamperReason::None;

	Set(InitialValue);
}

void FSecureFloat::Set(float NewValue)
{
	if (!bInitialized)
	{
		Key = GenerateRuntimeKey();
		EncodingPattern = static_cast<uint8>(FMath::RandRange(0, 2));
		bInitialized = true;
		TimeSinceLastRekey = 0.0f;
		LastTamperReason = ESecureValueTamperReason::None;
	}

	if (Rules.bUseRange)
	{
		NewValue = FMath::Clamp(NewValue, Rules.MinValue, Rules.MaxValue);
	}

	EncodeValue(NewValue);
	LastValidValue = NewValue;
	LastTamperReason = ESecureValueTamperReason::None;
}

float FSecureFloat::Get()
{
	if (!bInitialized)
	{
		RegisterTamper(ESecureValueTamperReason::NotInitialized);
		return Rules.DefaultValue;
	}

	const uint32 CurrentIntegrity = ComputeIntegrity(EncodedValueBits, ShadowBits, Key);
	if (CurrentIntegrity != Integrity)
	{
		RegisterTamper(ESecureValueTamperReason::IntegrityMismatch);
		return RecoverValue();
	}

	const float MainValue = DecodeMainValueUnchecked();
	const float ShadowValue = DecodeShadowValueUnchecked();

	const uint32 MainBits = ShieldUEPrivate::FloatToBits(MainValue);
	const uint32 ShadowBitsDecoded = ShieldUEPrivate::FloatToBits(ShadowValue);

	if (MainBits != ShadowBitsDecoded)
	{
		RegisterTamper(ESecureValueTamperReason::ShadowMismatch);
		return RecoverValue();
	}

	if (!IsWithinAllowedRange(MainValue))
	{
		RegisterTamper(ESecureValueTamperReason::RangeViolation);
		return RecoverValue();
	}

	return MainValue;
}

bool FSecureFloat::Validate()
{
	if (!bInitialized)
	{
		RegisterTamper(ESecureValueTamperReason::NotInitialized);
		return false;
	}

	const uint32 CurrentIntegrity = ComputeIntegrity(EncodedValueBits, ShadowBits, Key);
	if (CurrentIntegrity != Integrity)
	{
		RegisterTamper(ESecureValueTamperReason::IntegrityMismatch);
		return false;
	}

	const float MainValue = DecodeMainValueUnchecked();
	const float ShadowValue = DecodeShadowValueUnchecked();

	const uint32 MainBits = ShieldUEPrivate::FloatToBits(MainValue);
	const uint32 ShadowBitsDecoded = ShieldUEPrivate::FloatToBits(ShadowValue);

	if (MainBits != ShadowBitsDecoded)
	{
		RegisterTamper(ESecureValueTamperReason::ShadowMismatch);
		return false;
	}

	if (!IsWithinAllowedRange(MainValue))
	{
		RegisterTamper(ESecureValueTamperReason::RangeViolation);
		return false;
	}

	return true;
}

void FSecureFloat::Rekey()
{
	if (!bInitialized)
	{
		return;
	}

	const float CurrentValue = Get();
	Key = GenerateRuntimeKey();
	EncodeValue(CurrentValue);
	TimeSinceLastRekey = 0.0f;
}

void FSecureFloat::UpdateProtection(float DeltaSeconds)
{
	if (!bInitialized || !Rules.bEnableAutoRekey || Rules.RekeyIntervalSeconds <= 0.0f)
	{
		return;
	}

	TimeSinceLastRekey += FMath::Max(0.0f, DeltaSeconds);
	if (TimeSinceLastRekey >= Rules.RekeyIntervalSeconds)
	{
		Rekey();
	}
}

void FSecureFloat::ResetToDefault()
{
	if (!bInitialized)
	{
		Key = GenerateRuntimeKey();
		EncodingPattern = static_cast<uint8>(FMath::RandRange(0, 2));
		bInitialized = true;
	}

	EncodeValue(Rules.DefaultValue);
	LastValidValue = Rules.DefaultValue;
	TimeSinceLastRekey = 0.0f;
	LastTamperReason = ESecureValueTamperReason::None;
}

void FSecureFloat::CorruptForTesting()
{
	EncodedValueBits ^= 0x00FF00FFu;
}

void FSecureFloat::EncodeValue(float InValue)
{
	const uint32 RawBits = ShieldUEPrivate::FloatToBits(InValue);

	switch (EncodingPattern)
	{
	case 0:
		EncodedValueBits = RawBits ^ Key;
		break;

	case 1:
		EncodedValueBits = ShieldUEPrivate::RotateLeft32(RawBits ^ Key, Key % 16);
		break;

	default:
		EncodedValueBits = (RawBits + Key) ^ ShieldUEPrivate::RotateRight32(Key, 5);
		break;
	}

	ShadowBits = RawBits ^ 0xA5A5A5A5u;

	Integrity = ComputeIntegrity(EncodedValueBits, ShadowBits, Key);
}

float FSecureFloat::DecodeMainValueUnchecked() const
{
	uint32 RawBits = 0;

	switch (EncodingPattern)
	{
	case 0:
		RawBits = EncodedValueBits ^ Key;
		break;

	case 1:
		RawBits = ShieldUEPrivate::RotateRight32(EncodedValueBits, Key % 16) ^ Key;
		break;

	default:
		RawBits = (EncodedValueBits ^ ShieldUEPrivate::RotateRight32(Key, 5)) - Key;
		break;
	}

	return ShieldUEPrivate::BitsToFloat(RawBits);
}

float FSecureFloat::DecodeShadowValueUnchecked() const
{
	const uint32 RawBits = ShadowBits ^ 0xA5A5A5A5u;
	return ShieldUEPrivate::BitsToFloat(RawBits);
}

bool FSecureFloat::IsWithinAllowedRange(float InValue) const
{
	if (!Rules.bUseRange)
	{
		return true;
	}

	return InValue >= Rules.MinValue && InValue <= Rules.MaxValue;
}

float FSecureFloat::RecoverValue()
{
	switch (Rules.RecoveryMode)
	{
	case ESecureValueRecoveryMode::RestoreLastValid:
		EncodeValue(LastValidValue);
		return LastValidValue;

	case ESecureValueRecoveryMode::ClampToRange:
	{
		float Recovered = LastValidValue;

		if (Rules.bUseRange)
		{
			Recovered = FMath::Clamp(Recovered, Rules.MinValue, Rules.MaxValue);
		}

		EncodeValue(Recovered);
		LastValidValue = Recovered;
		return Recovered;
	}

	case ESecureValueRecoveryMode::ResetToDefault:
	default:
		EncodeValue(Rules.DefaultValue);
		LastValidValue = Rules.DefaultValue;
		return Rules.DefaultValue;
	}
}

void FSecureFloat::RegisterTamper(ESecureValueTamperReason Reason)
{
	++TamperCount;
	LastTamperReason = Reason;
}

uint32 FSecureFloat::ComputeIntegrity(uint32 InEncoded, uint32 InShadow, uint32 InKey) const
{
	uint32 Hash = 2166136261u;
	Hash = (Hash ^ InEncoded) * 16777619u;
	Hash = (Hash ^ InShadow) * 16777619u;
	Hash = (Hash ^ InKey) * 16777619u;
	return Hash;
}

uint32 FSecureFloat::GenerateRuntimeKey() const
{
	const uint64 Cycles = FPlatformTime::Cycles64();
	const uint32 TimePart = static_cast<uint32>(FDateTime::UtcNow().GetTicks());
	return static_cast<uint32>(Cycles) ^ TimePart ^ 0x9E3779B9u;
}

/* =========================
   FSecureInt32 V2 Core (stabilized)
   ========================= */

FSecureInt32::FSecureInt32()
{
	EncodedValue = 0;
	ShadowValue = 0;
	Key = 0;
	Integrity = 0;
	LastValidValue = 0;
	TamperCount = 0;
	EncodingPattern = 0;
	bInitialized = false;
	TimeSinceLastRekey = 0.0f;
	LastTamperReason = ESecureValueTamperReason::None;
}

bool FSecureInt32::ValidateRules(FString& OutError) const
{
	OutError.Reset();

	if (Rules.bUseRange && Rules.MinValue > Rules.MaxValue)
	{
		OutError = TEXT("MinValue cannot be greater than MaxValue.");
		return false;
	}

	if (Rules.bUseRange && (Rules.DefaultValue < Rules.MinValue || Rules.DefaultValue > Rules.MaxValue))
	{
		OutError = TEXT("DefaultValue must be inside the configured range.");
		return false;
	}

	if (Rules.bEnableAutoRekey && Rules.RekeyIntervalSeconds <= 0.0f)
	{
		OutError = TEXT("RekeyIntervalSeconds must be greater than zero when automatic re-keying is enabled.");
		return false;
	}

	if (static_cast<uint8>(Rules.RecoveryMode) > static_cast<uint8>(ESecureValueRecoveryMode::ResetToDefault))
	{
		OutError = TEXT("RecoveryMode is invalid.");
		return false;
	}

	return true;
}

void FSecureInt32::SetRules(const FSecureInt32Rules& InRules)
{
	Rules = InRules;
	FString RuleError;
	if (!ValidateRules(RuleError))
	{
		UE_LOG(LogShieldUE, Warning, TEXT("Invalid Secure Int rules: %s"), *RuleError);
	}
}

void FSecureInt32::Initialize(int32 InitialValue, const FSecureInt32Rules& InRules)
{
	Rules = InRules;
	FString RuleError;
	if (!ValidateRules(RuleError))
	{
		UE_LOG(LogShieldUE, Warning, TEXT("Invalid Secure Int rules: %s"), *RuleError);
	}
	Key = GenerateRuntimeKey();
	EncodingPattern = static_cast<uint8>(FMath::RandRange(0, 3));
	bInitialized = true;
	TimeSinceLastRekey = 0.0f;
	LastTamperReason = ESecureValueTamperReason::None;

	Set(InitialValue);
}

void FSecureInt32::Set(int32 NewValue)
{
	if (!bInitialized)
	{
		Key = GenerateRuntimeKey();
		EncodingPattern = static_cast<uint8>(FMath::RandRange(0, 3));
		bInitialized = true;
		TimeSinceLastRekey = 0.0f;
		LastTamperReason = ESecureValueTamperReason::None;
	}

	if (Rules.bUseRange)
	{
		NewValue = FMath::Clamp(
			NewValue,
			static_cast<int32>(Rules.MinValue),
			static_cast<int32>(Rules.MaxValue)
		);
	}

	EncodeValue(NewValue);
	LastValidValue = NewValue;
	LastTamperReason = ESecureValueTamperReason::None;
}

int32 FSecureInt32::Get()
{
	if (!bInitialized)
	{
		RegisterTamper(ESecureValueTamperReason::NotInitialized);
		return static_cast<int32>(Rules.DefaultValue);
	}

	const uint32 CurrentIntegrity = ComputeIntegrity(EncodedValue, ShadowValue, Key);
	if (CurrentIntegrity != Integrity)
	{
		RegisterTamper(ESecureValueTamperReason::IntegrityMismatch);
		return RecoverValue();
	}

	const int32 MainValue = DecodeMainValueUnchecked();
	const int32 ShadowDecodedValue = DecodeShadowValueUnchecked();

	if (MainValue != ShadowDecodedValue)
	{
		RegisterTamper(ESecureValueTamperReason::ShadowMismatch);
		return RecoverValue();
	}

	if (!IsWithinAllowedRange(MainValue))
	{
		RegisterTamper(ESecureValueTamperReason::RangeViolation);
		return RecoverValue();
	}

	return MainValue;
}

bool FSecureInt32::Validate()
{
	if (!bInitialized)
	{
		RegisterTamper(ESecureValueTamperReason::NotInitialized);
		return false;
	}

	const uint32 CurrentIntegrity = ComputeIntegrity(EncodedValue, ShadowValue, Key);
	if (CurrentIntegrity != Integrity)
	{
		RegisterTamper(ESecureValueTamperReason::IntegrityMismatch);
		return false;
	}

	const int32 MainValue = DecodeMainValueUnchecked();
	const int32 ShadowDecodedValue = DecodeShadowValueUnchecked();

	if (MainValue != ShadowDecodedValue)
	{
		RegisterTamper(ESecureValueTamperReason::ShadowMismatch);
		return false;
	}

	if (!IsWithinAllowedRange(MainValue))
	{
		RegisterTamper(ESecureValueTamperReason::RangeViolation);
		return false;
	}

	return true;
}

void FSecureInt32::Rekey()
{
	if (!bInitialized)
	{
		return;
	}

	const int32 CurrentValue = Get();
	Key = GenerateRuntimeKey();
	EncodeValue(CurrentValue);
	TimeSinceLastRekey = 0.0f;
}

void FSecureInt32::UpdateProtection(float DeltaSeconds)
{
	if (!bInitialized || !Rules.bEnableAutoRekey || Rules.RekeyIntervalSeconds <= 0.0f)
	{
		return;
	}

	TimeSinceLastRekey += FMath::Max(0.0f, DeltaSeconds);
	if (TimeSinceLastRekey >= Rules.RekeyIntervalSeconds)
	{
		Rekey();
	}
}

void FSecureInt32::ResetToDefault()
{
	if (!bInitialized)
	{
		Key = GenerateRuntimeKey();
		EncodingPattern = static_cast<uint8>(FMath::RandRange(0, 3));
		bInitialized = true;
	}

	const int32 DefaultInt = static_cast<int32>(Rules.DefaultValue);
	EncodeValue(DefaultInt);
	LastValidValue = DefaultInt;
	TimeSinceLastRekey = 0.0f;
	LastTamperReason = ESecureValueTamperReason::None;
}

void FSecureInt32::CorruptForTesting()
{
	EncodedValue ^= 0x00FF00FFu;
}

void FSecureInt32::EncodeValue(int32 InValue)
{
	const uint32 Raw = static_cast<uint32>(InValue);

	switch (EncodingPattern)
	{
	case 0:
		EncodedValue = Raw ^ Key;
		break;

	case 1:
		EncodedValue = ShieldUEPrivate::RotateLeft32(Raw ^ Key, Key % 16);
		break;

	case 2:
		EncodedValue = (Raw + Key) ^ ShieldUEPrivate::RotateRight32(Key, 5);
		break;

	default:
		EncodedValue = ShieldUEPrivate::RotateLeft32(Raw + Key, 7);
		break;
	}

	// Shadow is intentionally independent of Key to keep verification stable
	ShadowValue = Raw ^ 0xA5A5A5A5u;

	Integrity = ComputeIntegrity(EncodedValue, ShadowValue, Key);
}

int32 FSecureInt32::DecodeMainValueUnchecked() const
{
	uint32 Raw = 0;

	switch (EncodingPattern)
	{
	case 0:
		Raw = EncodedValue ^ Key;
		break;

	case 1:
		Raw = ShieldUEPrivate::RotateRight32(EncodedValue, Key % 16) ^ Key;
		break;

	case 2:
		Raw = (EncodedValue ^ ShieldUEPrivate::RotateRight32(Key, 5)) - Key;
		break;

	default:
		Raw = ShieldUEPrivate::RotateRight32(EncodedValue, 7) - Key;
		break;
	}

	return static_cast<int32>(Raw);
}

int32 FSecureInt32::DecodeShadowValueUnchecked() const
{
	const uint32 Raw = ShadowValue ^ 0xA5A5A5A5u;
	return static_cast<int32>(Raw);
}

bool FSecureInt32::IsWithinAllowedRange(int32 InValue) const
{
	if (!Rules.bUseRange)
	{
		return true;
	}

	return InValue >= static_cast<int32>(Rules.MinValue) &&
		InValue <= static_cast<int32>(Rules.MaxValue);
}

int32 FSecureInt32::RecoverValue()
{
	switch (Rules.RecoveryMode)
	{
	case ESecureValueRecoveryMode::RestoreLastValid:
		EncodeValue(LastValidValue);
		return LastValidValue;

	case ESecureValueRecoveryMode::ClampToRange:
	{
		int32 Recovered = LastValidValue;

		if (Rules.bUseRange)
		{
			Recovered = FMath::Clamp(
				Recovered,
				static_cast<int32>(Rules.MinValue),
				static_cast<int32>(Rules.MaxValue)
			);
		}

		EncodeValue(Recovered);
		LastValidValue = Recovered;
		return Recovered;
	}

	case ESecureValueRecoveryMode::ResetToDefault:
	default:
	{
		const int32 DefaultInt = static_cast<int32>(Rules.DefaultValue);
		EncodeValue(DefaultInt);
		LastValidValue = DefaultInt;
		return DefaultInt;
	}
	}
}

void FSecureInt32::RegisterTamper(ESecureValueTamperReason Reason)
{
	++TamperCount;
	LastTamperReason = Reason;
}

uint32 FSecureInt32::ComputeIntegrity(uint32 InEncoded, uint32 InShadow, uint32 InKey) const
{
	uint32 Hash = 2166136261u;
	Hash = (Hash ^ InEncoded) * 16777619u;
	Hash = (Hash ^ InShadow) * 16777619u;
	Hash = (Hash ^ InKey) * 16777619u;
	return Hash;
}

uint32 FSecureInt32::GenerateRuntimeKey() const
{
	const uint64 Cycles = FPlatformTime::Cycles64();
	const uint32 TimePart = static_cast<uint32>(FDateTime::UtcNow().GetTicks());
	return static_cast<uint32>(Cycles) ^ TimePart ^ 0x7F4A7C15u;
}

/* =========================
   FSecureBool V2 Core (stabilized)
   ========================= */

FSecureBool::FSecureBool()
{
	EncodedValue = 0;
	ShadowValue = 0;
	Key = 0;
	Integrity = 0;
	LastValidValue = false;
	TamperCount = 0;
	bInitialized = false;
	TimeSinceLastRekey = 0.0f;
	LastTamperReason = ESecureValueTamperReason::None;
}

bool FSecureBool::ValidateRules(FString& OutError) const
{
	OutError.Reset();

	if (Rules.bEnableAutoRekey && Rules.RekeyIntervalSeconds <= 0.0f)
	{
		OutError = TEXT("RekeyIntervalSeconds must be greater than zero when automatic re-keying is enabled.");
		return false;
	}

	if (static_cast<uint8>(Rules.RecoveryMode) > static_cast<uint8>(ESecureValueRecoveryMode::ResetToDefault))
	{
		OutError = TEXT("RecoveryMode is invalid.");
		return false;
	}

	return true;
}

void FSecureBool::SetRules(const FSecureBoolRules& InRules)
{
	Rules = InRules;
	FString RuleError;
	if (!ValidateRules(RuleError))
	{
		UE_LOG(LogShieldUE, Warning, TEXT("Invalid Secure Bool rules: %s"), *RuleError);
	}
}

void FSecureBool::Initialize(bool InitialValue, const FSecureBoolRules& InRules)
{
	Rules = InRules;
	FString RuleError;
	if (!ValidateRules(RuleError))
	{
		UE_LOG(LogShieldUE, Warning, TEXT("Invalid Secure Bool rules: %s"), *RuleError);
	}
	Key = GenerateRuntimeKey();
	bInitialized = true;
	TimeSinceLastRekey = 0.0f;
	LastTamperReason = ESecureValueTamperReason::None;

	Set(InitialValue);
}

void FSecureBool::Set(bool NewValue)
{
	if (!bInitialized)
	{
		Key = GenerateRuntimeKey();
		bInitialized = true;
		TimeSinceLastRekey = 0.0f;
		LastTamperReason = ESecureValueTamperReason::None;
	}

	EncodeValue(NewValue);
	LastValidValue = NewValue;
	LastTamperReason = ESecureValueTamperReason::None;
}

bool FSecureBool::Get()
{
	if (!bInitialized)
	{
		RegisterTamper(ESecureValueTamperReason::NotInitialized);
		return false;
	}

	const uint32 CurrentIntegrity = ComputeIntegrity(EncodedValue, ShadowValue, Key);
	if (CurrentIntegrity != Integrity)
	{
		RegisterTamper(ESecureValueTamperReason::IntegrityMismatch);
		return RecoverValue();
	}

	const bool MainValue = DecodeMainValueUnchecked();
	const bool ShadowValueDecoded = DecodeShadowValueUnchecked();

	if (MainValue != ShadowValueDecoded)
	{
		RegisterTamper(ESecureValueTamperReason::ShadowMismatch);
		return RecoverValue();
	}

	return MainValue;
}

bool FSecureBool::Validate()
{
	if (!bInitialized)
	{
		RegisterTamper(ESecureValueTamperReason::NotInitialized);
		return false;
	}

	const uint32 CurrentIntegrity = ComputeIntegrity(EncodedValue, ShadowValue, Key);
	if (CurrentIntegrity != Integrity)
	{
		RegisterTamper(ESecureValueTamperReason::IntegrityMismatch);
		return false;
	}

	const bool MainValue = DecodeMainValueUnchecked();
	const bool ShadowValueDecoded = DecodeShadowValueUnchecked();

	if (MainValue != ShadowValueDecoded)
	{
		RegisterTamper(ESecureValueTamperReason::ShadowMismatch);
		return false;
	}

	return true;
}

void FSecureBool::Rekey()
{
	if (!bInitialized)
	{
		return;
	}

	const bool CurrentValue = Get();
	Key = GenerateRuntimeKey();
	EncodeValue(CurrentValue);
	TimeSinceLastRekey = 0.0f;
}

void FSecureBool::UpdateProtection(float DeltaSeconds)
{
	if (!bInitialized || !Rules.bEnableAutoRekey || Rules.RekeyIntervalSeconds <= 0.0f)
	{
		return;
	}

	TimeSinceLastRekey += FMath::Max(0.0f, DeltaSeconds);
	if (TimeSinceLastRekey >= Rules.RekeyIntervalSeconds)
	{
		Rekey();
	}
}

void FSecureBool::ResetToDefault()
{
	if (!bInitialized)
	{
		Key = GenerateRuntimeKey();
		bInitialized = true;
	}

	const bool DefaultValue = Rules.DefaultValue;
	EncodeValue(DefaultValue);
	LastValidValue = DefaultValue;
	TimeSinceLastRekey = 0.0f;
	LastTamperReason = ESecureValueTamperReason::None;
}

void FSecureBool::CorruptForTesting()
{
	EncodedValue ^= 0xFFu;
}

void FSecureBool::EncodeValue(bool InValue)
{
	const uint8 Raw = InValue ? 1u : 0u;

	EncodedValue = Raw ^ Key;

	// Shadow is intentionally independent from key-driven encoding logic
	ShadowValue = Raw ^ 0xA5u;

	Integrity = ComputeIntegrity(EncodedValue, ShadowValue, Key);
}

bool FSecureBool::DecodeMainValueUnchecked() const
{
	const uint8 Raw = EncodedValue ^ Key;
	return Raw != 0;
}

bool FSecureBool::DecodeShadowValueUnchecked() const
{
	const uint8 Raw = ShadowValue ^ 0xA5u;
	return Raw != 0;
}

bool FSecureBool::RecoverValue()
{
	switch (Rules.RecoveryMode)
	{
	case ESecureValueRecoveryMode::RestoreLastValid:
		EncodeValue(LastValidValue);
		return LastValidValue;

	case ESecureValueRecoveryMode::ResetToDefault:
	default:
	{
		const bool DefaultValue = Rules.DefaultValue;
		EncodeValue(DefaultValue);
		LastValidValue = DefaultValue;
		return DefaultValue;
	}
	}
}

void FSecureBool::RegisterTamper(ESecureValueTamperReason Reason)
{
	++TamperCount;
	LastTamperReason = Reason;
}

uint32 FSecureBool::ComputeIntegrity(uint8 InEncoded, uint8 InShadow, uint8 InKey) const
{
	uint32 Hash = 2166136261u;
	Hash = (Hash ^ InEncoded) * 16777619u;
	Hash = (Hash ^ InShadow) * 16777619u;
	Hash = (Hash ^ InKey) * 16777619u;
	return Hash;
}

uint8 FSecureBool::GenerateRuntimeKey() const
{
	const uint64 Cycles = FPlatformTime::Cycles64();
	const uint32 TimePart = static_cast<uint32>(FDateTime::UtcNow().GetTicks());
	return static_cast<uint8>((Cycles ^ TimePart) & 0xFFu);
}

/* =========================
   FSecureInt64
   ========================= */

FSecureInt64::FSecureInt64()
{
	EncodedValue = 0;
	ShadowValue = 0;
	Key = 0;
	Integrity = 0;
	LastValidValue = 0;
	TamperCount = 0;
	EncodingPattern = 0;
	bInitialized = false;
	TimeSinceLastRekey = 0.0f;
	LastTamperReason = ESecureValueTamperReason::None;
}

bool FSecureInt64::ValidateRules(FString& OutError) const
{
	OutError.Reset();
	if (Rules.bUseRange && Rules.MinValue > Rules.MaxValue)
	{
		OutError = TEXT("MinValue cannot be greater than MaxValue.");
		return false;
	}
	if (Rules.bUseRange && (Rules.DefaultValue < Rules.MinValue || Rules.DefaultValue > Rules.MaxValue))
	{
		OutError = TEXT("DefaultValue must be inside the configured range.");
		return false;
	}
	if (Rules.bEnableAutoRekey && Rules.RekeyIntervalSeconds <= 0.0f)
	{
		OutError = TEXT("RekeyIntervalSeconds must be greater than zero when automatic re-keying is enabled.");
		return false;
	}
	if (static_cast<uint8>(Rules.RecoveryMode) > static_cast<uint8>(ESecureValueRecoveryMode::ResetToDefault))
	{
		OutError = TEXT("RecoveryMode is invalid.");
		return false;
	}
	return true;
}

void FSecureInt64::SetRules(const FSecureInt64Rules& InRules)
{
	Rules = InRules;
	FString RuleError;
	if (!ValidateRules(RuleError))
	{
		UE_LOG(LogShieldUE, Warning, TEXT("Invalid Secure Int64 rules: %s"), *RuleError);
	}
}

void FSecureInt64::Initialize(int64 InitialValue, const FSecureInt64Rules& InRules)
{
	Rules = InRules;
	FString RuleError;
	if (!ValidateRules(RuleError))
	{
		UE_LOG(LogShieldUE, Warning, TEXT("Invalid Secure Int64 rules: %s"), *RuleError);
	}
	Key = GenerateRuntimeKey();
	EncodingPattern = static_cast<uint8>(FMath::RandRange(0, 2));
	bInitialized = true;
	TimeSinceLastRekey = 0.0f;
	LastTamperReason = ESecureValueTamperReason::None;
	Set(InitialValue);
}

void FSecureInt64::Set(int64 NewValue)
{
	if (!bInitialized)
	{
		Key = GenerateRuntimeKey();
		EncodingPattern = static_cast<uint8>(FMath::RandRange(0, 2));
		bInitialized = true;
		TimeSinceLastRekey = 0.0f;
		LastTamperReason = ESecureValueTamperReason::None;
	}
	if (Rules.bUseRange)
	{
		NewValue = FMath::Clamp(NewValue, Rules.MinValue, Rules.MaxValue);
	}
	EncodeValue(NewValue);
	LastValidValue = NewValue;
	LastTamperReason = ESecureValueTamperReason::None;
}

int64 FSecureInt64::Get()
{
	if (!bInitialized)
	{
		RegisterTamper(ESecureValueTamperReason::NotInitialized);
		return Rules.DefaultValue;
	}
	if (ComputeIntegrity(EncodedValue, ShadowValue, Key) != Integrity)
	{
		RegisterTamper(ESecureValueTamperReason::IntegrityMismatch);
		return RecoverValue();
	}
	const int64 MainValue = DecodeMainValueUnchecked();
	if (MainValue != DecodeShadowValueUnchecked())
	{
		RegisterTamper(ESecureValueTamperReason::ShadowMismatch);
		return RecoverValue();
	}
	if (!IsWithinAllowedRange(MainValue))
	{
		RegisterTamper(ESecureValueTamperReason::RangeViolation);
		return RecoverValue();
	}
	return MainValue;
}

bool FSecureInt64::Validate()
{
	if (!bInitialized)
	{
		RegisterTamper(ESecureValueTamperReason::NotInitialized);
		return false;
	}
	if (ComputeIntegrity(EncodedValue, ShadowValue, Key) != Integrity)
	{
		RegisterTamper(ESecureValueTamperReason::IntegrityMismatch);
		return false;
	}
	const int64 MainValue = DecodeMainValueUnchecked();
	if (MainValue != DecodeShadowValueUnchecked())
	{
		RegisterTamper(ESecureValueTamperReason::ShadowMismatch);
		return false;
	}
	if (!IsWithinAllowedRange(MainValue))
	{
		RegisterTamper(ESecureValueTamperReason::RangeViolation);
		return false;
	}
	return true;
}

void FSecureInt64::Rekey()
{
	if (!bInitialized) return;
	const int64 CurrentValue = Get();
	Key = GenerateRuntimeKey();
	EncodeValue(CurrentValue);
	TimeSinceLastRekey = 0.0f;
}

void FSecureInt64::UpdateProtection(float DeltaSeconds)
{
	if (!bInitialized || !Rules.bEnableAutoRekey || Rules.RekeyIntervalSeconds <= 0.0f) return;
	TimeSinceLastRekey += FMath::Max(0.0f, DeltaSeconds);
	if (TimeSinceLastRekey >= Rules.RekeyIntervalSeconds) Rekey();
}

void FSecureInt64::ResetToDefault()
{
	if (!bInitialized)
	{
		Key = GenerateRuntimeKey();
		EncodingPattern = static_cast<uint8>(FMath::RandRange(0, 2));
		bInitialized = true;
	}
	EncodeValue(Rules.DefaultValue);
	LastValidValue = Rules.DefaultValue;
	TimeSinceLastRekey = 0.0f;
	LastTamperReason = ESecureValueTamperReason::None;
}

void FSecureInt64::CorruptForTesting()
{
	EncodedValue ^= 0x00FF00FF00FF00FFull;
}

void FSecureInt64::EncodeValue(int64 InValue)
{
	const uint64 Raw = static_cast<uint64>(InValue);
	const int32 Shift = static_cast<int32>(Key % 63ull) + 1;
	switch (EncodingPattern)
	{
	case 0: EncodedValue = Raw ^ Key; break;
	case 1: EncodedValue = ShieldUEPrivate::RotateLeft64(Raw ^ Key, Shift); break;
	default: EncodedValue = (Raw + Key) ^ ShieldUEPrivate::RotateRight64(Key, 13); break;
	}
	ShadowValue = Raw ^ 0xA5A5A5A5A5A5A5A5ull;
	Integrity = ComputeIntegrity(EncodedValue, ShadowValue, Key);
}

int64 FSecureInt64::DecodeMainValueUnchecked() const
{
	const int32 Shift = static_cast<int32>(Key % 63ull) + 1;
	uint64 Raw = 0;
	switch (EncodingPattern)
	{
	case 0: Raw = EncodedValue ^ Key; break;
	case 1: Raw = ShieldUEPrivate::RotateRight64(EncodedValue, Shift) ^ Key; break;
	default: Raw = (EncodedValue ^ ShieldUEPrivate::RotateRight64(Key, 13)) - Key; break;
	}
	return static_cast<int64>(Raw);
}

int64 FSecureInt64::DecodeShadowValueUnchecked() const
{
	return static_cast<int64>(ShadowValue ^ 0xA5A5A5A5A5A5A5A5ull);
}

bool FSecureInt64::IsWithinAllowedRange(int64 InValue) const
{
	return !Rules.bUseRange || (InValue >= Rules.MinValue && InValue <= Rules.MaxValue);
}

int64 FSecureInt64::RecoverValue()
{
	int64 Recovered = Rules.DefaultValue;
	if (Rules.RecoveryMode == ESecureValueRecoveryMode::RestoreLastValid)
	{
		Recovered = LastValidValue;
	}
	else if (Rules.RecoveryMode == ESecureValueRecoveryMode::ClampToRange)
	{
		Recovered = LastValidValue;
		if (Rules.bUseRange) Recovered = FMath::Clamp(Recovered, Rules.MinValue, Rules.MaxValue);
	}
	EncodeValue(Recovered);
	LastValidValue = Recovered;
	return Recovered;
}

void FSecureInt64::RegisterTamper(ESecureValueTamperReason Reason)
{
	++TamperCount;
	LastTamperReason = Reason;
}

uint32 FSecureInt64::ComputeIntegrity(uint64 InEncoded, uint64 InShadow, uint64 InKey) const
{
	uint32 Hash = 2166136261u;
	const uint32 Parts[] = {
		static_cast<uint32>(InEncoded), static_cast<uint32>(InEncoded >> 32),
		static_cast<uint32>(InShadow), static_cast<uint32>(InShadow >> 32),
		static_cast<uint32>(InKey), static_cast<uint32>(InKey >> 32)};
	for (const uint32 Part : Parts) Hash = (Hash ^ Part) * 16777619u;
	return Hash;
}

uint64 FSecureInt64::GenerateRuntimeKey() const
{
	return FPlatformTime::Cycles64() ^ static_cast<uint64>(FDateTime::UtcNow().GetTicks()) ^ 0x9E3779B97F4A7C15ull;
}

/* =========================
   FSecureDouble
   ========================= */

FSecureDouble::FSecureDouble()
{
	EncodedValueBits = 0;
	ShadowBits = 0;
	Key = 0;
	Integrity = 0;
	LastValidValue = 0.0;
	TamperCount = 0;
	EncodingPattern = 0;
	bInitialized = false;
	TimeSinceLastRekey = 0.0f;
	LastTamperReason = ESecureValueTamperReason::None;
}

bool FSecureDouble::ValidateRules(FString& OutError) const
{
	OutError.Reset();
	if (!FMath::IsFinite(Rules.MinValue) || !FMath::IsFinite(Rules.MaxValue) || !FMath::IsFinite(Rules.DefaultValue))
	{
		OutError = TEXT("Double rules cannot contain NaN or infinite values.");
		return false;
	}
	if (Rules.bUseRange && Rules.MinValue > Rules.MaxValue)
	{
		OutError = TEXT("MinValue cannot be greater than MaxValue.");
		return false;
	}
	if (Rules.bUseRange && (Rules.DefaultValue < Rules.MinValue || Rules.DefaultValue > Rules.MaxValue))
	{
		OutError = TEXT("DefaultValue must be inside the configured range.");
		return false;
	}
	if (Rules.bEnableAutoRekey && Rules.RekeyIntervalSeconds <= 0.0f)
	{
		OutError = TEXT("RekeyIntervalSeconds must be greater than zero when automatic re-keying is enabled.");
		return false;
	}
	if (static_cast<uint8>(Rules.RecoveryMode) > static_cast<uint8>(ESecureValueRecoveryMode::ResetToDefault))
	{
		OutError = TEXT("RecoveryMode is invalid.");
		return false;
	}
	return true;
}

void FSecureDouble::SetRules(const FSecureDoubleRules& InRules)
{
	Rules = InRules;
	FString RuleError;
	if (!ValidateRules(RuleError)) UE_LOG(LogShieldUE, Warning, TEXT("Invalid Secure Double rules: %s"), *RuleError);
}

void FSecureDouble::Initialize(double InitialValue, const FSecureDoubleRules& InRules)
{
	Rules = InRules;
	FString RuleError;
	if (!ValidateRules(RuleError)) UE_LOG(LogShieldUE, Warning, TEXT("Invalid Secure Double rules: %s"), *RuleError);
	Key = GenerateRuntimeKey();
	EncodingPattern = static_cast<uint8>(FMath::RandRange(0, 2));
	bInitialized = true;
	TimeSinceLastRekey = 0.0f;
	LastTamperReason = ESecureValueTamperReason::None;
	Set(InitialValue);
}

void FSecureDouble::Set(double NewValue)
{
	if (!bInitialized)
	{
		Key = GenerateRuntimeKey();
		EncodingPattern = static_cast<uint8>(FMath::RandRange(0, 2));
		bInitialized = true;
		TimeSinceLastRekey = 0.0f;
		LastTamperReason = ESecureValueTamperReason::None;
	}
	if (Rules.bUseRange) NewValue = FMath::Clamp(NewValue, Rules.MinValue, Rules.MaxValue);
	EncodeValue(NewValue);
	LastValidValue = NewValue;
	LastTamperReason = ESecureValueTamperReason::None;
}

double FSecureDouble::Get()
{
	if (!bInitialized)
	{
		RegisterTamper(ESecureValueTamperReason::NotInitialized);
		return Rules.DefaultValue;
	}
	if (ComputeIntegrity(EncodedValueBits, ShadowBits, Key) != Integrity)
	{
		RegisterTamper(ESecureValueTamperReason::IntegrityMismatch);
		return RecoverValue();
	}
	const double MainValue = DecodeMainValueUnchecked();
	if (ShieldUEPrivate::DoubleToBits(MainValue) != ShieldUEPrivate::DoubleToBits(DecodeShadowValueUnchecked()))
	{
		RegisterTamper(ESecureValueTamperReason::ShadowMismatch);
		return RecoverValue();
	}
	if (!IsWithinAllowedRange(MainValue))
	{
		RegisterTamper(ESecureValueTamperReason::RangeViolation);
		return RecoverValue();
	}
	return MainValue;
}

bool FSecureDouble::Validate()
{
	if (!bInitialized)
	{
		RegisterTamper(ESecureValueTamperReason::NotInitialized);
		return false;
	}
	if (ComputeIntegrity(EncodedValueBits, ShadowBits, Key) != Integrity)
	{
		RegisterTamper(ESecureValueTamperReason::IntegrityMismatch);
		return false;
	}
	const double MainValue = DecodeMainValueUnchecked();
	if (ShieldUEPrivate::DoubleToBits(MainValue) != ShieldUEPrivate::DoubleToBits(DecodeShadowValueUnchecked()))
	{
		RegisterTamper(ESecureValueTamperReason::ShadowMismatch);
		return false;
	}
	if (!IsWithinAllowedRange(MainValue))
	{
		RegisterTamper(ESecureValueTamperReason::RangeViolation);
		return false;
	}
	return true;
}

void FSecureDouble::Rekey()
{
	if (!bInitialized) return;
	const double CurrentValue = Get();
	Key = GenerateRuntimeKey();
	EncodeValue(CurrentValue);
	TimeSinceLastRekey = 0.0f;
}

void FSecureDouble::UpdateProtection(float DeltaSeconds)
{
	if (!bInitialized || !Rules.bEnableAutoRekey || Rules.RekeyIntervalSeconds <= 0.0f) return;
	TimeSinceLastRekey += FMath::Max(0.0f, DeltaSeconds);
	if (TimeSinceLastRekey >= Rules.RekeyIntervalSeconds) Rekey();
}

void FSecureDouble::ResetToDefault()
{
	if (!bInitialized)
	{
		Key = GenerateRuntimeKey();
		EncodingPattern = static_cast<uint8>(FMath::RandRange(0, 2));
		bInitialized = true;
	}
	EncodeValue(Rules.DefaultValue);
	LastValidValue = Rules.DefaultValue;
	TimeSinceLastRekey = 0.0f;
	LastTamperReason = ESecureValueTamperReason::None;
}

void FSecureDouble::CorruptForTesting()
{
	EncodedValueBits ^= 0x00FF00FF00FF00FFull;
}

void FSecureDouble::EncodeValue(double InValue)
{
	const uint64 RawBits = ShieldUEPrivate::DoubleToBits(InValue);
	const int32 Shift = static_cast<int32>(Key % 63ull) + 1;
	switch (EncodingPattern)
	{
	case 0: EncodedValueBits = RawBits ^ Key; break;
	case 1: EncodedValueBits = ShieldUEPrivate::RotateLeft64(RawBits ^ Key, Shift); break;
	default: EncodedValueBits = (RawBits + Key) ^ ShieldUEPrivate::RotateRight64(Key, 13); break;
	}
	ShadowBits = RawBits ^ 0xA5A5A5A5A5A5A5A5ull;
	Integrity = ComputeIntegrity(EncodedValueBits, ShadowBits, Key);
}

double FSecureDouble::DecodeMainValueUnchecked() const
{
	const int32 Shift = static_cast<int32>(Key % 63ull) + 1;
	uint64 RawBits = 0;
	switch (EncodingPattern)
	{
	case 0: RawBits = EncodedValueBits ^ Key; break;
	case 1: RawBits = ShieldUEPrivate::RotateRight64(EncodedValueBits, Shift) ^ Key; break;
	default: RawBits = (EncodedValueBits ^ ShieldUEPrivate::RotateRight64(Key, 13)) - Key; break;
	}
	return ShieldUEPrivate::BitsToDouble(RawBits);
}

double FSecureDouble::DecodeShadowValueUnchecked() const
{
	return ShieldUEPrivate::BitsToDouble(ShadowBits ^ 0xA5A5A5A5A5A5A5A5ull);
}

bool FSecureDouble::IsWithinAllowedRange(double InValue) const
{
	return !Rules.bUseRange || (InValue >= Rules.MinValue && InValue <= Rules.MaxValue);
}

double FSecureDouble::RecoverValue()
{
	double Recovered = Rules.DefaultValue;
	if (Rules.RecoveryMode == ESecureValueRecoveryMode::RestoreLastValid) Recovered = LastValidValue;
	else if (Rules.RecoveryMode == ESecureValueRecoveryMode::ClampToRange)
	{
		Recovered = LastValidValue;
		if (Rules.bUseRange) Recovered = FMath::Clamp(Recovered, Rules.MinValue, Rules.MaxValue);
	}
	EncodeValue(Recovered);
	LastValidValue = Recovered;
	return Recovered;
}

void FSecureDouble::RegisterTamper(ESecureValueTamperReason Reason)
{
	++TamperCount;
	LastTamperReason = Reason;
}

uint32 FSecureDouble::ComputeIntegrity(uint64 InEncoded, uint64 InShadow, uint64 InKey) const
{
	uint32 Hash = 2166136261u;
	const uint32 Parts[] = {
		static_cast<uint32>(InEncoded), static_cast<uint32>(InEncoded >> 32),
		static_cast<uint32>(InShadow), static_cast<uint32>(InShadow >> 32),
		static_cast<uint32>(InKey), static_cast<uint32>(InKey >> 32)};
	for (const uint32 Part : Parts) Hash = (Hash ^ Part) * 16777619u;
	return Hash;
}

uint64 FSecureDouble::GenerateRuntimeKey() const
{
	return FPlatformTime::Cycles64() ^ static_cast<uint64>(FDateTime::UtcNow().GetTicks()) ^ 0xD1B54A32D192ED03ull;
}

/* =========================
   FSecureByte
   ========================= */

FSecureByte::FSecureByte()
{
	EncodedValue = 0;
	ShadowValue = 0;
	Key = 0;
	Integrity = 0;
	LastValidValue = 0;
	TamperCount = 0;
	bInitialized = false;
	TimeSinceLastRekey = 0.0f;
	LastTamperReason = ESecureValueTamperReason::None;
}

bool FSecureByte::ValidateRules(FString& OutError) const
{
	OutError.Reset();
	if (Rules.bUseRange && Rules.MinValue > Rules.MaxValue)
	{
		OutError = TEXT("MinValue cannot be greater than MaxValue.");
		return false;
	}
	if (Rules.bUseRange && (Rules.DefaultValue < Rules.MinValue || Rules.DefaultValue > Rules.MaxValue))
	{
		OutError = TEXT("DefaultValue must be inside the configured range.");
		return false;
	}
	if (Rules.bEnableAutoRekey && Rules.RekeyIntervalSeconds <= 0.0f)
	{
		OutError = TEXT("RekeyIntervalSeconds must be greater than zero when automatic re-keying is enabled.");
		return false;
	}
	if (static_cast<uint8>(Rules.RecoveryMode) > static_cast<uint8>(ESecureValueRecoveryMode::ResetToDefault))
	{
		OutError = TEXT("RecoveryMode is invalid.");
		return false;
	}
	return true;
}

void FSecureByte::SetRules(const FSecureByteRules& InRules)
{
	Rules = InRules;
	FString RuleError;
	if (!ValidateRules(RuleError)) UE_LOG(LogShieldUE, Warning, TEXT("Invalid Secure Byte rules: %s"), *RuleError);
}

void FSecureByte::Initialize(uint8 InitialValue, const FSecureByteRules& InRules)
{
	Rules = InRules;
	FString RuleError;
	if (!ValidateRules(RuleError)) UE_LOG(LogShieldUE, Warning, TEXT("Invalid Secure Byte rules: %s"), *RuleError);
	Key = GenerateRuntimeKey();
	bInitialized = true;
	TimeSinceLastRekey = 0.0f;
	LastTamperReason = ESecureValueTamperReason::None;
	Set(InitialValue);
}

void FSecureByte::Set(uint8 NewValue)
{
	if (!bInitialized)
	{
		Key = GenerateRuntimeKey();
		bInitialized = true;
		TimeSinceLastRekey = 0.0f;
		LastTamperReason = ESecureValueTamperReason::None;
	}
	if (Rules.bUseRange) NewValue = FMath::Clamp(NewValue, Rules.MinValue, Rules.MaxValue);
	EncodeValue(NewValue);
	LastValidValue = NewValue;
	LastTamperReason = ESecureValueTamperReason::None;
}

uint8 FSecureByte::Get()
{
	if (!bInitialized)
	{
		RegisterTamper(ESecureValueTamperReason::NotInitialized);
		return Rules.DefaultValue;
	}
	if (ComputeIntegrity(EncodedValue, ShadowValue, Key) != Integrity)
	{
		RegisterTamper(ESecureValueTamperReason::IntegrityMismatch);
		return RecoverValue();
	}
	const uint8 MainValue = DecodeMainValueUnchecked();
	if (MainValue != DecodeShadowValueUnchecked())
	{
		RegisterTamper(ESecureValueTamperReason::ShadowMismatch);
		return RecoverValue();
	}
	if (!IsWithinAllowedRange(MainValue))
	{
		RegisterTamper(ESecureValueTamperReason::RangeViolation);
		return RecoverValue();
	}
	return MainValue;
}

bool FSecureByte::Validate()
{
	if (!bInitialized)
	{
		RegisterTamper(ESecureValueTamperReason::NotInitialized);
		return false;
	}
	if (ComputeIntegrity(EncodedValue, ShadowValue, Key) != Integrity)
	{
		RegisterTamper(ESecureValueTamperReason::IntegrityMismatch);
		return false;
	}
	const uint8 MainValue = DecodeMainValueUnchecked();
	if (MainValue != DecodeShadowValueUnchecked())
	{
		RegisterTamper(ESecureValueTamperReason::ShadowMismatch);
		return false;
	}
	if (!IsWithinAllowedRange(MainValue))
	{
		RegisterTamper(ESecureValueTamperReason::RangeViolation);
		return false;
	}
	return true;
}

void FSecureByte::Rekey()
{
	if (!bInitialized) return;
	const uint8 CurrentValue = Get();
	Key = GenerateRuntimeKey();
	EncodeValue(CurrentValue);
	TimeSinceLastRekey = 0.0f;
}

void FSecureByte::UpdateProtection(float DeltaSeconds)
{
	if (!bInitialized || !Rules.bEnableAutoRekey || Rules.RekeyIntervalSeconds <= 0.0f) return;
	TimeSinceLastRekey += FMath::Max(0.0f, DeltaSeconds);
	if (TimeSinceLastRekey >= Rules.RekeyIntervalSeconds) Rekey();
}

void FSecureByte::ResetToDefault()
{
	if (!bInitialized)
	{
		Key = GenerateRuntimeKey();
		bInitialized = true;
	}
	EncodeValue(Rules.DefaultValue);
	LastValidValue = Rules.DefaultValue;
	TimeSinceLastRekey = 0.0f;
	LastTamperReason = ESecureValueTamperReason::None;
}

void FSecureByte::CorruptForTesting()
{
	EncodedValue ^= 0xFFu;
}

void FSecureByte::EncodeValue(uint8 InValue)
{
	EncodedValue = InValue ^ Key;
	ShadowValue = InValue ^ 0xA5u;
	Integrity = ComputeIntegrity(EncodedValue, ShadowValue, Key);
}

uint8 FSecureByte::DecodeMainValueUnchecked() const
{
	return EncodedValue ^ Key;
}

uint8 FSecureByte::DecodeShadowValueUnchecked() const
{
	return ShadowValue ^ 0xA5u;
}

bool FSecureByte::IsWithinAllowedRange(uint8 InValue) const
{
	return !Rules.bUseRange || (InValue >= Rules.MinValue && InValue <= Rules.MaxValue);
}

uint8 FSecureByte::RecoverValue()
{
	uint8 Recovered = Rules.DefaultValue;
	if (Rules.RecoveryMode == ESecureValueRecoveryMode::RestoreLastValid) Recovered = LastValidValue;
	else if (Rules.RecoveryMode == ESecureValueRecoveryMode::ClampToRange)
	{
		Recovered = LastValidValue;
		if (Rules.bUseRange) Recovered = FMath::Clamp(Recovered, Rules.MinValue, Rules.MaxValue);
	}
	EncodeValue(Recovered);
	LastValidValue = Recovered;
	return Recovered;
}

void FSecureByte::RegisterTamper(ESecureValueTamperReason Reason)
{
	++TamperCount;
	LastTamperReason = Reason;
}

uint32 FSecureByte::ComputeIntegrity(uint8 InEncoded, uint8 InShadow, uint8 InKey) const
{
	uint32 Hash = 2166136261u;
	Hash = (Hash ^ InEncoded) * 16777619u;
	Hash = (Hash ^ InShadow) * 16777619u;
	Hash = (Hash ^ InKey) * 16777619u;
	return Hash;
}

uint8 FSecureByte::GenerateRuntimeKey() const
{
	const uint64 Cycles = FPlatformTime::Cycles64();
	const uint64 TimePart = static_cast<uint64>(FDateTime::UtcNow().GetTicks());
	return static_cast<uint8>((Cycles ^ TimePart ^ 0x6Du) & 0xFFu);
}
