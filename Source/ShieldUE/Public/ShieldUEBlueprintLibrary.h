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
};
