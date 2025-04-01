# simplexModus
x86-64 operating system (kernel) using [limine](https://github.com/limine-bootloader/limine)

needs gcc and make for compilation
needs qemu and ovmf for virtualization

to build and run:

```bash
git submodule init
make
make build
make run
```

and it should work :)

> [!NOTE]
> if you get an error that ovmf was not found, make sure you install it and/or locate the files (likely in /usr/share/ovmf) and replace the paths in `GNUmakefile` at the bottom. if this does not work, open an issue.
