# simplexModus
x86_64 UEFI operating system (kernel) using [limine](https://github.com/limine-bootloader/limine).

simplexModus is a 64-bit operating system that gives the user full control over their device. \
there are two modes, **core** and **safe**. core mode operates in ring 0 and requires a password confirmation to enter, and safe mode operates in ring 3, which is the default.

see `LICENSE` for license information \
see `docs` for documentation, including contribution guidelines and a build guide

> ![NOTE]
> simplexModus will **NOT** boot on BIOS systems. simplexModus is designed for x86 64-bit UEFI systems.
> This means that it will boot on most modern systems, but not on systems from ~2006 or before.
