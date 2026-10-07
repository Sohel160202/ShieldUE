# Changelog

All notable ShieldUE changes are documented here.

## Unreleased

### Added

- `FSecureInt64`, `FSecureDouble`, and `FSecureByte` value types.
- C++ and Blueprint APIs for the new numeric types.
- `FSecureVector`, `FSecureRotator`, and `FSecureTransform` structured value types.
- Aggregate integrity checks across structured value components.
- Blueprint accessors for initialization state and last valid values.
- Blueprint testing nodes for controlled corruption tests.
- Blueprint rule-validation nodes with human-readable errors.
- C++ rule validation for Float, Int32, and Bool values.
- Warnings for invalid ranges, invalid defaults, non-finite floats, invalid recovery modes, and invalid automatic re-key intervals.
- Security and contribution documentation.

### Clarified

- ShieldUE's client-side security boundaries.
- Save-game and multiplayer responsibilities.
- The difference between obfuscation, integrity validation, and authoritative security.

## 0.1.0

- Initial public ShieldUE plugin.
- Secure Float, Secure Int32, and Secure Bool types.
- Integrity checks, shadow verification, recovery modes, and runtime re-keying.
