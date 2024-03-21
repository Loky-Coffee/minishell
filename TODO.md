
- o		THE ROAD TO NINJASHELL
- ✅		getting started
- ✅		lexer

- *		parser
			* redirects
			* prescedance && || |

-		executer
			* expander
			* redirects

-		built-ins
		-	exit -42
		- exit abc def
		- exit 12 34  (geht bis long_max)

- ✅		grepp
		=> 127

- 		own env
		* execve use own env
		* on load create linked-list of env-variables
		* use everyweher the builtin env-lst
			=> in cd…

-		cd
			cd -
		✅	cd ~
			cd ~/Documents
			unset OLDPWD
			==> check exit_codes

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

--------------------------------------------------------------------------------

Test CMDs

-		sleep 5 | sleep 3 | sleep 3 | sleep 3 | sleep | sleep 3 | sleep 3

--------------------------------------------------------------------------------

RESOURCES

-		Abstract syntax tree
		https://en.wikipedia.org/wiki/Abstract_syntax_tree
		
-		COLOR PROMPT ERROR explanation:
		https://stackoverflow.com/questions/31329952/colorized-readline-prompt-breaks-control-a		
