
- o		THE ROAD TO NINJASHELL
- ✅		getting started
- ✅		lexer

- *		parser
			* redirects
			* prescedance && || |

-		executer
			* expander
			* redirects

-		built-ins:
		{
			echo
			{
				❌ Basic output functionality.
				❌ Handling of command-line arguments.
				❌ Interpretation of escape sequences (optional, depends on shell specifications).
			}
			cd
			{
				✅	cd - Switch to the previous directory and update OLDPWD and PWD environment variables.
				✅	cd ~ Switch to the user's home directory (use the HOME environment variable).
				✅	cd ~/Documents Navigate to a directory relative to the home directory.
				✅	==> check exit_codes Ensure correct exit status on success/failure.
				❌ Must take the path from our environment. Use environmental variables for path resolution.
				❌ handeln cd $PATH Navigate using environment variable paths.
				❌ handeln cd ../Desktop : Support relative path navigation.
				✅ handeln "cd" 	=> cds into Home_Directory
			}
			pwd
			{
				❌ Must take the path from our environment.  Ensure the output reflects the current directory from the environment, not just the working directory.
			}
			export
			{
				❌ export PATH=$PATH:/Users/aalatzas/Desktop/mein_git/minishell  check with which minishell or minishell from other directory
				❌ Must take the path from our environment. Properly handle environment variable assignment and modification.

			}
			unset
			{
				❌ unset OLDPWD Ensure ability to remove environment variables, specifically OLDPWD.
			}
			env
			{
				❌ Use execve with its own environment.
				❌ on load create linked-list of env-variables
				❌ use everyweher the builtin env-lst
			}
			exit
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



# Aris 22.03.24 commit komentare:
# guten morgen!
* commit msg new aris: ich habe ausgelagert die funktion is_tilde. dazu habe ich mit strjoin den problem gellost falls nach der tilde ein path angegeben ist.. like cd ~/Desktop um das zu erreichen musste ich die tilde als saparater token speichern... hofe ist ok :P und wenn ich das gemacht habe dachte ich ich wechle auch den minus und prufe mit is_sigle_token .... TOKEN_MINUS . aber ist nicht moglich da dann bei ls -la wir 3 tokens haben.. Apsolute path wie cd minishell/libft geht auch. was nicht geht ist cd ../libft man musste auch die 2 punkte als eine token speichern...und wieder mit ft_strjoin arbeiten um das zu erreichen.

* dazu in bash wenn man cd $PATH eingegeben wird bekkomt man diese ausgabe..
((((bash: cd: /Users/aalatzas/.brew/bin:/Users/aalatzas/.brew/bin:/Users/aalatzas/.brew/bin:/Users/aalatzas/.brew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/munki:/Library/Apple/usr/bin:/Library/Frameworks/Mono.framework/Versions/Current/Commands:/Users/aalatzas/.brew/bin: No such file or directory))))
wie man das realiseren kann ich habe keine anought... kann es sein das perror den $PATH lessen kann??


# also TODO:
			handeln cd $PATH
			handeln cd ../Desktop


# <<<<<bug>>>>: History hat nicht funkuniert! ... kann man es reproduzieren?? oder war eine einzahl fall?

aalatzas ~/Desktop/mein_git/minishell [main] $ ./minishell
        •  •  ┏┓┓┏┏┓┓ ┓
      ┏┓┓┏┓┓┏┓┗┓┣┫┣ ┃ ┃
━━━━━━┛┗┗┛┗┃┗┻┗┛┛┗┗┛┗┛┗┛━━━━━━
           ┛

ninjaSHELL: /minishell $ cd &PATH
type: [1] str: [cd]
-------------------------
type: [1] str: [&PATH]
-------------------------
ninjaSHELL: cd: &PATH: No such file or directory
ninjaSHELL: /minishell $ cd $PATH
type: [1] str: [cd]
-------------------------
type: [1] str: [$PATH]
-------------------------
ninjaSHELL: cd: $PATH: No such file or directory
ninjaSHELL: /minishell $ cd ../../../../
Display all 214 possibilities? (y or n)
.localized   ahocuk/      asioud/      bocal/       daspring/    dsonko/      exam/        fsandel/     hidris/      jeberle/     kishimwe/    lporoshi/    mickert/     momweno/     ngrebhar/    nsloniow/    pkothapa/    rmeuth/      sprodatu/    tjkruger/    vlenard/     yowoo/
Shared/      aholper/     avelan/      bpisak-l/    davnguye/    dszymanc/    fal-gana/    fschipor/    hipham/      jel-kasm/    kkaratsi/    lschiera/    mkazzaz/     motalha/     nhaas/       obenjair/    plefev/      rpambhar/    ssayor/      tkerber/     vmamoten/    ytoshihi/
aal-samm/    ahuluban/    aweissha/    bszikora/    dcharala/    eahn/        fduriqi/     fschmid/     hkumbhan/    jevincen/    klamprak/    lsorg/       mkraisma/    mouasif/     njantsch/    oemelyan/    prehmann/    rriebsch/    sschneid/    tkilling/    voparkan/    zkilic/
aalatzas/    ajehle/      aweizman/    bwerner/     deepthought/ eallas/      fgabler/     fsturm/      htaimour/    jgraf/       kmohamed/    makurz/      mleibeng/    mreidenb/    nkolle/      ohamadou/    rallouan/    rstix/       ssergiu/     tlai/        vsivanat/    zsszepfa/
aalhalab/    akroll/      awolschi/    cberariu/    dgiurgev/    ekulichk/    fjenisch/    fvoicu/      ibochkar/    jidrizi/     kwelz/       maustel/     mmeising/    mrezaei/     nmihaile/    ohanchak/    ramoussa/    sawang/      ssudarsa/    tlemma/      wdegraf/
aantonie/    amecani/     azhu/        cboubour/    dhendzel/    elenz/       fkhodara/    ggeorgie/    ihama/       jkarippa/    lfiorini/    mbelhaj-/    mmirje/      mring/       nnabaeei/    olanokhi/    rburgsta/    sbintars/    swahrman/    tokraft/     wollio/
abouthib/    amohame2/    bbohle/      chuhlig/     disufi/      enijakow/    fkolly/      gjupy/       iminic/      jludt/       linxu/       mben-has/    moabid/      mrubsame/    nnasilow/    osaral/      rdoukali/    schibane/    tbolsako/    tsimitop/    ygolshan/
adinari/     amrmoham/    bbrocai/     cpuiu/       dkaiser/     eozogul/     flafi/       gsiddiqu/    isemin/      jsanger/     lkrabbe/     mbenchak/    mobadiah/    mstrauss/    nniraula/    otuser/      rdragan/     sdemiroz/    tdehne/      ttaylan/     ykerdel/
adrherna/    anoury/      blohrer/     cutku/       dmendes/     eschirni/    fmarquar/    gstronge/    itekir/      juatorre/    lkreuzer/    mdchich/     mokeller/    mtravez/     nrokkou/     pgeeser/     rkedida/     semakni/     tevers/      vantoniu/    ykhabulo/
aguediri/    apeposhi/    bluzi/       cyazid-r/    dscheffn/    eunskim/     frapp/       hameer-b/    jbook/       kapatel/     llacsivy/    mevangel/    mokoucha/    ngoyat/      nscheefe/    pgrossma/    rmarquar/    slaszlo-/    tgmelin/     vkhangem/    yorlians/
ninjaSHELL: /minishell $ cd ../../../../exam
type: [1] str: [cd]
-------------------------
type: [1] str: [../../../../exam]
-------------------------
ninjaSHELL: /Users $ cd -
ninjaSHELL: Can't write to history file.
type: [1] str: [cd]
-------------------------
type: [10] str: [-]
-------------------------
ninjaSHELL: /minishell $ cd lala
type: [1] str: [cd]
-------------------------
type: [1] str: [lala]
-------------------------
ninjaSHELL: cd: lala: No such file or directory
ninjaSHELL: /minishell $

# sorry ich muss ins bett kann nicht mehr denke die env mache ich morgen >P