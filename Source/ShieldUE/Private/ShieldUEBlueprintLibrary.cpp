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

void UShieldUEBlueprintLibrary::SecureBool_InitializeFromInternalRules(FSecureBool& Value, bool InitialValue)
{
	Value.Initialize(InitialValue, Value.GetRules());
}