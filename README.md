# 42-Minishell - As beautiful as a shell

**Overview:**
Minishell is a command-line interface (CLI) implementation, enabling users to interact with an operating system through entered commands.
Developed as part of the 42 curriculum, this project strives to deliver a minimalistic yet functional shell environment written in the C programming language.

Emphasizing a seamless experience, Minishell incorporates fundamental built-in commands and advanced features, including signal handling,
management of complex command sequences, and support for pipelines. By adhering to POSIX standards, Minishell ensures compatibility and consistency across Unix-like operating systems.

**Parser**
The parser function serves as the core of the Minishell project, acting as a pivotal component in handling user input.
Its primary responsibilities encompass tokenizing input, categorizing tokens, expanding environment variables, and
organizing input/output redirections (such as "<" for input, ">" for output, ">>" for append, and "<<" for heredoc). 
The parser excels in identifying commands, organizing them, and, for non-built-in commands, determining the execution path.
In essence, it plays a central role in processing and structuring the user's input for seamless execution within the Minishell environment.

How it works:

```"ls" > out_target '-l' > out_target2 < 'in_target' -a | wc -l | cat -e```
