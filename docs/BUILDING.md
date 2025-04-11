needs gcc and make for compilation
needs qemu and ovmf for virtualization

to build and run:

```bash
git submodule init
make kernel
make build
make run
```

> [!NOTE]
> if you get an error that ovmf was not found, make sure you install it and/or locate the files (likely in /usr/share/ovmf) and replace the paths in `GNUmakefile`. if this does not work, open an issue.
