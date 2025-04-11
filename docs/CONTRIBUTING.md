# Contribution guide
All changes are done through standard GitHub pull requests. Ensure your code fulfills the following requirements:

* Readable and understandable
* Purpose for each file and function is clear
* Each file contains a valid license declaration (you can check which files do not using `make checklicense`)
* Mostly professional
* Follows all naming conventions

# Naming conventions
| Type             | Convention            |
|------------------|-----------------------|
| Files            | `camelCase`           |
| Directories      | `camelCase`           |
| Identifiers      | `snake_case`          |
| Functions        | `snake_case`          |
| Struct functions | `struct_functionName` |
| Constants        | `CAPS_SNAKE_CASE`     |

The star operator in a pointer goes **BEFORE** the space between the type and identifier. For example,
```c
int* my_pointer; // correct
int *my_pointer; // incorrect
```

# License declaration
The simplexModus standard license declaration has the path of the file (relative to simplexModus/), all modification years, author, the path of the LICENSE file, and a short description of the file. \
If you believe you have contributed a significant amount of a file, you may add yourself to the list of authors of a file. Any marked name of an author is never removed from the file. Author list changes can be approved or denied at the discretion of the maintainer.

**Example**
```c
/*
 * simplexModus/src/kernel/main.c
 * copyright (c) 2025 lilaf, MIT license.
 * view more license information by viewing simplexModus/LICENSE.
 *
 * simplexModus kernel initialization and entry point
 */
```
