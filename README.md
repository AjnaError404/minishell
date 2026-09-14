*This project has been created as part of the 42 curriculum by laaubry and ykandous.*

# Minishell

## Description
Minishell is a systems programming project that recreates a functional UNIX command-line interpreter, adhering closely to POSIX and standard Bash behaviors. The project emphasizes low-level systems programming in C: process synchronization, inter-process communication (IPC), file descriptor manipulation, abstract syntax tree (AST) evaluation, and signal handling.

### Key Features
* **Interactive Command Line:** Full terminal line editing and prompt history powered by the GNU `readline` library.
* **Command Execution:** Resolution of system binaries using the `PATH` environment variable, as well as absolute and relative path executions.
* **Pipelines (`|`):** Multi-stage piping supporting asynchronous execution and propagation of exit statuses (rightmost command priority).
* **I/O Redirections:** Influx (`<`), truncation (`>`), append (`>>`), and multi-heredoc (`<<`) handling.
* **In-Memory Heredoc:** Non-blocking heredoc implementation via anonymous UNIX pipes with instant signal responsiveness (`Ctrl-C` sets exit status `130`).
* **Variable & Quote Expansion:** Parameter expansion (`$VAR`, `$?`), single quote literal handling (`'...'`), and double quote variable expansions (`"..."`).
* **Signal Handling:** POSIX-compliant behavior for `Ctrl-C` (`SIGINT`), `Ctrl-\` (`SIGQUIT`), and `Ctrl-D` (`EOF`), adapting dynamically between idle prompt, running child processes, and heredocs.
* **Built-in Commands:** Native implementations of `echo` (with `-n`), `cd` (with `$HOME` fallback), `pwd`, `export`, `unset`, `env`, and `exit`.

---

## Architecture & Memory Model

The shell is engineered around three primary design paradigms:

1. **Finite State Automaton (FSM) Tokenizer:** 
   User input is parsed via a deterministic automaton ensuring robust handling of unquoted separators, sticky redirections (e.g., `echo foo>out`), and orphan tokens (`|`, `||`).
2. **Abstract Syntax Tree (AST):** 
   Tokens are converted into a binary execution tree where pipeline nodes separate subtrees, ensuring predictable file descriptor propagation and child branching.
3. **The `rumba` Memory Tracker:**
   To guarantee leak-free execution even on sudden errors or subshell interruptions, dynamic allocations are registered within a custom linked-list arena tracker (`t_rumba`). This allows deterministic, single-call memory deallocations (`del_all_rumba`) across fork exits.
4. **Strict POSIX Signals:**
   Only a single global variable (`volatile sig_atomic_t g_signal` or `int g_signal`) is utilized strictly to capture incoming asynchronous signals, in compliance with the 42 subject requirements.

---

## Instructions

### Prerequisites
* A UNIX-like environment (Linux / macOS / WSL).
* `clang` or `gcc` compiler.
* `make`.
* GNU Readline library (`libreadline-dev` on Debian/Ubuntu).

### Compilation
Compile the executable using the root Makefile:
```bash
git clone <repository_url>
cd minishell
make

```

Available Makefile rules:

* `make` or `make all`: Compiles the `minishell` executable.
* `make clean`: Removes intermediate object files (`*.o`).
* `make fclean`: Cleans objects and removes the compiled binary.
* `make re`: Performs a clean recompilation.

### Execution

Run the shell interactively:

```bash
./minishell

```

To run within an empty environment (evaluating resilience against uninitialized variables):

```bash
env -i ./minishell

```

---

## Debugging & Valgrind Profiling

Minishell is fully stabilized against memory leaks, file descriptor leaks, and memory corruption across parent and child processes.

### Memory Leaks & Child Tracing

Because pipelines and heredocs execute inside child processes via `fork()`, standard Valgrind invocations miss child leaks. The shell should be checked using `--trace-children=yes`:

```bash
valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --track-fds=yes \
         --trace-children=yes \
         --suppressions=readline.supp \
         ./minishell

```

### GNU Readline Suppressions (`readline.supp`)

The GNU Readline library allocates internal terminal capability tables and history state buffers that are intended to persist for the lifetime of the process. In accordance with the 42 evaluation rubric, these internal reachable allocations are classified as non-leaks and can be filtered cleanly using the included `readline.supp` file.

---

## Tests

Here is a curated suite of edge cases designed to test parsing, synchronization, and resource cleanup:

### 1. Nested Builtin Pipelines

Testing subshell termination, memory release, and rightmost exit status propagation without crashing the parent shell:

```bash
minishell$ exit 12 | exit 24
exit
minishell$echo$?
24

```

### 2. Heredoc Signal Interruption

Cancelling heredoc line intake must cleanly terminate the child reader without orphan file descriptors:

```bash
minishell$ cat << EOF
> ^C
minishell$echo$?
130

```

### 3. Asynchronous Pipe Saturation (SIGPIPE)

Verifying proper handling when consumer processes close read ends before producer processes complete:

```bash
minishell$ cat /dev/urandom | head -c 1000 | wc -c
1000

```

### 4. Failed Redirection Within Pipeline

Ensuring that permission or path failures abort only the affected stage while setting exit code `1`:

```bash
minishell$ cat < non_existent_file | ls
minishell: non_existent_file: No such file or directory
...
minishell$echo$?
0

```

### 5. Sticky Redirections and Multiple Chaining

```bash
minishell$ echo hello>f1>f2>f3
minishell$ cat f3
hello
minishell$ rm f1 f2 f3

```

---

## Resources

**Systems Programming & Inter-Process Communication**

* [Codequoi: Créer et tuer des processus fils en C (fork, wait)](https://www.codequoi.com/creer-et-tuer-des-processus-fils-en-c/)
* [Codequoi: Pipe, une méthode de communication inter-processus](https://www.codequoi.com/pipe-une-methode-de-communication-inter-processus/)
* [Codequoi: Redirections et descripteurs de fichiers (open, dup2)](https://www.codequoi.com/manipuler-un-fichier-a-laide-de-son-descripteur-en-c/)
* [Codequoi: Signaux POSIX (signal, sigaction)](https://www.codequoi.com/envoyer-et-intercepter-un-signal-en-c/)

**Architecture & Theory**

* [Writing Your Own Shell (Purdue University)](https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf)
* [GNU Bash Reference Manual](https://www.google.com/search?q=https://www.gnu.org/software/bash/manual/)

---

### Use of Artificial Intelligence (AI)

In the spirit of transparency, Generative AI (LLM) was utilized during the development of this project as an interactive tutor and debugging assistant. Specifically, AI was used for:

1. **Debugging & Memory Management:** Analyzing Valgrind logs to identify complex segmentation faults and memory leaks, diagnosing deep child leak traces during pipeline forks and analyzing file descriptor inheritance.
2. **Edge-Case Hardening:** Identifying race conditions and signal handler subtleties across interactive heredoc execution.
3. **Documentation:** This `README.md` file was structured, filtered, and drafted with the help of AI based on a raw list of resources and requirements provided by the authors.



