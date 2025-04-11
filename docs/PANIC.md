# simplexModus standard panic codes
### Panic types
The following types are provided as macros in `src/panic/panic.h` with `_MIN` and `_MAX` variants. Use these macros as offsets, do not hard-code panic codes.
| Code range          | Type name/macro              | Available codes |
|---------------------|------------------------------|-----------------|
| `0x01`-`0xEF`       | PANIC_KERNEL_INIT            | 239             |
| `0xF0`-`0xEFF`      | PANIC_KERNEL_MEMORY          | 3,599           |
| `0xF00`-`0x19FF`    | PANIC_KERNEL_FS              | 2,815           |
| `0x1A00`-`0x28FF`   | PANIC_KERNEL_DEVICES         | 3,839           |
| `0x2900`-`0x29FF`   | PANIC_KERNEL_DEVICE_SPECIFIC | 255             |
| `0x2A00`-`0x2CFF`   | PANIC_KERNEL_SECURITY        | 767             |
| `0x2D00`-`0x3CFF`   | PANIC_KERNEL_MISC            | 4,095           |
| `0x3D00`-`0xEFFF`   | PANIC_KERNEL_FUTURE_RESERVED | 45,823          |
| `0xF000`-`0xFFFF`   | PANIC_PROGRAM_SPECIFIC       | 4,095           |
| `0x10000`-`0x10EFF` | PANIC_CUSTOM_KERNEL          | 3,840           |

> [!WARNING]
> Do not use `PANIC_KERNEL_FUTURE_RESERVED` (`_MIN` or `_MAX`) as an error code offset. `PANIC_KERNEL_FUTURE_RESERVED` is subject to change.

> [!NOTE]
> `PANIC_CUSTOM_KERNEL` is used for modified versions of the simplexModus kernel and their custom panic error codes.

### Panic codes
| Code   | Description                    | Type macro + offset              |
|--------|--------------------------------|----------------------------------|
| `0x01` | MEMORY MAP LOAD FAILED         | PANIC_KERNEL_INIT_MIN + 0x0      |
| `0x02` | HHDM OFFSET LOAD FAILED        | PANIC_KERNEL_INIT_MIN + 0x1      |
| `0xF0` | NO USABLE MEMORY IN MEMORY MAP | PANIC_KERNEL_MEMORY_MIN + 0x0    |
| `0xF1` | OUT OF MEMORY                  | PANIC_KERNEL_MEMORY_MIN + 0x1    |
| `0xF2` | PAGE MAP LEVEL 4 IS NULL       | PANIC_KERNEL_MEMORY_MIN + 0x2    |
