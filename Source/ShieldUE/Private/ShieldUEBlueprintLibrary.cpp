#include "ShieldUEBlueprintLibrary.h"


/* FLOAT */

void UShieldUEBlueprintLibrary::SecureFloat_Initialize(FSecureFloat& Value, float InitialValue, const FSecureFloatRules& Rules)
{
	Value.Initialize(InitialValue, Rules);
}

void UShieldUEBlueprintLibrary::SecureFloat_Set(FSecureFloat& Value, float NewValue)
{
	Value.Set(NewValue);
}

float UShieldUEBlueprintLibrary::SecureFloat_Get(FSecureFloat& Value)
{
	return Value.Get();
}

bool UShieldUEBlueprintLibrary::SecureFloat_Validate(FSecureFloat& Value)
{
	return Value.Validate();
}

void UShieldUEBlueprintLibrary::SecureFloat_Rekey(FSecureFloat& Value)
{
	Value.Rekey();
}

void UShieldUEBlueprintLibrary::SecureFloat_UpdateProtection(FSecureFloat& Value, float DeltaSeconds)
{
	Value.UpdateProtection(DeltaSeconds);
}

void UShieldUEBlueprintLibrary::SecureFloat_ResetToDefault(FSecureFloat& Value)
{
	Value.ResetToDefault();
}

int32 UShieldUEBlueprintLibrary::SecureFloat_GetTamperCount(const FSecureFloat& Value)
{
	return static_cast<int32>(Value.GetTamperCount());
}

ESecureValueTamperReason UShieldUEBlueprintLibrary::SecureFloat_GetLastTamperReason(const FSecureFloat& Value)
{
	return Value.GetLastTamperReason();
}

bool UShieldUEBlueprintLibrary::SecureFloat_IsInitialized(const FSecureFloat& Value)
{
	return Value.IsInitialized();
}

float UShieldUEBlueprintLibrary::SecureFloat_GetLastValidValue(const FSecureFloat& Value)
{
	return Value.GetLastValidValue();
}

void UShieldUEBlueprintLibrary::SecureFloat_CorruptForTesting(FSecureFloat& Value)
{
	Value.CorruptForTesting();
}

bool UShieldUEBlueprintLibrary::SecureFloat_ValidateRules(const FSecureFloatRules& Rules, FString& OutError)
{
	FSecureFloat Value;
	Value.SetRules(Rules);
	return Value.ValidateRules(OutError);
}

void UShieldUEBlueprintLibrary::SecureFloat_InitializeFromInternalRules(FSecureFloat& Value, float InitialValue)
{
	Value.Initialize(InitialValue, Value.GetRules());
}


/* INT */

void UShieldUEBlueprintLibrary::SecureInt_Initialize(FSecureInt32& Value, int32 InitialValue, const FSecureInt32Rules& Rules)
{
	Value.Initialize(InitialValue, Rules);
}

void UShieldUEBlueprintLibrary::SecureInt_Set(FSecureInt32& Value, int32 NewValue)
{
	Value.Set(NewValue);
}

int32 UShieldUEBlueprintLibrary::SecureInt_Get(FSecureInt32& Value)
{
	return Value.Get();
}

bool UShieldUEBlueprintLibrary::SecureInt_Validate(FSecureInt32& Value)
{
	return Value.Validate();
}

void UShieldUEBlueprintLibrary::SecureInt_Rekey(FSecureInt32& Value)
{
	Value.Rekey();
}

void UShieldUEBlueprintLibrary::SecureInt_UpdateProtection(FSecureInt32& Value, float DeltaSeconds)
{
	Value.UpdateProtection(DeltaSeconds);
}

void UShieldUEBlueprintLibrary::SecureInt_ResetToDefault(FSecureInt32& Value)
{
	Value.ResetToDefault();
}

int32 UShieldUEBlueprintLibrary::SecureInt_GetTamperCount(const FSecureInt32& Value)
{
	return static_cast<int32>(Value.GetTamperCount());
}

ESecureValueTamperReason UShieldUEBlueprintLibrary::SecureInt_GetLastTamperReason(const FSecureInt32& Value)
{
	return Value.GetLastTamperReason();
}

bool UShieldUEBlueprintLibrary::SecureInt_IsInitialized(const FSecureInt32& Value)
{
	return Value.IsInitialized();
}

int32 UShieldUEBlueprintLibrary::SecureInt_GetLastValidValue(const FSecureInt32& Value)
{
	return Value.GetLastValidValue();
}

void UShieldUEBlueprintLibrary::SecureInt_CorruptForTesting(FSecureInt32& Value)
{
	Value.CorruptForTesting();
}

bool UShieldUEBlueprintLibrary::SecureInt_ValidateRules(const FSecureInt32Rules& Rules, FString& OutError)
{
	FSecureInt32 Value;
	Value.SetRules(Rules);
	return Value.ValidateRules(OutError);
}

void UShieldUEBlueprintLibrary::SecureInt_InitializeFromInternalRules(FSecureInt32& Value, int32 InitialValue)
{
	Value.Initialize(InitialValue, Value.GetRules());
}


/* BOOL */

void UShieldUEBlueprintLibrary::SecureBool_Initialize(FSecureBool& Value, bool InitialValue, const FSecureBoolRules& Rules)
{
	Value.Initialize(InitialValue, Rules);
}

void UShieldUEBlueprintLibrary::SecureBool_Set(FSecureBool& Value, bool NewValue)
{
	Value.Set(NewValue);
}

bool UShieldUEBlueprintLibrary::SecureBool_Get(FSecureBool& Value)
{
	return Value.Get();
}

bool UShieldUEBlueprintLibrary::SecureBool_Validate(FSecureBool& Value)
{
	return Value.Validate();
}

void UShieldUEBlueprintLibrary::SecureBool_Rekey(FSecureBool& Value)
{
	Value.Rekey();
}

void UShieldUEBlueprintLibrary::SecureBool_UpdateProtection(FSecureBool& Value, float DeltaSeconds)
{
	Value.UpdateProtection(DeltaSeconds);
}

void UShieldUEBlueprintLibrary::SecureBool_ResetToDefault(FSecureBool& Value)
{
	Value.ResetToDefault();
}

int32 UShieldUEBlueprintLibrary::SecureBool_GetTamperCount(const FSecureBool& Value)
{
	return static_cast<int32>(Value.GetTamperCount());
}

ESecureValueTamperReason UShieldUEBlueprintLibrary::SecureBool_GetLastTamperReason(const FSecureBool& Value)
{
	return Value.GetLastTamperReason();
}

bool UShieldUEBlueprintLibrary::SecureBool_IsInitialized(const FSecureBool& Value)
{
	return Value.IsInitialized();
}

bool UShieldUEBlueprintLibrary::SecureBool_GetLastValidValue(const FSecureBool& Value)
{
	return Value.GetLastValidValue();
}

void UShieldUEBlueprintLibrary::SecureBool_CorruptForTesting(FSecureBool& Value)
{
	Value.CorruptForTesting();
}

bool UShieldUEBlueprintLibrary::SecureBool_ValidateRules(const FSecureBoolRules& Rules, FString& OutError)
{
	FSecureBool Value;
	Value.SetRules(Rules);
	return Value.ValidateRules(OutError);
}

void UShieldUEBlueprintLibrary::SecureBool_InitializeFromInternalRules(FSecureBool& Value, bool InitialValue)
{
	Value.Initialize(InitialValue, Value.GetRules());
}

/* INT64 */

void UShieldUEBlueprintLibrary::SecureInt64_Initialize(FSecureInt64& Value, int64 InitialValue, const FSecureInt64Rules& Rules) { Value.Initialize(InitialValue, Rules); }
void UShieldUEBlueprintLibrary::SecureInt64_Set(FSecureInt64& Value, int64 NewValue) { Value.Set(NewValue); }
int64 UShieldUEBlueprintLibrary::SecureInt64_Get(FSecureInt64& Value) { return Value.Get(); }
bool UShieldUEBlueprintLibrary::SecureInt64_Validate(FSecureInt64& Value) { return Value.Validate(); }
void UShieldUEBlueprintLibrary::SecureInt64_Rekey(FSecureInt64& Value) { Value.Rekey(); }
void UShieldUEBlueprintLibrary::SecureInt64_UpdateProtection(FSecureInt64& Value, float DeltaSeconds) { Value.UpdateProtection(DeltaSeconds); }
void UShieldUEBlueprintLibrary::SecureInt64_ResetToDefault(FSecureInt64& Value) { Value.ResetToDefault(); }
int32 UShieldUEBlueprintLibrary::SecureInt64_GetTamperCount(const FSecureInt64& Value) { return static_cast<int32>(Value.GetTamperCount()); }
ESecureValueTamperReason UShieldUEBlueprintLibrary::SecureInt64_GetLastTamperReason(const FSecureInt64& Value) { return Value.GetLastTamperReason(); }
bool UShieldUEBlueprintLibrary::SecureInt64_IsInitialized(const FSecureInt64& Value) { return Value.IsInitialized(); }
int64 UShieldUEBlueprintLibrary::SecureInt64_GetLastValidValue(const FSecureInt64& Value) { return Value.GetLastValidValue(); }
void UShieldUEBlueprintLibrary::SecureInt64_CorruptForTesting(FSecureInt64& Value) { Value.CorruptForTesting(); }
bool UShieldUEBlueprintLibrary::SecureInt64_ValidateRules(const FSecureInt64Rules& Rules, FString& OutError)
{
	FSecureInt64 Value;
	Value.SetRules(Rules);
	return Value.ValidateRules(OutError);
}
void UShieldUEBlueprintLibrary::SecureInt64_InitializeFromInternalRules(FSecureInt64& Value, int64 InitialValue) { Value.Initialize(InitialValue, Value.GetRules()); }

/* DOUBLE */

void UShieldUEBlueprintLibrary::SecureDouble_Initialize(FSecureDouble& Value, double InitialValue, const FSecureDoubleRules& Rules) { Value.Initialize(InitialValue, Rules); }
void UShieldUEBlueprintLibrary::SecureDouble_Set(FSecureDouble& Value, double NewValue) { Value.Set(NewValue); }
double UShieldUEBlueprintLibrary::SecureDouble_Get(FSecureDouble& Value) { return Value.Get(); }
bool UShieldUEBlueprintLibrary::SecureDouble_Validate(FSecureDouble& Value) { return Value.Validate(); }
void UShieldUEBlueprintLibrary::SecureDouble_Rekey(FSecureDouble& Value) { Value.Rekey(); }
void UShieldUEBlueprintLibrary::SecureDouble_UpdateProtection(FSecureDouble& Value, float DeltaSeconds) { Value.UpdateProtection(DeltaSeconds); }
void UShieldUEBlueprintLibrary::SecureDouble_ResetToDefault(FSecureDouble& Value) { Value.ResetToDefault(); }
int32 UShieldUEBlueprintLibrary::SecureDouble_GetTamperCount(const FSecureDouble& Value) { return static_cast<int32>(Value.GetTamperCount()); }
ESecureValueTamperReason UShieldUEBlueprintLibrary::SecureDouble_GetLastTamperReason(const FSecureDouble& Value) { return Value.GetLastTamperReason(); }
bool UShieldUEBlueprintLibrary::SecureDouble_IsInitialized(const FSecureDouble& Value) { return Value.IsInitialized(); }
double UShieldUEBlueprintLibrary::SecureDouble_GetLastValidValue(const FSecureDouble& Value) { return Value.GetLastValidValue(); }
void UShieldUEBlueprintLibrary::SecureDouble_CorruptForTesting(FSecureDouble& Value) { Value.CorruptForTesting(); }
bool UShieldUEBlueprintLibrary::SecureDouble_ValidateRules(const FSecureDoubleRules& Rules, FString& OutError)
{
	FSecureDouble Value;
	Value.SetRules(Rules);
	return Value.ValidateRules(OutError);
}
void UShieldUEBlueprintLibrary::SecureDouble_InitializeFromInternalRules(FSecureDouble& Value, double InitialValue) { Value.Initialize(InitialValue, Value.GetRules()); }

/* BYTE */

void UShieldUEBlueprintLibrary::SecureByte_Initialize(FSecureByte& Value, uint8 InitialValue, const FSecureByteRules& Rules) { Value.Initialize(InitialValue, Rules); }
void UShieldUEBlueprintLibrary::SecureByte_Set(FSecureByte& Value, uint8 NewValue) { Value.Set(NewValue); }
uint8 UShieldUEBlueprintLibrary::SecureByte_Get(FSecureByte& Value) { return Value.Get(); }
bool UShieldUEBlueprintLibrary::SecureByte_Validate(FSecureByte& Value) { return Value.Validate(); }
void UShieldUEBlueprintLibrary::SecureByte_Rekey(FSecureByte& Value) { Value.Rekey(); }
void UShieldUEBlueprintLibrary::SecureByte_UpdateProtection(FSecureByte& Value, float DeltaSeconds) { Value.UpdateProtection(DeltaSeconds); }
void UShieldUEBlueprintLibrary::SecureByte_ResetToDefault(FSecureByte& Value) { Value.ResetToDefault(); }
int32 UShieldUEBlueprintLibrary::SecureByte_GetTamperCount(const FSecureByte& Value) { return static_cast<int32>(Value.GetTamperCount()); }
ESecureValueTamperReason UShieldUEBlueprintLibrary::SecureByte_GetLastTamperReason(const FSecureByte& Value) { return Value.GetLastTamperReason(); }
bool UShieldUEBlueprintLibrary::SecureByte_IsInitialized(const FSecureByte& Value) { return Value.IsInitialized(); }
uint8 UShieldUEBlueprintLibrary::SecureByte_GetLastValidValue(const FSecureByte& Value) { return Value.GetLastValidValue(); }
void UShieldUEBlueprintLibrary::SecureByte_CorruptForTesting(FSecureByte& Value) { Value.CorruptForTesting(); }
bool UShieldUEBlueprintLibrary::SecureByte_ValidateRules(const FSecureByteRules& Rules, FString& OutError)
{
	FSecureByte Value;
	Value.SetRules(Rules);
	return Value.ValidateRules(OutError);
}
void UShieldUEBlueprintLibrary::SecureByte_InitializeFromInternalRules(FSecureByte& Value, uint8 InitialValue) { Value.Initialize(InitialValue, Value.GetRules()); }

/* VECTOR */

void UShieldUEBlueprintLibrary::SecureVector_Initialize(FSecureVector& Value, const FVector& InitialValue, const FSecureVectorRules& Rules) { Value.Initialize(InitialValue, Rules); }
void UShieldUEBlueprintLibrary::SecureVector_Set(FSecureVector& Value, const FVector& NewValue) { Value.Set(NewValue); }
FVector UShieldUEBlueprintLibrary::SecureVector_Get(FSecureVector& Value) { return Value.Get(); }
bool UShieldUEBlueprintLibrary::SecureVector_Validate(FSecureVector& Value) { return Value.Validate(); }
void UShieldUEBlueprintLibrary::SecureVector_Rekey(FSecureVector& Value) { Value.Rekey(); }
void UShieldUEBlueprintLibrary::SecureVector_UpdateProtection(FSecureVector& Value, float DeltaSeconds) { Value.UpdateProtection(DeltaSeconds); }
void UShieldUEBlueprintLibrary::SecureVector_ResetToDefault(FSecureVector& Value) { Value.ResetToDefault(); }
int32 UShieldUEBlueprintLibrary::SecureVector_GetTamperCount(const FSecureVector& Value) { return static_cast<int32>(Value.GetTamperCount()); }
ESecureValueTamperReason UShieldUEBlueprintLibrary::SecureVector_GetLastTamperReason(const FSecureVector& Value) { return Value.GetLastTamperReason(); }
bool UShieldUEBlueprintLibrary::SecureVector_IsInitialized(const FSecureVector& Value) { return Value.IsInitialized(); }
FVector UShieldUEBlueprintLibrary::SecureVector_GetLastValidValue(const FSecureVector& Value) { return Value.GetLastValidValue(); }
void UShieldUEBlueprintLibrary::SecureVector_CorruptForTesting(FSecureVector& Value) { Value.CorruptForTesting(); }
bool UShieldUEBlueprintLibrary::SecureVector_ValidateRules(const FSecureVectorRules& Rules, FString& OutError)
{
	FSecureVector Value;
	Value.SetRules(Rules);
	return Value.ValidateRules(OutError);
}
void UShieldUEBlueprintLibrary::SecureVector_InitializeFromInternalRules(FSecureVector& Value, const FVector& InitialValue) { Value.Initialize(InitialValue, Value.GetRules()); }

/* ROTATOR */

void UShieldUEBlueprintLibrary::SecureRotator_Initialize(FSecureRotator& Value, const FRotator& InitialValue, const FSecureVectorRules& Rules) { Value.Initialize(InitialValue, Rules); }
void UShieldUEBlueprintLibrary::SecureRotator_Set(FSecureRotator& Value, const FRotator& NewValue) { Value.Set(NewValue); }
FRotator UShieldUEBlueprintLibrary::SecureRotator_Get(FSecureRotator& Value) { return Value.Get(); }
bool UShieldUEBlueprintLibrary::SecureRotator_Validate(FSecureRotator& Value) { return Value.Validate(); }
void UShieldUEBlueprintLibrary::SecureRotator_Rekey(FSecureRotator& Value) { Value.Rekey(); }
void UShieldUEBlueprintLibrary::SecureRotator_UpdateProtection(FSecureRotator& Value, float DeltaSeconds) { Value.UpdateProtection(DeltaSeconds); }
void UShieldUEBlueprintLibrary::SecureRotator_ResetToDefault(FSecureRotator& Value) { Value.ResetToDefault(); }
int32 UShieldUEBlueprintLibrary::SecureRotator_GetTamperCount(const FSecureRotator& Value) { return static_cast<int32>(Value.GetTamperCount()); }
ESecureValueTamperReason UShieldUEBlueprintLibrary::SecureRotator_GetLastTamperReason(const FSecureRotator& Value) { return Value.GetLastTamperReason(); }
bool UShieldUEBlueprintLibrary::SecureRotator_IsInitialized(const FSecureRotator& Value) { return Value.IsInitialized(); }
FRotator UShieldUEBlueprintLibrary::SecureRotator_GetLastValidValue(const FSecureRotator& Value) { return Value.GetLastValidValue(); }
void UShieldUEBlueprintLibrary::SecureRotator_CorruptForTesting(FSecureRotator& Value) { Value.CorruptForTesting(); }
bool UShieldUEBlueprintLibrary::SecureRotator_ValidateRules(const FSecureVectorRules& Rules, FString& OutError)
{
	FSecureRotator Value;
	Value.SetRules(Rules);
	return Value.ValidateRules(OutError);
}
void UShieldUEBlueprintLibrary::SecureRotator_InitializeFromInternalRules(FSecureRotator& Value, const FRotator& InitialValue) { Value.Initialize(InitialValue, Value.GetRules()); }

/* TRANSFORM */

void UShieldUEBlueprintLibrary::SecureTransform_Initialize(FSecureTransform& Value, const FTransform& InitialValue, const FSecureTransformRules& Rules) { Value.Initialize(InitialValue, Rules); }
void UShieldUEBlueprintLibrary::SecureTransform_Set(FSecureTransform& Value, const FTransform& NewValue) { Value.Set(NewValue); }
FTransform UShieldUEBlueprintLibrary::SecureTransform_Get(FSecureTransform& Value) { return Value.Get(); }
bool UShieldUEBlueprintLibrary::SecureTransform_Validate(FSecureTransform& Value) { return Value.Validate(); }
void UShieldUEBlueprintLibrary::SecureTransform_Rekey(FSecureTransform& Value) { Value.Rekey(); }
void UShieldUEBlueprintLibrary::SecureTransform_UpdateProtection(FSecureTransform& Value, float DeltaSeconds) { Value.UpdateProtection(DeltaSeconds); }
void UShieldUEBlueprintLibrary::SecureTransform_ResetToDefault(FSecureTransform& Value) { Value.ResetToDefault(); }
int32 UShieldUEBlueprintLibrary::SecureTransform_GetTamperCount(const FSecureTransform& Value) { return static_cast<int32>(Value.GetTamperCount()); }
ESecureValueTamperReason UShieldUEBlueprintLibrary::SecureTransform_GetLastTamperReason(const FSecureTransform& Value) { return Value.GetLastTamperReason(); }
bool UShieldUEBlueprintLibrary::SecureTransform_IsInitialized(const FSecureTransform& Value) { return Value.IsInitialized(); }
FTransform UShieldUEBlueprintLibrary::SecureTransform_GetLastValidValue(const FSecureTransform& Value) { return Value.GetLastValidValue(); }
void UShieldUEBlueprintLibrary::SecureTransform_CorruptForTesting(FSecureTransform& Value) { Value.CorruptForTesting(); }
bool UShieldUEBlueprintLibrary::SecureTransform_ValidateRules(const FSecureTransformRules& Rules, FString& OutError)
{
	FSecureTransform Value;
	Value.SetRules(Rules);
	return Value.ValidateRules(OutError);
}
void UShieldUEBlueprintLibrary::SecureTransform_InitializeFromInternalRules(FSecureTransform& Value, const FTransform& InitialValue) { Value.Initialize(InitialValue, Value.GetRules()); }
