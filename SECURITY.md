# ShieldUE Security Policy

## Scope

ShieldUE is a runtime gameplay-integrity and tamper-detection plugin for Unreal Engine. It is designed to make naïve memory edits detectable and recoverable for values such as health, currency, score, ammunition, and progression.

ShieldUE is not a complete anti-cheat product, cryptographic storage system, or replacement for server-authoritative multiplayer architecture.

## Reporting a vulnerability

Please do not publish an exploit or sensitive security report in a public issue.

Use GitHub's private vulnerability reporting feature when it is enabled for this repository. If it is unavailable, contact the developer through [iamsohel.xyz](https://iamsohel.xyz) and include:

- A clear description of the issue
- Affected engine and plugin versions
- Reproduction steps or a minimal project
- The expected and observed behavior
- Any suggested mitigation

Please allow reasonable time for investigation before public disclosure.

## Security boundaries

- Client-side values can ultimately be inspected by a determined attacker.
- A project secret embedded in a shipped client cannot be treated as permanently secret.
- ShieldUE does not make a client authoritative in a competitive multiplayer game.
- Server-side validation is required for rankings, purchases, currency, inventory, and progression that must be trusted.
- Decoded values may exist plainly in memory while the game is actively using them.

## Development testing

`CorruptForTesting()` and the Blueprint testing nodes intentionally modify protected state. They are provided for development and automated tests only and must not be used as shipping gameplay logic.
