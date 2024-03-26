
- o		THE ROAD TO NINJASHELL
- ✅		getting started
- ✅		lexer


### && Problem :
export var=hello && echo $var | wc -l
ist the same|
export var=hello; echo $var | echo $var
	no fork			fork		fork

if 1 command and not built in --> fork
if 1 command and built in --> no fork
if multiple commands --> fork

### first Expander and then parser!!!!!!!!!

- *		parser
			* redirects
			* prescedance && || |

-		executer
			* expander
			* redirects
			* Test for /bin/lss => no file fount
			  lss => cmd not found

-		built-ins:
		{
			echo
			{
				❌ Basic output functionality.
				❌ Handling of command-line arguments.
				❌ Interpretation of escape sequences (optional, depends on shell specifications).
			}
			cd ✅
			{
				✅	cd - Switch to the previous directory and update OLDPWD and PWD environment variables.
				✅	cd ~ Switch to the user's home directory (use the HOME environment variable).
				✅	cd ~/Documents Navigate to a directory relative to the home directory.
				✅	==> check exit_codes Ensure correct exit status on success/failure.
				✅ Must take the path from our environment. Use environmental variables for path resolution.
				❌ handeln cd $PATH Navigate using environment variable paths. // NEED expander
				✅ handeln cd ../Desktop : Support relative path navigation.
				✅ handeln "cd" 	=> cds into Home_Directory
			}
			pwd ✅
			{
				✅ Must take the path from our environment.  Ensure the output reflects the current directory from the environment, not just the working directory.
			}
			export
			{
				❌ export PATH=$PATH:/Users/aalatzas/Desktop/mein_git/minishell  check with which minishell or minishell from other directory
				✅ Must take the path from our environment. Properly handle environment variable assignment and modification.
			}
			unset
			{
				❌ unset OLDPWD Ensure ability to remove environment variables, specifically OLDPWD.
			}
			env ✅
			{
				✅ Use execve with its own environment.
				✅ on load create Array-list of env-variables
				✅ use everyweher the builtin env-lst
			}
			exit ✅
			{
				exit -42
				exit abc def
				exit 12 34  (geht bis long_max)
			}
		}

- ✅		grepp
		=> 127

-		exit should return the correct arguments and exit_code
			* exit -42			=> prints "exit"	=> 214
			* exit 123 456		=> prints "exit" then prints "bash: exit: too many arguments"	=>	doesn't exit
			* exit abc			=> prints "exit" then prints "bash: exit: abc: numeric argument required"	=>	exit_code: 255
			* exit ab 12		=> prints "exit" then prints "bash: exit: abc: numeric argument required"	=>	exit_code: 255
			* exit 127 cd		=> prints "exit" then prints "bash: exit: too many arguments"	=>	doesn't exit
			* exit 1			=> prints "exit"	=>	1
			* exit 0			=> prints "exit"	=>	0
			* exit				=> prints "exit"	=>	0

-		Here_doc
			bash-3.2$ cat << EOF
			> hallo
			> $USER
			> '$USER'
			> "$USER"
			> EOF
			hallo
			nmihaile
			'nmihaile'
			"nmihaile"

Additional Considerations
* Error Handling: Provide meaningful error messages and use consistent exit codes.
* Memory Management: Properly allocate and free memory to prevent leaks.
* Signal Handling: Manage signals (SIGINT, SIGQUIT, etc.) gracefully within your shell and built-ins.
* Portability: Consider the portability of your shell across different Unix-like systems.
* Customization: Allow customization through shell variables or configuration files.
* Scripting Support: If your shell will support scripting, consider syntax, control structures, and script execution features.
* User Documentation: Provide clear documentation for your shell's features, built-ins, and usage.
--------------------------------------------------------------------------------
Test CMDs

-		sleep 5 | sleep 3 | sleep 3 | sleep 3 | sleep | sleep 3 | sleep 3

--------------------------------------------------------------------------------

RESOURCES

-		Abstract syntax tree
		https://en.wikipedia.org/wiki/Abstract_syntax_tree

-		COLOR PROMPT ERROR explanation:
		https://stackoverflow.com/questions/31329952/colorized-readline-prompt-breaks-control-a

-		SHELL Language explanation:
		https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html
