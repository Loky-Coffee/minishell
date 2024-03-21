
- ✅		getting started
-		parser
-		executer
-		built-ins
		-	exit -42
		- exit abc def
		- exit 12 34  (geht bis long_max)

-		lsss
		+> 127

- 		own env
		- execve use own env

cd -
cd ~
cd ~/Documents
unset OLDPWD
==> check exit_codes

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
