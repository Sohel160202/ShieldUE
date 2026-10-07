# Rule Validation

ShieldUE validates rules when values are initialized or when `SetRules` is called.

## Float rules

The following are rejected:

- NaN values
- Infinite values
- A minimum greater than the maximum
- A default outside the configured range
- A non-positive automatic re-key interval when automatic re-keying is enabled
- An invalid recovery mode

## Integer rules

The following are rejected:

- A minimum greater than the maximum
- A default outside the configured range
- A non-positive automatic re-key interval when automatic re-keying is enabled
- An invalid recovery mode

## Boolean rules

Boolean rules validate the automatic re-key interval and recovery mode.

Invalid rules are retained so the caller can inspect or correct them, and a `LogShieldUE` warning is emitted. Projects should use the Blueprint validation nodes or the C++ `ValidateRules` functions before starting gameplay when rules are data-driven.
