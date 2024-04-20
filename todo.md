**Input exemple:**
`(> out_1 "ls" > out_2 '-l' < 'in' -a | grep "keyword" | wc -l) && echo success || echo ko`

The input is tokenized and categorized

| Token     | Type      | Quote   |
|-----------|-----------|---------|
| (         | PARENTESHIS_OPEN | NONE    |
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
| )         | PARENTESHIS_OPEN | NONE    |
| &&        | AND      | NONE    |
| echo        | WORD      | NONE    |
| success        | WORD      | NONE    |
| &#124;&#124;        | OR      | NONE    |
| echo        | WORD      | NONE    |
| ko        | WORD      | NONE    |


| Token     | Type      | Quote   |
|-----------|-----------|---------|
| (> out_1 "ls" > out_2 '-l' < 'in' -a &#124; grep "keyword" &#124; wc -l) | PARENTHESIS | NONE
| &&        | AND      | NONE    |
| echo        | WORD      | NONE    |
| success        | WORD      | NONE    |
| &#124;&#124;        | OR      | NONE    |
| echo        | WORD      | NONE    |
| ko        | WORD      | NONE    |


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



invalids inputs
(ls | (grep i | grep s ))
