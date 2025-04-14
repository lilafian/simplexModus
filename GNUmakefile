# simplexModus compilation, linking, and building rules.
# 
# 	!!!! This makefile is for GNU make on Linux. If you are using another operating system, edit the programs below. !!!!
#
# If you are here to change the OVMF paths, use Ctrl+F, grep, or a similar tool for "OVMF" in this file.

MAKEFLAGS += -rR
.SUFFIXES:

override KERNELOUTPUTFILENAME := smkernel
override USERSPACEOUTPUTFILENAME := smuexp

CC := cc

CFLAGS := -g -O2 -pipe

CPPFLAGS :=

NASMFLAGS := -F dwarf -g

LDFLAGS :=

FONT_FILE := assets/fonts/zap-ext-vga16.psf

override CC_IS_CLANG := $(shell ! $(CC) -- version 2>/dev/null | grep 'clang' >/dev/null 2>&1; echo $$?)

ifeq ($(CC_IS_CLANG),1)
	override CC += \
		-target x86_64-unknown-none
endif

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

override NASMFLAGS += \
    -Wall \
    -f elf64

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
override KSRCFILES := $(shell cd src/kernel && find -L . -type f | LC_ALL=C sort)
override KCFILES := $(filter %.c,$(KSRCFILES))
override KASFILES := $(filter %.S,$(KSRCFILES))
override KNASMFILES := $(filter %.asm,$(KSRCFILES))
override KOBJ := $(addprefix obj/kernel/,$(KCFILES:.c=.c.o) $(KASFILES:.S=.S.o) $(KNASMFILES:.asm=.asm.o)) obj/zap_vga.o
override KHEADER_DEPS := $(addprefix obj/kernel/,$(KCFILES:.c=.c.d) $(KASFILES:.S=.S.d))

override USRCFILES := $(shell cd src/user && find -L . -type f | LC_ALL=C sort)
override UCFILES := $(filter %.c,$(USRCFILES))
override UASFILES := $(filter %.S,$(USRCFILES))
override UNASMFILES := $(filter %.asm,$(USRCFILES))
override UOBJ := $(addprefix obj/user/,$(UCFILES:.c=.c.o) $(UASFILES:.S=.S.o) $(UNASMFILES:.asm=.asm.o))
override UHEADER_DEPS := $(addprefix obj/user/,$(UCFILES:.c=.c.d) $(UASFILES:.S=.S.d))


.PHONY: kernel
kernel: bin/$(KERNELOUTPUTFILENAME)

-include $(KHEADER_DEPS)

# link rules for the kernel
bin/$(KERNELOUTPUTFILENAME): GNUmakefile linker.ld $(KOBJ)

	mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) $(LDFLAGS) $(KOBJ) -o $@

.PHONY: userexp
userexp: bin/$(USERSPACEOUTPUTFILENAME)

-include $(UHEADER_DEPS)

# link rules for the userspace
bin/$(USERSPACEOUTPUTFILENAME): GNUmakefile linker.ld $(UOBJ)

	mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) $(LDFLAGS) $(UOBJ) -o $@


# compilation rules for C files
obj/%.c.o: src/%.c GNUmakefile
	mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# compilation rules for assembly .S files
obj/%.S.o: src/%.S GNUmakefile
	mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# compilation rules for assembly .asm files
obj/%.asm.o: src/%.asm GNUmakefile
	mkdir -p "$$(dirname $@)"
	nasm $(NASMFLAGS) $< -o $@

# rule for creating the default font binary
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
	rm -f simplexModus.img
	dd if=/dev/zero of=simplexModus.img bs=512 count=93750
	mkfs.fat -F 32 simplexModus.img

	mmd -i simplexModus.img ::/EFI
	mmd -i simplexModus.img ::/EFI/BOOT
	mmd -i simplexModus.img ::/system
	mmd -i simplexModus.img ::/boot
	mmd -i simplexModus.img ::/boot/limine

	mcopy -i simplexModus.img limine/BOOTX64.EFI ::/EFI/BOOT
	mcopy -i simplexModus.img bin/smkernel ::/system
	mcopy -i simplexModus.img bin/smuexp ::/system
	mcopy -i simplexModus.img limine.conf ::/boot/limine
	mcopy -i simplexModus.img startup.nsh ::

.PHONY: checklicense
checklicense:
	@files=$$(find ./src -type f \( -name "*.c" -o -name "*.h" \) ! -path "./src/kernel/boot/limine.h"); \
	missing=0; \
	for file in $$files; do \
		if ! awk 'NR <= 10 { block = block $$0 "\n" } END { \
			if (block !~ /\/\*\n \* simplexModus\/.*\n \* copyright \(c\) [0-9]{4} .*, MIT license\.\n \* view more license information by viewing simplexModus\/LICENSE\.\n \*\n \* .*\n \*\//) \
				exit 1; \
		}' "$$file"; then \
			echo "Missing or invalid license declaration at $$file"; \
			missing=1; \
		fi; \
	done; \
	if [ $$missing -eq 0 ]; then \
		echo "All files in src contain a valid license declaration!"; \
	fi

.PHONY: run
run:
	@echo "!================================================================!"
	@echo "! If your password is prompted, it is for booting OVMF with sudo !"
	@echo "!================================================================!"
	sudo qemu-system-x86_64 \
		-machine pc \
		-enable-kvm \
		-drive file=simplexModus.img \
		-m 256M \
		-drive if=pflash,format=raw,readonly=on,file=/usr/share/ovmf/x64/OVMF_CODE.4m.fd \
		-drive if=pflash,format=raw,file=/usr/share/ovmf/x64/OVMF_VARS.4m.fd \

.PHONY: rundbg
rundbg:
	@echo "!==========================================================================!"
	@echo "!      If your password is prompted, it is for booting OVMF with sudo      !"
	@echo "!==========================================================================!"
	@echo "! You are running simplexModus in DEBUG MODE! Use GDB to debug the kernel. !"
	@echo "!==========================================================================!"
	sudo qemu-system-x86_64 \
		-s -S \
		-machine pc \
		-enable-kvm \
		-drive file=simplexModus.img \
		-m 256M \
		-drive if=pflash,format=raw,readonly=on,file=/usr/share/ovmf/x64/OVMF_CODE.4m.fd \
		-drive if=pflash,format=raw,file=/usr/share/ovmf/x64/OVMF_VARS.4m.fd \

