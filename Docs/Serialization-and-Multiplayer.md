# Serialization and Multiplayer Guidance

## Save games

Protected values should not automatically be treated as tamper-proof save data. A packaged client contains the code required to decode its own values, and a save format may expose enough information for a determined attacker to study it.

Before relying on ShieldUE for save games, decide whether the project needs:

- Logical-value serialization followed by fresh re-encoding on load
- Protected-payload serialization for continuity
- A separate save-file integrity or signing system
- Server-side validation for cloud saves

This behavior is not yet standardized by ShieldUE.

## Replication

ShieldUE does not make replicated client data trustworthy. Clients can inspect their own memory and may observe values during normal gameplay.

For multiplayer games:

- The server must remain authoritative for currency, score, inventory, progression, and rankings.
- Validate important client requests on the server.
- Use ShieldUE as a local integrity layer or a signal for suspicious behavior.
- Do not accept a client-reported protected value as proof of honesty.

## Data assets and replays

Projects should test how their chosen serialization system handles protected structs before shipping. Pay particular attention to copied values, version changes, replay compatibility, and migrations between plugin versions.
