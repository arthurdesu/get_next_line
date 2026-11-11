*This project has been created as part of the 42 curriculum by mamehri.*

# get_next_line

Reading a file descriptor, one line at a time.

## Description

### Goal

`get_next_line` is a 42 project that consists in writing a single C function:

```c
char *get_next_line(int fd);
```

Each call returns the next line available on the file descriptor `fd`. Called in a loop, it reads a whole file line by line. The project is an introduction to:

- file descriptors and the `read()` system call;
- `static` variables, which keep state between two calls of a function;
- dynamic memory management (`malloc` / `free`) and leak-free error handling;
- compile-time configuration: the size of the chunks given to `read()` is set with the `BUFFER_SIZE` macro and must never change the result.

### Overview

| Return value | Meaning |
|---|---|
| a `char *` | The next line, allocated with `malloc`, including its terminating `\n`. The last line of a file that does not end with a newline is returned without one. The caller must `free` it. |
| `NULL` | There is nothing left to read, or an error occurred (invalid `fd`, `read()` failure, `malloc` failure). |

## Instructions

### Files

| File | Content |
|---|---|
| `get_next_line.h` | Prototype of `get_next_line()`, default value of `BUFFER_SIZE` and the required includes. |
| `get_next_line.c` | `get_next_line()` and the functions that fill the stash, extract a line and trim the stash. |
| `get_next_line_utils.c` | String helpers (length, join, ...) and some other small helper functions. |

### Compilation

The function is not a program by itself: the sources are compiled together with a test program such as the `main.c` shown below.

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl
```

`BUFFER_SIZE` can be any positive integer. If the flag is omitted, the default value defined in `get_next_line.h` is used.

### Execution

Example of `main.c`: it reads the file given as argument (or the standard input if there is none) and prints it line by line.

```c
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "get_next_line.h"

int	main(int argc, char **argv)
{
	int		fd;
	char	*line;

	fd = 0;
	if (argc > 1)
		fd = open(argv[1], O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	if (fd > 0)
		close(fd);
	return (0);
}
```

```bash
./gnl file.txt          # read a file
./gnl < file.txt        # read the standard input (redirection)
echo "hello" | ./gnl    # read from a pipe
```

## Algorithm

### Principle: a static stash fed by chunked reads

`read()` is destructive: bytes that have been read cannot be pushed back into the file descriptor. Moreover, `read()` returns up to `BUFFER_SIZE` bytes without caring where the lines end, so a single call can return the end of the current line followed by the beginning of the next one. This surplus has to survive until the next call of `get_next_line()`.

The solution is a **stash**: a `static` pointer to a heap-allocated string that holds every byte already read from the file descriptor but not yet returned to the caller. A static variable lives for the whole execution of the program, so the stash keeps its content from one call to the next, while remaining invisible to the rest of the program.

Each call goes through five steps:

1. **Check the arguments.** If `fd < 0` or `BUFFER_SIZE <= 0`, return `NULL` immediately.
2. **Fill the stash.** As long as the stash does not contain a `\n`, call `read()` to get up to `BUFFER_SIZE` bytes into a heap-allocated buffer, terminate it with `\0` and append it to the stash. The loop stops as soon as the stash contains a `\n`, at the end of the file (`read()` returns `0`) or on error (`read()` returns `-1`). If the stash already holds a complete line, `read()` is not called at all.
3. **Extract the line.** If the stash is `NULL` or empty, free it and return `NULL`. Otherwise copy into a new string everything from the beginning of the stash up to the first `\n` included (or the whole stash if the end of the file was reached without a newline).
4. **Trim the stash.** Replace the stash by a copy of what follows the extracted line and free the old one. If nothing follows, free the stash and set it to `NULL`.
5. **Return the line.** It now belongs to the caller.

### Worked example

File content `ab\ncd\nef` (no trailing newline) with `BUFFER_SIZE=4`:

| Call | Stash before | `read()` returns | Stash after filling | Line returned | Stash after trimming |
|---|---|---|---|---|---|
| 1 | *(empty)* | `ab\nc` | `ab\nc` | `ab\n` | `c` |
| 2 | `c` | `d\nef` | `cd\nef` | `cd\n` | `ef` |
| 3 | `ef` | `0` (end of file) | `ef` | `ef` | *(freed, `NULL`)* |
| 4 | *(`NULL`)* | `0` (end of file) | *(`NULL`)* | `NULL` | *(`NULL`)* |

## Bonus
 
The bonus part makes `get_next_line()` read several file descriptors alternately (for example `fd` 3, then 4, then 3 again) without losing the reading position of each one and without mixing their lines.
 
The algorithm stays the same. Only the stash changes: the `static` pointer to the stash becomes a single `static` pointer to a linked list with one node per file descriptor, each node holding its `fd` and its own stash, so the five steps above are applied only to the stash of the requested descriptor.
 
The bonus files are `get_next_line_bonus.h`, `get_next_line_bonus.c` and `get_next_line_utils_bonus.c`. They are compiled like the mandatory ones, with a test program that alternates between several descriptors:
 
```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main.c -o gnl_bonus
```

## Resources

### References

**Documentation**

- The 42 `get_next_line` subject.
- `man 2 read`: https://man7.org/linux/man-pages/man2/read.2.html
- `man 3 malloc` (`malloc`, `free`): https://man7.org/linux/man-pages/man3/malloc.3.html

**Articles**

- Code (quoi), *File descriptor*: https://www.codequoi.com/en/handling-a-file-by-its-descriptor-in-c/

### Use of AI

| Task | Part of the project | How AI was used |
|---|---|---|
| Understanding the concepts | Behaviour of `read()`, file descriptors, `static` storage, buffer versus stash | Asked for explanations and examples, then traced the behaviour of `read()`, the buffer and the static stash by hand on a small multi-line example to check that I had understood. |
| Testing | Edge cases | Suggestions of edge cases to test (empty file, no trailing newline, `BUFFER_SIZE` of 1 and very large, invalid `fd`). |
| Documentation | This README | Drafted the structure and the wording with AI, then reviewed and corrected the content against my code. |

The source files (`get_next_line.h`, `get_next_line.c`, `get_next_line_utils.c`, `get_next_line_bonus.h`, `get_next_line_bonus.c`, `get_next_line_utils_bonus.c`) were written by me: AI was used to understand the problem, not to produce the code.