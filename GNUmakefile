MAKEFLAGS += -rR
.SUFFIXES:

override KERNELOUTPUTFILENAME := spxMdsKernel

CC := cc

CFLAGS := -g -O2 -pipe

CPPFLAGS :=

NASMFLAGS := -F dwarf -g

LDFLAGS :=

FONT_FILE := assets/fonts/zap-ext-vga16.psf
STARTUP_FILE := startup.nsh

override CC_IS_CLANG := $(shell ! $(CC) -- version 2>/dev/null | grep 'clang' >/dev/null 2>&1; echo $$?)

ifeq ($(CC_IS_CLANG),1)
	override CC += \
		-target x86_64-unknown-none
endif

# dont change!
override CFLAGS += \
    -Wall \
    -Wextra \
    -std=gnu11 \
    -ffreestanding \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-PIC \
    -m64 \
    -march=x86-64 \
    -mno-80387 \
    -mno-mmx \
    -mno-sse \
    -mno-sse2 \
    -mno-red-zone \
    -mcmodel=kernel

# dont change!
override NASMFLAGS += \
    -Wall \
    -f elf64

# dont change!
override LDFLAGS += \
    -Wl,-m,elf_x86_64 \
    -Wl,--build-id=none \
    -nostdlib \
    -static \
    -z max-page-size=0x1000 \
    -T linker.ld

# Use "find" to glob all *.c, *.S, and *.asm files in the tree and obtain the
# object and header dependency file names.
#  - osdev wiki
override SRCFILES := $(shell cd src && find -L * -type f | LC_ALL=C sort)
override CFILES := $(filter %.c,$(SRCFILES))
override ASFILES := $(filter %.S,$(SRCFILES))
override NASMFILES := $(filter %.asm,$(SRCFILES))
override OBJ := $(addprefix obj/,$(CFILES:.c=.c.o) $(ASFILES:.S=.S.o) $(NASMFILES:.asm=.asm.o)) obj/zap_vga.o
override HEADER_DEPS := $(addprefix obj/,$(CFILES:.c=.c.d) $(ASFILES:.S=.S.d))

.PHONY: all
all: bin/$(KERNELOUTPUTFILENAME)

-include $(HEADER_DEPS)

# Link rules for the final executable.
bin/$(KERNELOUTPUTFILENAME): GNUmakefile linker.ld $(OBJ)

	mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $@

# Compilation rules for *.c files.
obj/%.c.o: src/%.c GNUmakefile
	mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Compilation rules for *.S files.
obj/%.S.o: src/%.S GNUmakefile
	mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Compilation rules for *.asm (nasm) files.
obj/%.asm.o: src/%.asm GNUmakefile
	mkdir -p "$$(dirname $@)"
	nasm $(NASMFLAGS) $< -o $@

# Rule for creating the font object file
obj/zap_vga.o: $(FONT_FILE)
	mkdir -p "$$(dirname $@)"
	objcopy -O elf64-x86-64 -B i386 -I binary $< $@

# Remove object files and the final executable.
.PHONY: clean
clean:
	rm -rf bin obj iso_root

# Build ISO image
.PHONY: build
build:
	# Build "limine" utility.
	make -C limine

	# Create a directory which will be our ISO root.
	mkdir -p iso_root

	# Copy the relevant files over.
	mkdir -p iso_root/boot
	cp -v bin/spxMdsKernel iso_root/boot/
	mkdir -p iso_root/boot/limine
	cp -v limine.conf limine/limine-bios.sys limine/limine-bios-cd.bin \
	      limine/limine-uefi-cd.bin iso_root/boot/limine/

	# Create the EFI boot tree and copy Limine's EFI executables over.
	mkdir -p iso_root/EFI/BOOT
	cp -v limine/BOOTX64.EFI iso_root/EFI/BOOT/
	cp -v limine/BOOTIA32.EFI iso_root/EFI/BOOT/

	# Create the bootable ISO.
	xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
	        -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
	        -apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
	        -efi-boot-part --efi-boot-image --protective-msdos-label \
	        iso_root -o simplexModus.iso

	# Install Limine stage 1 and 2 for legacy BIOS boot.
	./limine/limine bios-install simplexModus.iso

.PHONY: run
run:
	@echo "!=============================================================!"
	@echo "If your password is prompted, it is for booting OVMF with sudo!"
	@echo "!=============================================================!"
	sudo qemu-system-x86_64 \
		-machine pc \
		-enable-kvm \
		-drive file=simplexModus.iso,media=cdrom,if=none,id=spxmds \
		-m 256M \
		-drive if=pflash,format=raw,readonly=on,file=/usr/share/ovmf/x64/OVMF_CODE.4m.fd \
		-drive if=pflash,format=raw,file=/usr/share/ovmf/x64/OVMF_VARS.4m.fd \
		-device ide-cd,drive=spxmds,bootindex=0

.PHONY: rundbg
rundbg:
	@echo "!=============================================================!"
	@echo "If your password is prompted, it is for booting OVMF with sudo!"
	@echo "!=============================================================!"
	sudo qemu-system-x86_64 \
		-s -S \
		-machine pc \
		-enable-kvm \
		-drive file=simplexModus.iso,media=cdrom,if=none,id=spxmds \
		-m 256M \
		-drive if=pflash,format=raw,readonly=on,file=/usr/share/ovmf/x64/OVMF_CODE.4m.fd \
		-drive if=pflash,format=raw,file=/usr/share/ovmf/x64/OVMF_VARS.4m.fd \
		-device ide-cd,drive=spxmds,bootindex=0

