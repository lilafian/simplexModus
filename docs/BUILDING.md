# Build guide

## Requirements
### Compilation
 - GCC
 - GNU make
### Virtualization
 - QEMU (or another virtualization software, see below)
 - OVMF

## Compile and build

Ensure you run `git submodule update` at least once to ensure Limine works.

To build, run the following commands (they can be chained together using `&&` just fine):

```bash
make kernel
make userexp
make build
```

 - `make kernel` - compiles the kernel (src/kernel)
 - `make userexp` - compiles the user experience (src/user)
 - `make build` - creates the bootable ISO file

## Run in a virtual machine

> Install OVMF from your distribution's package manager or another source first, or simplexModus may not run correctly or run at all.

To run in a VM, run the following command:

```bash
make run
```

> [!NOTE]
> If you get an error that OVMF was not found, make sure you install it and/or locate the files (likely in /usr/share/ovmf) and replace the paths in `GNUmakefile`. If this does not work, open an issue.

## Use another VM software

To use another VM software, edit the GNUmakefile's `run` target (near the bottom) and use that software's CLI command. You can also manually insert the ISO into any GUI software, like VirtualBox.

## Debugging

> Install GDB if you do not already have it before debugging.

To debug in QEMU, run the following command:

```bash
make rundbg
```

Then, you can use GDB to connect and debug using:

```bash
gdb bin/spxMdsKernel

(gdb) target remote :1234
(gdb) hb kernMain
```

You can switch `kernMain` to any function or memory address in the kernel.
