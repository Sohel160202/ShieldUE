#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SecureValueTypes.h"
#include "ShieldUEBlueprintLibrary.generated.h"

UCLASS()
class SHIELDUE_API UShieldUEBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/* FLOAT */

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Float", meta = (ToolTip = "Initialize a Secure Float with a starting value and protection rules."))
	static void SecureFloat_Initialize(UPARAM(ref) FSecureFloat& Value, float InitialValue, const FSecureFloatRules& Rules);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Float", meta = (ToolTip = "Set the current value of a Secure Float."))
	static void SecureFloat_Set(UPARAM(ref) FSecureFloat& Value, float NewValue);

	UFUNCTION(BlueprintPure, Category = "ShieldUE|Float", meta = (ToolTip = "Get the decoded runtime value from a Secure Float."))
	static float SecureFloat_Get(UPARAM(ref) FSecureFloat& Value);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Float", meta = (ToolTip = "Validate a Secure Float and return whether its protected state is still valid."))
	static bool SecureFloat_Validate(UPARAM(ref) FSecureFloat& Value);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Float", meta = (ToolTip = "Generate a new runtime key and re-encode the Secure Float."))
	static void SecureFloat_Rekey(UPARAM(ref) FSecureFloat& Value);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Float", meta = (ToolTip = "Update protection timers. Call from Tick when automatic re-keying is enabled."))
	static void SecureFloat_UpdateProtection(UPARAM(ref) FSecureFloat& Value, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Float", meta = (ToolTip = "Restore the Secure Float to its configured default value."))
	static void SecureFloat_ResetToDefault(UPARAM(ref) FSecureFloat& Value);

	UFUNCTION(BlueprintPure, Category = "ShieldUE|Float", meta = (ToolTip = "Return the number of detected tampering events."))
	static int32 SecureFloat_GetTamperCount(const FSecureFloat& Value);

	UFUNCTION(BlueprintPure, Category = "ShieldUE|Float", meta = (ToolTip = "Return the reason for the most recent detected tampering event."))
	static ESecureValueTamperReason SecureFloat_GetLastTamperReason(const FSecureFloat& Value);

	UFUNCTION(BlueprintPure, Category = "ShieldUE|Float", meta = (ToolTip = "Return whether the Secure Float has been initialized."))
	static bool SecureFloat_IsInitialized(const FSecureFloat& Value);

	UFUNCTION(BlueprintPure, Category = "ShieldUE|Float", meta = (ToolTip = "Return the last valid value recorded by the Secure Float."))
	static float SecureFloat_GetLastValidValue(const FSecureFloat& Value);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Float|Testing", meta = (ToolTip = "Intentionally corrupt a Secure Float for development testing. Do not use in shipping gameplay."))
	static void SecureFloat_CorruptForTesting(UPARAM(ref) FSecureFloat& Value);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Float", meta = (ToolTip = "Validate Secure Float rules and return a human-readable error when invalid."))
	static bool SecureFloat_ValidateRules(const FSecureFloatRules& Rules, FString& OutError);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Float", meta = (ToolTip = "Initialize a Secure Float using the rules already stored inside the variable."))
	static void SecureFloat_InitializeFromInternalRules(UPARAM(ref) FSecureFloat& Value, float InitialValue);


	/* INT */

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Int", meta = (ToolTip = "Initialize a Secure Int with a starting value and protection rules."))
	static void SecureInt_Initialize(UPARAM(ref) FSecureInt32& Value, int32 InitialValue, const FSecureInt32Rules& Rules);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Int", meta = (ToolTip = "Set the current value of a Secure Integer."))
	static void SecureInt_Set(UPARAM(ref) FSecureInt32& Value, int32 NewValue);

	UFUNCTION(BlueprintPure, Category = "ShieldUE|Int", meta = (ToolTip = "Get the decoded runtime value from a Secure Integer."))
	static int32 SecureInt_Get(UPARAM(ref) FSecureInt32& Value);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Int", meta = (ToolTip = "Validate a Secure Integer and return whether its protected state is still valid."))
	static bool SecureInt_Validate(UPARAM(ref) FSecureInt32& Value);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Int", meta = (ToolTip = "Generate a new runtime key and re-encode the Secure Integer."))
	static void SecureInt_Rekey(UPARAM(ref) FSecureInt32& Value);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Int", meta = (ToolTip = "Update protection timers. Call from Tick when automatic re-keying is enabled."))
	static void SecureInt_UpdateProtection(UPARAM(ref) FSecureInt32& Value, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Int", meta = (ToolTip = "Restore the Secure Integer to its configured default value."))
	static void SecureInt_ResetToDefault(UPARAM(ref) FSecureInt32& Value);

	UFUNCTION(BlueprintPure, Category = "ShieldUE|Int", meta = (ToolTip = "Return the number of detected tampering events."))
	static int32 SecureInt_GetTamperCount(const FSecureInt32& Value);

	UFUNCTION(BlueprintPure, Category = "ShieldUE|Int", meta = (ToolTip = "Return the reason for the most recent detected tampering event."))
	static ESecureValueTamperReason SecureInt_GetLastTamperReason(const FSecureInt32& Value);

	UFUNCTION(BlueprintPure, Category = "ShieldUE|Int", meta = (ToolTip = "Return whether the Secure Integer has been initialized."))
	static bool SecureInt_IsInitialized(const FSecureInt32& Value);

	UFUNCTION(BlueprintPure, Category = "ShieldUE|Int", meta = (ToolTip = "Return the last valid value recorded by the Secure Integer."))
	static int32 SecureInt_GetLastValidValue(const FSecureInt32& Value);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Int|Testing", meta = (ToolTip = "Intentionally corrupt a Secure Integer for development testing. Do not use in shipping gameplay."))
	static void SecureInt_CorruptForTesting(UPARAM(ref) FSecureInt32& Value);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Int", meta = (ToolTip = "Validate Secure Integer rules and return a human-readable error when invalid."))
	static bool SecureInt_ValidateRules(const FSecureInt32Rules& Rules, FString& OutError);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Int", meta = (ToolTip = "Initialize a Secure Int using the rules already stored inside the variable."))
	static void SecureInt_InitializeFromInternalRules(UPARAM(ref) FSecureInt32& Value, int32 InitialValue);


	/* BOOL */

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Bool", meta = (ToolTip = "Initialize a Secure Bool with a starting value and protection rules."))
	static void SecureBool_Initialize(UPARAM(ref) FSecureBool& Value, bool InitialValue, const FSecureBoolRules& Rules);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Bool", meta = (ToolTip = "Set the current value of a Secure Bool."))
	static void SecureBool_Set(UPARAM(ref) FSecureBool& Value, bool NewValue);

	UFUNCTION(BlueprintPure, Category = "ShieldUE|Bool", meta = (ToolTip = "Get the decoded runtime value from a Secure Bool."))
	static bool SecureBool_Get(UPARAM(ref) FSecureBool& Value);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Bool", meta = (ToolTip = "Validate a Secure Bool and return whether its protected state is still valid."))
	static bool SecureBool_Validate(UPARAM(ref) FSecureBool& Value);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Bool", meta = (ToolTip = "Generate a new runtime key and re-encode the Secure Bool."))
	static void SecureBool_Rekey(UPARAM(ref) FSecureBool& Value);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Bool", meta = (ToolTip = "Update protection timers. Call from Tick when automatic re-keying is enabled."))
	static void SecureBool_UpdateProtection(UPARAM(ref) FSecureBool& Value, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Bool", meta = (ToolTip = "Restore the Secure Bool to its configured default value."))
	static void SecureBool_ResetToDefault(UPARAM(ref) FSecureBool& Value);

	UFUNCTION(BlueprintPure, Category = "ShieldUE|Bool", meta = (ToolTip = "Return the number of detected tampering events."))
	static int32 SecureBool_GetTamperCount(const FSecureBool& Value);

	UFUNCTION(BlueprintPure, Category = "ShieldUE|Bool", meta = (ToolTip = "Return the reason for the most recent detected tampering event."))
	static ESecureValueTamperReason SecureBool_GetLastTamperReason(const FSecureBool& Value);

	UFUNCTION(BlueprintPure, Category = "ShieldUE|Bool", meta = (ToolTip = "Return whether the Secure Bool has been initialized."))
	static bool SecureBool_IsInitialized(const FSecureBool& Value);

	UFUNCTION(BlueprintPure, Category = "ShieldUE|Bool", meta = (ToolTip = "Return the last valid value recorded by the Secure Bool."))
	static bool SecureBool_GetLastValidValue(const FSecureBool& Value);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Bool|Testing", meta = (ToolTip = "Intentionally corrupt a Secure Bool for development testing. Do not use in shipping gameplay."))
	static void SecureBool_CorruptForTesting(UPARAM(ref) FSecureBool& Value);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Bool", meta = (ToolTip = "Validate Secure Bool rules and return a human-readable error when invalid."))
	static bool SecureBool_ValidateRules(const FSecureBoolRules& Rules, FString& OutError);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Bool", meta = (ToolTip = "Initialize a Secure Bool using the rules already stored inside the variable."))
	static void SecureBool_InitializeFromInternalRules(UPARAM(ref) FSecureBool& Value, bool InitialValue);

	/* INT64 */

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Int64")
	static void SecureInt64_Initialize(UPARAM(ref) FSecureInt64& Value, int64 InitialValue, const FSecureInt64Rules& Rules);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Int64")
	static void SecureInt64_Set(UPARAM(ref) FSecureInt64& Value, int64 NewValue);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Int64")
	static int64 SecureInt64_Get(UPARAM(ref) FSecureInt64& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Int64")
	static bool SecureInt64_Validate(UPARAM(ref) FSecureInt64& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Int64")
	static void SecureInt64_Rekey(UPARAM(ref) FSecureInt64& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Int64")
	static void SecureInt64_UpdateProtection(UPARAM(ref) FSecureInt64& Value, float DeltaSeconds);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Int64")
	static void SecureInt64_ResetToDefault(UPARAM(ref) FSecureInt64& Value);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Int64")
	static int32 SecureInt64_GetTamperCount(const FSecureInt64& Value);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Int64")
	static ESecureValueTamperReason SecureInt64_GetLastTamperReason(const FSecureInt64& Value);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Int64")
	static bool SecureInt64_IsInitialized(const FSecureInt64& Value);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Int64")
	static int64 SecureInt64_GetLastValidValue(const FSecureInt64& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Int64|Testing")
	static void SecureInt64_CorruptForTesting(UPARAM(ref) FSecureInt64& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Int64")
	static bool SecureInt64_ValidateRules(const FSecureInt64Rules& Rules, FString& OutError);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Int64")
	static void SecureInt64_InitializeFromInternalRules(UPARAM(ref) FSecureInt64& Value, int64 InitialValue);

	/* DOUBLE */

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Double")
	static void SecureDouble_Initialize(UPARAM(ref) FSecureDouble& Value, double InitialValue, const FSecureDoubleRules& Rules);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Double")
	static void SecureDouble_Set(UPARAM(ref) FSecureDouble& Value, double NewValue);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Double")
	static double SecureDouble_Get(UPARAM(ref) FSecureDouble& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Double")
	static bool SecureDouble_Validate(UPARAM(ref) FSecureDouble& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Double")
	static void SecureDouble_Rekey(UPARAM(ref) FSecureDouble& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Double")
	static void SecureDouble_UpdateProtection(UPARAM(ref) FSecureDouble& Value, float DeltaSeconds);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Double")
	static void SecureDouble_ResetToDefault(UPARAM(ref) FSecureDouble& Value);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Double")
	static int32 SecureDouble_GetTamperCount(const FSecureDouble& Value);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Double")
	static ESecureValueTamperReason SecureDouble_GetLastTamperReason(const FSecureDouble& Value);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Double")
	static bool SecureDouble_IsInitialized(const FSecureDouble& Value);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Double")
	static double SecureDouble_GetLastValidValue(const FSecureDouble& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Double|Testing")
	static void SecureDouble_CorruptForTesting(UPARAM(ref) FSecureDouble& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Double")
	static bool SecureDouble_ValidateRules(const FSecureDoubleRules& Rules, FString& OutError);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Double")
	static void SecureDouble_InitializeFromInternalRules(UPARAM(ref) FSecureDouble& Value, double InitialValue);

	/* BYTE */

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Byte")
	static void SecureByte_Initialize(UPARAM(ref) FSecureByte& Value, uint8 InitialValue, const FSecureByteRules& Rules);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Byte")
	static void SecureByte_Set(UPARAM(ref) FSecureByte& Value, uint8 NewValue);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Byte")
	static uint8 SecureByte_Get(UPARAM(ref) FSecureByte& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Byte")
	static bool SecureByte_Validate(UPARAM(ref) FSecureByte& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Byte")
	static void SecureByte_Rekey(UPARAM(ref) FSecureByte& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Byte")
	static void SecureByte_UpdateProtection(UPARAM(ref) FSecureByte& Value, float DeltaSeconds);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Byte")
	static void SecureByte_ResetToDefault(UPARAM(ref) FSecureByte& Value);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Byte")
	static int32 SecureByte_GetTamperCount(const FSecureByte& Value);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Byte")
	static ESecureValueTamperReason SecureByte_GetLastTamperReason(const FSecureByte& Value);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Byte")
	static bool SecureByte_IsInitialized(const FSecureByte& Value);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Byte")
	static uint8 SecureByte_GetLastValidValue(const FSecureByte& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Byte|Testing")
	static void SecureByte_CorruptForTesting(UPARAM(ref) FSecureByte& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Byte")
	static bool SecureByte_ValidateRules(const FSecureByteRules& Rules, FString& OutError);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Byte")
	static void SecureByte_InitializeFromInternalRules(UPARAM(ref) FSecureByte& Value, uint8 InitialValue);

	/* VECTOR */
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Vector") static void SecureVector_Initialize(UPARAM(ref) FSecureVector& Value, const FVector& InitialValue, const FSecureVectorRules& Rules);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Vector") static void SecureVector_Set(UPARAM(ref) FSecureVector& Value, const FVector& NewValue);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Vector") static FVector SecureVector_Get(UPARAM(ref) FSecureVector& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Vector") static bool SecureVector_Validate(UPARAM(ref) FSecureVector& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Vector") static void SecureVector_Rekey(UPARAM(ref) FSecureVector& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Vector") static void SecureVector_UpdateProtection(UPARAM(ref) FSecureVector& Value, float DeltaSeconds);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Vector") static void SecureVector_ResetToDefault(UPARAM(ref) FSecureVector& Value);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Vector") static int32 SecureVector_GetTamperCount(const FSecureVector& Value);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Vector") static ESecureValueTamperReason SecureVector_GetLastTamperReason(const FSecureVector& Value);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Vector") static bool SecureVector_IsInitialized(const FSecureVector& Value);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Vector") static FVector SecureVector_GetLastValidValue(const FSecureVector& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Vector|Testing") static void SecureVector_CorruptForTesting(UPARAM(ref) FSecureVector& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Vector") static bool SecureVector_ValidateRules(const FSecureVectorRules& Rules, FString& OutError);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Vector") static void SecureVector_InitializeFromInternalRules(UPARAM(ref) FSecureVector& Value, const FVector& InitialValue);

	/* ROTATOR */
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Rotator") static void SecureRotator_Initialize(UPARAM(ref) FSecureRotator& Value, const FRotator& InitialValue, const FSecureVectorRules& Rules);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Rotator") static void SecureRotator_Set(UPARAM(ref) FSecureRotator& Value, const FRotator& NewValue);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Rotator") static FRotator SecureRotator_Get(UPARAM(ref) FSecureRotator& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Rotator") static bool SecureRotator_Validate(UPARAM(ref) FSecureRotator& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Rotator") static void SecureRotator_Rekey(UPARAM(ref) FSecureRotator& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Rotator") static void SecureRotator_UpdateProtection(UPARAM(ref) FSecureRotator& Value, float DeltaSeconds);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Rotator") static void SecureRotator_ResetToDefault(UPARAM(ref) FSecureRotator& Value);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Rotator") static int32 SecureRotator_GetTamperCount(const FSecureRotator& Value);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Rotator") static ESecureValueTamperReason SecureRotator_GetLastTamperReason(const FSecureRotator& Value);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Rotator") static bool SecureRotator_IsInitialized(const FSecureRotator& Value);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Rotator") static FRotator SecureRotator_GetLastValidValue(const FSecureRotator& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Rotator|Testing") static void SecureRotator_CorruptForTesting(UPARAM(ref) FSecureRotator& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Rotator") static bool SecureRotator_ValidateRules(const FSecureVectorRules& Rules, FString& OutError);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Rotator") static void SecureRotator_InitializeFromInternalRules(UPARAM(ref) FSecureRotator& Value, const FRotator& InitialValue);

	/* TRANSFORM */
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Transform") static void SecureTransform_Initialize(UPARAM(ref) FSecureTransform& Value, const FTransform& InitialValue, const FSecureTransformRules& Rules);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Transform") static void SecureTransform_Set(UPARAM(ref) FSecureTransform& Value, const FTransform& NewValue);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Transform") static FTransform SecureTransform_Get(UPARAM(ref) FSecureTransform& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Transform") static bool SecureTransform_Validate(UPARAM(ref) FSecureTransform& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Transform") static void SecureTransform_Rekey(UPARAM(ref) FSecureTransform& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Transform") static void SecureTransform_UpdateProtection(UPARAM(ref) FSecureTransform& Value, float DeltaSeconds);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Transform") static void SecureTransform_ResetToDefault(UPARAM(ref) FSecureTransform& Value);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Transform") static int32 SecureTransform_GetTamperCount(const FSecureTransform& Value);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Transform") static ESecureValueTamperReason SecureTransform_GetLastTamperReason(const FSecureTransform& Value);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Transform") static bool SecureTransform_IsInitialized(const FSecureTransform& Value);
	UFUNCTION(BlueprintPure, Category = "ShieldUE|Transform") static FTransform SecureTransform_GetLastValidValue(const FSecureTransform& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Transform|Testing") static void SecureTransform_CorruptForTesting(UPARAM(ref) FSecureTransform& Value);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Transform") static bool SecureTransform_ValidateRules(const FSecureTransformRules& Rules, FString& OutError);
	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Transform") static void SecureTransform_InitializeFromInternalRules(UPARAM(ref) FSecureTransform& Value, const FTransform& InitialValue);
};
