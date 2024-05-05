
- o		THE ROAD TO NINJASHELL
- ✅		getting started
- ✅		lexer
- ✅		parser
		✅ 	SORT REDIRECTS (HERE_DOC has to be before REDIRECT_OUTs // so we should put REDIRECT_Ins && HERE_DOC before REDIRECT_OUTs)
		✅	FIX REDIRECT_IN / OUT PARSING:
				✅	ls > f11 > f12
				✅	>> f11 << f1 << f2 << f3 cat
- ✅		expander
			Happy Hacky FIX
				🍌	order of expansions: (3.5 Shell Expansions)
						🍌 1.	parameter and variable expansion
						🍌 2.	word splitting
						🍌 3.	filename expansion
						🍌 4.	quote characters present in the original word are removed unless they have been quoted themselves (quote removal)
- ✅		executer
			✅	expander musst be called before we decide if cmd is builtin or a command
					export foo=cd && $foo
			✅ HERE_DOC
				✅	here_doc Prompt txt-cursor movable with ARROW-KEYs
				✅	cat << EOF  ctrl+c
				✅	FIX HERE_DOC STDIN after prev redirect_ins
						cat < f1 < f2 < f3 << EOF
			✅	ambiguous redirect
					> $foo
					> $foo$kuh
					> *
			✅	echo A && export lala=42
			✅	ls && ls && ls
			✅	SAVE STDIN_FILENO && STDOUT_FILENO in t_ms 
- ✅ 	Signals
			✅ for minishell -c args
			✅ for minishell testfile
			✅ if OUT is no STDOUT_FILENO aka not a Terminal we don't use (tcgetattr && tcsetattr) and we should show ^C
			✅ for minishell 
			✅ RESET or SET SIGNALS accordingly for SUBSHELLS
			✅ "wc" //=> returns 130 for ctrl-c
			✅ Sigansl for HERE_DOC		(WIFEXITED(status) / WIFSIGNALED(status))
			✅ correct EXIT_CODE (child_exit_status // …)
			✅ wc | sleep 30 | sleep 30
- ✅		Wildcards
			(for a recursive version look below to: A RECURSIVE WILDCARDS PATTERN MATCHING FUNCTION)
			✅	basic implemen`tation
			✅	echo *b
			-	expand "*"	// BUT BASH 3.2 DOESN'T HANDLE THIS AS WELL SO WE COULD LEAVE IT AS IS 
				echo "> >> < * ? [ ] | ; [ ] || && ( ) & # $  <<" 
			-	** 	=>	matches only directories  //=> not aplicable for Minisshell_bonus we only have to implemnt *
- ❌		single argument errorcode for "./minishell ls" should behave like "bash ls"
			./minishell -c lsll		=> exit_code
			./minishell lsss		=> exit_code
- ✅		export
		✅	basic sorting
		✅	displays original ms->envp
		✅ export key without value in ms->unset_envvar
			✅ free ms->unset_envvars
		✅	displays keys with no value
		✅	sort keys && key/value-pairs
		✅	unset loescht auch aus ms->unset_envvars
		✅	add ignore_case for export und unset of ms->unset_envvars	
				(export helo=123 && export HaLo=abc)
		✅	+=
- ✅		historyfile will now be stored in $HOME first // than in "_" // than in argv[0]
			retrieve History Path from environent variable "_"
			if you call "make t" we use av[0] which will be "./minishell"
- ✅ 	Multiple HereDocs: only the last HEREDOC should write into STDIN_FILENO
			✅ >> f11 << f1 << f2 << f3 cat ----> mandatory/here_doc.c:35:9
- ❌ 	norminetten


was machen die funktionen???? wir haben die nicht benutzt....
lstat
fstat
unlink
opendir
readdir
closedir
isatty
ttyname
ttyslot
ioctl
tcsetattr
tcgetattr
tgetent
tgetflag
tgetnum
tgetstr
tgoto
tputs

TESTs

-	✅	| Hello
-	?	Hello |			what should we do?? >> BASH opens a PROMPT but should we???


### && Problem :
export var=hello && echo $var | wc -l
ist the same|
export var=hello; echo $var | echo $var
	no fork			fork		fork

if 1 command and built in --> no fork
if 1 command and not built in --> fork
if multiple commands --> fork

### first Expander and then parser!!!!!!!!!

- *		expander
			✅	$ Behandeln. in (OLD: replace_dollar NOW: expand_variable).

- *		parser
			✅	redirects
			✅	prescedance && || |
			✅	< mandatory | cat


-		executer
			✅	expander
			✅	redirects
			✅	Test for /bin/lss => no file fount
			  	lss => cmd not found

-		built-ins:
		{
			echo
			{
				✅ 	Basic output functionality.
				✅ 	Handling of command-line arguments.
				✅	Interpretation of escape sequences (optional, depends on shell specifications).
				✅	we doen't handel this: echo -nnnnnnnnnnnnnn "Hello world"
				✅ 	we doen't handel this: echo -nnnnnnnnnnnnnnX "Hello world"
			}
			cd ✅
			{
				TODO=Use cmd struct .
				✅	"cd -" Switch to the previous directory and update OLDPWD and PWD environment variables.
					✅ 	Print Path bei "cd -"
				✅	cd ~ Switch to the user's home directory (use the HOME environment variable).
				✅	cd ~/Documents Navigate to a directory relative to the home directory.
				✅	==> check exit_codes Ensure correct exit status on success/failure.
				✅	Must take the path from our environment. Use environmental variables for path resolution.
				✅	handeln cd ../Desktop : Support relative path navigation.
				✅	handeln "cd" 	=> cds into Home_Directory
				✅	handeln cd $PATH Navigate using environment variable paths. // NEED expander
			}
			pwd ✅
			{
				✅	M̶u̶s̶t̶ ̶t̶a̶k̶e̶ ̶t̶h̶e̶ ̶p̶a̶t̶h̶ ̶f̶r̶o̶m̶ ̶o̶u̶r̶ ̶e̶n̶v̶i̶r̶o̶n̶m̶e̶n̶t̶.̶  Ensure the output reflects the current directory from the environment, not just the working directory.
			}
			export
			{
				✅	Must take the path from our environment. Properly handle environment variable assignment and modification.
				✅	export 1num=Hello
					=> bash: export: `1num=Hello': not a valid identifier
				❌	export PATH=$PATH:/Users/aalatzas/Desktop/mein_git/minishell  check with which minishell or minishell from other directory
			}
			unset
			{
				✅ unset OLDPWD Ensure ability to remove environment variables, specifically OLDPWD.
			}
			env ✅
			{
				✅ Use execve with its own environment.
				✅ on load create Array-list of env-variables
				✅ use everyweher the builtin env-lst
			}
			exit ✅
			{
				✅	exit
				✅	exit 0
				✅	exit 1
				✅	exit -42
				✅	exit abc def
				✅	exit 123 456  (geht bis long_max)

				✅	exit 123 456	=> prints "exit" then prints "bash: exit: too many arguments"					=> x	return 1
				✅	exit 127 cd		=> prints "exit" then prints "bash: exit: too many arguments"					=> x	return 1

				✅	exit ab 12		=> prints "exit" then prints "bash: exit: abc: numeric argument required"		=> ✓	exit_code: 255
				✅	exit abc			=> prints "exit" then prints "bash: exit: abc: numeric argument required"	=> ✓	exit_code: 255
				✅	exit 1			=> prints "exit"																=> ✓	1
				✅	exit 0			=> prints "exit"																=> ✓	0
				✅	exit			=> prints "exit"																=> ✓	0
				✅	exit -42		=> prints "exit"																=> ✓	214
			}
		}

- ✅		grepp
		=> 127


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

- ✅		sleep 5 | sleep 3 | sleep 3 | sleep 3 | sleep | sleep 3 | sleep 3

--------------------------------------------------------------------------------

RESOURCES

-		BASH REFERENCE MANUAL
		https://www.gnu.org/savannah-checkouts/gnu/bash/manual/bash.html

		BASH GUIDE FOR BEGINNERS
		https://tldp.org/LDP/Bash-Beginners-Guide/html/index.html

-		BASH Exit_Codes
		https://www.adminschoice.com/exit-error-codes-in-bash-and-linux-os

-		Abstract syntax tree
		https://en.wikipedia.org/wiki/Abstract_syntax_tree

-		COLOR PROMPT ERROR explanation:
		https://stackoverflow.com/questions/31329952/colorized-readline-prompt-breaks-control-a

-		SHELL Language explanation:
		https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html

-		Signal - TERMIOS => tcgetattr / tcsetattr
		https://stackoverflow.com/questions/68602211/forcing-a-terminal-not-to-print-ctrl-hotkeys-when-signals-are-caught

-		DEFAULT $PATH (trotz env -i)
		Der Default-PATH wird fuer Bash beim compelieren in config-top.h gesetzt:
		https://github.com/bminor/bash/blob/master/config-top.h

-		from trusanov Pratt-Parser-In-RUST
		Simple but Powerful Pratt Parsing:
		https://matklad.github.io/2020/04/13/simple-but-powerful-pratt-parsing.html

-		Shunting yard algorithm
		https://en.wikipedia.org/wiki/Shunting_yard_algorithm

-		Operator-precedence parser
		https://en.wikipedia.org/wiki/Operator-precedence_parser

-		Order of operations
		https://en.wikipedia.org/wiki/Order_of_operations
		Operator associativity
		https://en.wikipedia.org/wiki/Operator_associativity

-		WILDCARDS (*) - pattern matching
		https://www.youtube.com/watch?v=fWeTjhgDt3A
		https://www.youtube.com/watch?v=7SHV_QfVROE

-		other Minishells
		Timofei Rusanov / trusanov:		https://github.com/taaae/minishell/blob/main/include/lexer.h
		Markus Kurz / makurz: 			https://github.com/kurz-m/minishell/blob/main/src/expander/expander_main.c
		Jamshidbek2000 / zstenger93		https://github.com/zstenger93/minishell/blob/master/mindmap.png
		@twagger / @iamwen1023			https://github.com/twagger/minishell?tab=readme-ov-file
										(uses / explains grammer [maybe explains it…])

### EXPANDER:

fangt der string mit " und ist in die mitte ' so werden die aussen ' enfern und alles expenden.
bash-3.2$ echo "a'lala'LALA'a"
a'lala'LALA'a
bash-3.2$
		1				1 2					2		3 4			3   4
echo 	"	$USER		" 'lala$USER		'LALA	' "$USERa	'a	"
			aalatzas	  'lalaaalatzas		'LALA	'			'a
bash-3.2$

2. Implementierung der Grundlagen
a. Eingabeverarbeitung
Lesen der Eingabe: Entwickeln Sie eine Methode zum Lesen der Benutzereingabe. Dies könnte das Parsen einer Kommandozeile oder das Lesen einer Datei (für Skripte) umfassen.
b. Tokenisierung
Aufteilung in Tokens: Teilen Sie die Eingabe in sinnvolle Einheiten (Tokens) auf, wie Wörter, Operatoren, und Anführungszeichen. Dies ist grundlegend für die spätere Verarbeitung.
3. Implementierung spezifischer Expansionen
c. Variablenexpansion
Erkennen von Variablen: Implementieren Sie die Logik, um Variablen in der Eingabe zu erkennen ($VAR_NAME).
Auflösung von Variablen: Entwickeln Sie eine Funktion, um den Wert einer Variablen zu ermitteln und den Verweis auf die Variable durch diesen Wert zu ersetzen.
d. Anführungszeichen-Handling
Anführungszeichen: Implementieren Sie eine einfache Logik zum korrekten Umgang mit einfachen (') und doppelten Anführungszeichen ("), um festzulegen, welche Teile der Eingabe buchstäblich zu nehmen sind und welche Expansionen zulassen.
e. Tilde-Expansion
Tilde-Expansion: Fügen Sie die Unterstützung für die Ersetzung von ~ durch das Home-Verzeichnis des Benutzers hinzu.
4. Erweiterte Features
Sobald die grundlegenden Funktionen implementiert sind, können Sie komplexere Features hinzufügen:

Globbing (Wildcard-Expansion)
Arithmetische Expansion
Befehlssubstitution
Escape-Zeichen


1) Anführungszeichen: Beginnen Sie mit der Behandlung von Anführungszeichen, da diese die Interpretation aller anderen Zeichen beeinflussen können. Das korrekte Verarbeiten von einfachen (') und doppelten Anführungszeichen (") ist grundlegend, um zu entscheiden, ob Expansionen durchgeführt werden sollten oder nicht.

2) Escape-Zeichen: Als Nächstes behandeln Sie Escape-Zeichen (\), da diese in direktem Zusammenhang mit Anführungszeichen stehen und bestimmen können, ob Zeichen wörtlich genommen oder speziell behandelt werden.

3) Tilde-Expansion: Die Tilde-Expansion ist relativ einfach zu implementieren und wird oft vor anderen Expansionen durchgeführt, da sie meist unabhängig von diesen ist.

4) Variablenexpansion: Variablen sind ein zentraler Bestandteil von Bash-Scripts. Die Fähigkeit, Variablen durch ihre Werte zu ersetzen, ist essentiell und sollte früh implementiert werden.

5) Arithmetische Expansion: Dies erfordert möglicherweise ein Parsing und eine Auswertung von Ausdrücken, kann aber wichtig sein, um die Flexibilität von Scripten zu erhöhen.

6) Befehlssubstitution: Dies ermöglicht es, die Ausgabe von Befehlen in Argumente und andere Konstrukte einzubetten, und ist eine der leistungsfähigeren Features von Bash.

7) Globbing (Dateinamenserweiterung): Die Expansion von Mustern zu Dateinamen ist wichtig für die Interaktion mit dem Dateisystem und sollte nach den grundlegenden Textexpansionsmechanismen implementiert werden.

8) Prozesssubstitution: Da dies eine komplexere Form der Befehlssubstitution ist und temporäre Dateinamen erfordert, sollte sie zu einem späteren Zeitpunkt angegangen werden.

9) Wortteilung (Word Splitting): Dies ist wichtig für die korrekte Behandlung von Eingaben und Argumenten und hängt von der vorherigen Behandlung von Anführungszeichen und Escape-Zeichen ab.

10) Bedingte Expansion: Ermöglicht die Ausführung von Expansionen basierend auf Bedingungen. Dies erfordert ein tieferes Verständnis von Bash-Zuständen und könnte komplexer in der Implementierung sein.

11) Zuweisungserweiterung: Dies ist nützlich für dynamische Zuweisungen in Skripten und könnte in Kombination mit Variablenexpansion implementiert werden.

12) Entfernung von Anführungszeichen: Schlussendlich sollten Anführungszeichen entfernt werden, wenn sie nicht mehr benötigt werden, was nach allen anderen Expansionsarten erfolgen sollte.



--------------------------------------------------------------------------------

A recursive WILDCARDS PATTERN MATCHING FUNCTION:


static int	is_matching(const char *str, const char *pattern)
{
	if (*pattern == '\0')
		return (*str == '\0');
	if (*pattern == '*')
	{
		while (*str)
		{
			if (is_matching(str, pattern + 1))
				return (1);
			str++;
		}
		return (is_matching(str, pattern + 1));
	}
	else if (*pattern == '?' || *str == *pattern)
		return (is_matching(str + 1, pattern + 1));
	return (0);
}



--------------------------------------------------------------------------------

FIRST HALF OF THE DRAGON BOOK
(https://www.youtube.com/watch?v=fIPO4G42wYE&t=95s from 00:00 to ~45:00)

a > b + c * d + 3

parse_leaf(ct):
	next = get_next_token(ct)

	if is_string_literal(next)	return make_string(next, ct)
	if is_word(next)			return make_word(next, ct)
	if is_operator				return make_operator(next, ct)

	report_error("Unhandled input.)


parse_increasing_precedence(left, min_prec, ct):
	next = get_next_token(ct)
	if !is_binary_operator(next)
		return left
	next_prec = get_precedence(next)
	if next_prec <= min_prec
	{
		return left
	}
	else
	{
		right = parse_expression(next_prec)
		return make_binary(left, to_operator(next), right)
	}


parse_expression(ct, min_prec)
	left = parse_leaf(ct)

	while (true)
	{
		node = parse_increasing_precedence(left, min_prec, ct)
		if node == left
			break
		left = node
	}
	return left

and than the caller attaches it to the graeter than ( > )