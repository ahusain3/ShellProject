Name:Ahsen Husain
ID:0895117
        Explaination of Algorithm
The program has a main loop in which 3 important tasks are executed.
1) getln is used to get input and parse it
2) a for loop is used to count the number of arguments
3) an execute function is used to execute the command

The execute function
The execute function contains several if statements. 
The if statements compare the first argument arg[0] to the names
of possible commands. For example "add" or "exit"
The if statements also check for '<' and '>' for redirection of input
and output.
There is one if statement that checks if the final argument is '&'
in which case the command is run in the background.
If the command makes it through all if statements the command
is run as a linux command in the foreground.

Every if statement contains a function to do the required work.
For example the add if statement will call the add function.

        Assumptions
Commands will be spelt correctly.
For example, to exit "exit" must be typed in.
commands such as "exi", "ex" or "quit" will result in an invalid
command error message and you will be prompted again.

Background commands will have one '&' character any additional '&'
will result in an error message and a reprompt.


        The way to run the program
use the command
make
use the command
./msh

The command backwards has been added
Example: backwards GO 1 2 3
Details: display arguments in reverse order
Output:
3
2
1
GO