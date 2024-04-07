
- o		THE ROAD TO NINJASHELL
- ✅		getting started
- ✅		lexer


TESTs

| Hello


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
			❌	$ Behandeln. in (OLD: replace_dollar NOW: expand_variable).

- *		parser
			* redirects
			* prescedance && || |
			< mandatory | cat


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
				❌ echo -nnnnnnnnnnnnnn "Hello world"
				❌ echo -nnnnnnnnnnnnnnX "Hello world"
			}
			cd ✅
			{
				TODO=Use cmd struct .
				✅	cd - Switch to the previous directory and update OLDPWD and PWD environment variables.
				✅	cd ~ Switch to the user's home directory (use the HOME environment variable).
				✅	cd ~/Documents Navigate to a directory relative to the home directory.
				✅	==> check exit_codes Ensure correct exit status on success/failure.
				✅ Must take the path from our environment. Use environmental variables for path resolution.
				✅ handeln cd ../Desktop : Support relative path navigation.
				✅ handeln "cd" 	=> cds into Home_Directory
				❌ handeln cd $PATH Navigate using environment variable paths. // NEED expander
			}
			pwd ✅
			{
				✅ Must take the path from our environment.  Ensure the output reflects the current directory from the environment, not just the working directory.
			}
			export
			{
				✅	Must take the path from our environment. Properly handle environment variable assignment and modification.
				❌	export 1num=Hello
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
				✅	exit 12 34  (geht bis long_max)
			}
		}

- ✅		grepp
		=> 127

-		exit should return the correct arguments and exit_code
			* exit 123 456		=> prints "exit" then prints "bash: exit: too many arguments"				=> x	return 1
			* exit 127 cd		=> prints "exit" then prints "bash: exit: too many arguments"				=> x	return 1

			* exit ab 12		=> prints "exit" then prints "bash: exit: abc: numeric argument required"	=> ✓	exit_code: 255
			* exit abc			=> prints "exit" then prints "bash: exit: abc: numeric argument required"	=> ✓	exit_code: 255
			* exit 1			=> prints "exit"															=> ✓	1
			* exit 0			=> prints "exit"															=> ✓	0
			* exit				=> prints "exit"															=> ✓	0
			* exit -42			=> prints "exit"															=> ✓	214

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