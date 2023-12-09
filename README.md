# 42-Minishell

### Overview 

Minishell is a command-line interface (CLI) implementation, enabling users to interact with an operating system through entered commands.
Developed as part of the 42 curriculum, this project strives to deliver a minimalistic yet functional shell environment written in the C programming language.

Emphasizing a seamless experience, Minishell incorporates fundamental built-in commands and advanced features, including signal handling,
management of complex command sequences, and support for pipelines. By adhering to POSIX standards, Minishell ensures compatibility and consistency across Unix-like operating systems.


### How it works

The parser function serves as the core of the Minishell project, acting as a pivotal component in handling user input.
Its primary responsibilities encompass tokenizing input, categorizing tokens, expanding environment variables, and
organizing input/output redirections (such as "<" for input, ">" for output, ">>" for append, and "<<" for heredoc). 
The parser excels in identifying commands, organizing them, and, for non-built-in commands, determining the execution path.
In essence, it plays a central role in processing and structuring the user's input for seamless execution within the Minishell environment.

**Input exemple:**
`> out_1 "ls" > out_2 '-l' < 'in' -a | grep "keyword" | wc -l`

The input is tokenized and categorized

| Token     | Type      | Quote   |
|-----------|-----------|---------|
| >         | REDIR_OUT | NONE    |
| out_1     | WORD      | NONE    |
| ls        | WORD      | D_QUOTE |
| >         | REDIR_OUT | NONE    |
| out_2     | WORD      | NONE    |
| -l        | WORD      | S_QUOTE |
| <         | REDIR_IN  | NONE    |
| in        | WORD      | S_QUOTE |
| -a        | WORD      | NONE    |
| &#124;    | PIPELINE  | NONE    |
| grep      | WORD      | NONE    |
| "keyword" | WORD      | D_QUOTE |
| &#124;    | PIPELINE  | NONE    |
| wc        | WORD      | NONE    |
| -l        | WORD      | NONE    |

Then a linkedlist is created where each node represents a stage in the command pipeline:
the node contains an array with the arguments, a linkedlist for input redirections ("<" or "<<")
and a linkedlist for output redirections (">", ">>").
# LinkedList Representation:

| Node | Commands            | Redir_In        | Redir_Out          | 
|------|---------------------|-----------------|--------------------|
| 1    | ["ls", "-l", "-a"]  | ["in"]          | ["out_1", "out_2"] |
| 2    | ["grep", "keyword"] | []              | []                 |
| 3    | ["wc", "-l"]        | []              | []                 |


Execution:
Following validation, the linked list proceeds to execution. The execution phase utilizes the `pipe()` and `fork()` functions to orchestrate the command executions.

| Process | Command             | Input             | Output             | Description      |
|---------|---------------------|-------------------|-------------------|-------------------|
| P1      | ls -l -a > out 2    |   NONE            | [pipe1_write_end] | Executes the node and writes to pipe1 |
| P2      | grep "keyword"      | [pipe1_read_end]  | [pipe2_write_end] | Reads from pipe1 executes the node, and writes to pipe2. |
| P3      | wc -l               | [pipe2_read_end]  |                   | Reads from pipe2 and executes the last node to the STDOUT. |

## Tests
Explore a set of test examples provided here for a comprehensive examination of the Minishell project.

First things first, execute `make` at the root of the repository.

### Simple commands

```bash
ls -la | sort -r
```
total 120
drwxrwxrwx 1 1000 root    512 Dec  8 22:48 .  <br>
drwxrwxrwx 1 1000 root    512 Dec  6 00:42 ..  <br>
-rwxrwxrwx 1 1000 root     74 Dec  8 22:12 file <br>
drwxrwxrwx 1 1000 root    512 Dec  8 22:12 .git <br>
drwxrwxrwx 1 1000 root    512 Dec  8 22:12 includes <br>
-rwxrwxrwx 1 1000 root   2678 Dec  8 22:15 Makefile <br>
-rwxrwxrwx 1 1000 root 114048 Dec  8 22:15 minishell <br>
-rwxrwxrwx 1 1000 root     83 Nov 25 17:11 readline_supression <br>
-rwxrwxrwx 1 1000 root   4006 Dec  8 22:56 README.md <br>
drwxrwxrwx 1 1000 root    512 Dec  8 22:15 srcs <br>
drwxrwxrwx 1 1000 root    512 Dec  1 12:38 .vscode <br>
-rwxrwxrwx 1 1000 root     81 Dec  8 22:24 wc
<br>
<br>

```bash
date
```
Fri Dec  8 10:56:18 PM WET 2023  
<br>


```bash
wc -w Makefile
```
278 Makefile
<br>
<br>

```bash
mkdir directory_test
```
Create a directory
<br>
<br>

```bash
rmdir directory_test
```
remove directory
<br>
<br>

```bash
touch file1 file2 file3
```
Create empty files file1, file2, and file3.
<br>
<br>

```bash
[ONLY WHITESPACES (tab and space)]
```
Do nothing
<br>
<br>


```bash
[TAB] ls
```
`includes`  Makefile  readline_supression  README.md  `srcs`
<br>
<br>

```bash
             	 	ls                      -la|cat -e
```
Yes! The `whitespaces` and pipeline without space are intentional. <br>
Does the command work? <br>
What if you replace the `spaces` by `tab's`? <br>
Still working? Great

Expected output:
total 12$
drwxrwxrwx 1 1000 root  512 Dec  8 23:01 .$ <br>
drwxrwxrwx 1 1000 root  512 Dec  6 00:42 ..$ <br> 
drwxrwxrwx 1 1000 root  512 Dec  8 23:01 .git$ <br>
drwxrwxrwx 1 1000 root  512 Dec  8 22:12 includes$ <br>
-rwxrwxrwx 1 1000 root 2678 Dec  8 22:15 Makefile$ <br>
-rwxrwxrwx 1 1000 root   83 Nov 25 17:11 readline_supression$ <br>
-rwxrwxrwx 1 1000 root 4622 Dec  8 23:33 README.md$ <br>
drwxrwxrwx 1 1000 root  512 Dec  8 23:01 srcs$ <br>
drwxrwxrwx 1 1000 root  512 Dec  1 12:38 .vscode$

<br>
<br>

### built-in commands

### Echo

```bash
echo
```
Nothing is printed

```bash
echo hello world!
```
hello world!

