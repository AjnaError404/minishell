# Récapitulatif technique des modifications 

## 1. Architecture globale & sujet 42 (minishell.h, signals.c, main.c)

### Passage à la structure t_shell :

Création de typedef struct s_shell { char **env; int status; } t_shell;.

Suppression de g_exit_status. La seule variable globale du projet est désormais int g_signal = 0; (qui ne stocke que le numéro du signal reçu, 100 % conforme au sujet).

shell.status remplace l'ancienne globale pour stocker les codes de retour ($?), et shell.env centralise l'environnement au lieu de balader des char ***.

### Gestion des signaux :

signals.c n'assigne plus que g_signal = sig. Le main et l'exécution convertissent le signal en code d'erreur (128 + g_signal, soit 130 pour SIGINT).

## 2. Lexer, Automate & Syntaxe (parsing/fs_automaton.c, parsing/check_lexer.c)

### Correction du heap-buffer-overflow sur les pipes (fs_automaton.c) :

fls_countword ne comptait les séparateurs que si un mot les précédait. Sur |, | ls ou ls || cat, il renvoyait 0, fls_split allouait un bloc trop petit (8 octets) et débordait (Invalid write of size 8).

fls_countword prend désormais en compte add_sep pour comptabiliser les séparateurs même s'ils sont au début ou consécutifs.

### Validation de la syntaxe des pipes (check_lexer.c) :

Ajout de check_lex_pip(lex) appelé juste après create_lexer : bloque les pipes orphelins en début/fin de ligne (|, | ls, ls |, ls || cat) et renvoie directement l'erreur de syntaxe avec code 2 sans tenter de construire l'arbre.

### Vérification des quotes (check_lexer.c) :

check_quotes_closed bloque l'analyse dès le départ si une quote reste ouverte ("feur, 'g perdu), affiche l'erreur et met le statut à 2.

## 3. Redirections & Heredoc (parsing/redir.c, parsing/heredoc.c)

### Redirections collées sans espace (redir.c) :

Dans cmd_to_args, ajout d'un automate add_spaces_redir qui insère des espaces autour des chevrons < et > uniquement lorsqu'ils sont hors quotes. Ainsi, echo wesh>zzz.txt est transformé en echo wesh > zzz.txt avant le split.

Suppression de la fonction morte switch_open qui doublonnait avec fill_args_fd.c.

### Heredoc 100 % en mémoire (heredoc.c) :

Remplacement complet des fichiers temporaires sur disque (/tmp/.heredoc_tmp) par un pipe UNIX in-memory.

Évite toute fuite de fichier sur le disque en cas de Ctrl+C, tout problème de droits (chmod 000) et tout descripteur orphelin.

## 4. Exécution & Mémoire Valgrind (exec_dir/, tree.c, expander.c)

### Suppression des fuites dans les sous-processus (clean_child_exit) :

Dans chaque enfant de fork(), appel systématique à clean_child_exit() qui libère my_env et vide l'arène rumba avant de faire exit(). Les sous-processus quittent avec 0 fuite.

### Nettoyage des descripteurs de fichiers (FD leaks) :

Fermeture des FDs de l'arbre inutilisés dans les branches gauche et droite des pipelines (close_tree_fds).

Dans execute_command, exit ne duplique plus inutilement stdin/stdout (ce qui laissait traîner 2 FDs ouverts au prompt de sortie).

### Expander (expander.c, tree.c) :

L'expansion de $? utilise désormais shell->status.

## 5. Correctifs Builtins & Gestion des Statuts d'Erreur

### Builtin `cd` : Support de `cd` sans argument (`builtins1.c`)
* **Problème :** `cd` sans argument affichait en dur `minishell: cd: need an argument`.
* **Correction :** Si `args[1]` est `NULL`, la fonction récupère la valeur de la variable `HOME` via `find_envvar(env, "HOME")` et bascule dessus. Si `$HOME` n'est pas défini, renvoie l'erreur standard `minishell: cd: HOME not set` avec le code de retour `1`.

### Builtin `exit` : Arguments multiples & synchronisation (`builtins2.c`, `exec_core.c`)
* **Problème :** `exit 1 2 3` affichait le message d'erreur mais ne mettait pas à jour `shell->status`, conservant la valeur de la commande précédente.
* **Correction :** * `ft_exit` renvoie désormais `1` en cas de surplus d'arguments (`too many arguments`) sans interrompre le shell.
  * Dans `exec_core.c`, `run_builtin` récupère ce retour et l'affecte directement à `shell->status`.
  * Normalisation du cast des codes de sortie sur un octet : `exit((unsigned char)status)`.

### Redirections d'entrée inexistantes (`parsing/tree.c`)
* **Problème :** Une commande avec un fichier d'entrée introuvable (ex. `cat < introuvable.txt`) échouait à l'ouverture, mais laissait `$?` à `0`.
* **Correction :** Lorsque `tree_update_args_n_fd` échoue lors de la préparation de l'AST, `create_tree` positionne explicitement `shell->status = 1` avant d'avorter la création du nœud.

## 6. Validation des Pipelines, Priorités et Heredoc

### Pipelines (`exec_redir.c`)
* Gestion des pipelines multiples sans interblocage (tests validés jusqu'à 4 processus en chaîne).
* Les builtins s'exécutent correctement dans les sous-processus sans fermer le shell parent.
* Concurrence et statuts : `$?` reflète systématiquement le statut de sortie de la commande la plus à droite du pipeline (`exit 42 | ls` donne `0`, `ls | exit 42` donne `42`).

### Interruption Heredoc (`heredoc.c`, `signals.c`)
* En cours de saisie heredoc, `SIGINT` (`Ctrl+C`) interrompt immédiatement la boucle de lecture, ferme le pipe sans fuite de descripteur et réinitialise le prompt avec le code de sortie `130`.

---
# Changelog Technique Minishell

### 1. Architecture & Variable Globale
* **`minishell.h`** :
  - Définition de `typedef struct s_shell { char **env; int status; } t_shell;`.
  - Remplacement de `extern int g_exit_status;` par `extern int g_signal;`.
* **`signals.c`** :
  - Remplacement de `g_exit_status` par `int g_signal = 0;`.
  - `handle_sigint(int sig)` : ne fait plus que `g_signal = sig;` en plus du réaffichage du prompt (`rl_on_new_line`, `rl_replace_line`, `rl_redisplay`).
* **`main.c`** :
  - Création de `init_shell(t_shell *shell, t_rumba **rumba, char **envp)` : initialise l'environnement copié et fixe `status = 0`.
  - `main()` : conversion du signal en statut (`shell.status = 128 + g_signal; g_signal = 0;`) dès qu'un signal est intercepté.
  - `process_line()` : prend `t_shell *shell` au lieu de l'ancienne variable globale.

---

### 2. Lexer, Parser & Syntaxe
* **`parsing/fs_automaton.c`** :
  - `fls_countword()` : mise à jour pour comptabiliser les séparateurs consécutifs ou en début de ligne (`add_sep`). Corrige le heap-buffer-overflow (allocation insuffisante dans `fls_split`) sur les cas `|`, `| ls` et `ls || cat`.
* **`parsing/check_lexer.c`** :
  - Ajout de `check_lex_pip(t_lexeme *lex)` : vérifie la validité des pipes dans la liste chaînée de lexèmes avant la création de l'AST. Bloque les pipes orphelins (`|`, `| ls`, `ls |`, `ls || cat`) et renvoie le code d'erreur `2`.
  - `check_quotes_closed(char *line)` : vérifie que toutes les quotes sont appariées. Si non, affiche l'erreur et le `main` assigne `shell->status = 2`.
* **`parsing/redir.c`** :
  - Ajout de `static char *add_spaces_redir(char *cmd, t_rumba **rumba_mk1)` : automate à états finis insérant un espace avant et après chaque chevron (`<`, `>`, `<<`, `>>`) uniquement s'il est en dehors de guillemets (`STATE_OUT_QUOTE`).
  - `cmd_to_args()` : passe la ligne brute dans `add_spaces_redir` avant de la découper avec `fls_split`. Permet de parser correctement les redirections collées (`echo wesh>file`).
  - Suppression de la fonction morte `switch_open`.

---

### 3. Gestion de l'AST & Expander
* **`parsing/tree.c`** :
  - `create_tree(t_lexeme *lexer, t_shell *shell, t_rumba **rumba_mk1)` : prend désormais `t_shell *shell`.
  - Si `tree_update_args_n_fd` échoue (ex. fichier d'entrée introuvable ou heredoc interrompu), la fonction assigne `shell->status = (g_signal ? 128 + g_signal : 1)` avant de renvoyer `NULL`.
* **`expander/expander.c`** :
  - `expand_var()` : utilise `shell->status` pour résoudre `$?` (via `ft_itoa_gp`) et `shell->env` pour résoudre les variables d'environnement ordinaires.
  - Mise à jour des signatures de `treenode_expand_args`, `treenode_expand_cmd` et `tree_expand_all` pour propager `t_shell *shell`.

---

### 4. Heredoc in-memory
* **`parsing/heredoc.c`** :
  - `heredoc_magic(char *delimiter, t_rumba **rumba_mk1)` : refonte complète sans création de fichier temporaire sur disque. Ouvre un `pipe(pipefd)` Unix, écrit les lignes lues dans `pipefd[1]` via un child, puis renvoie `pipefd[0]` au parent.
  - `wait_heredoc(pid_t pid)` : intercepte `SIGINT` sur le child du heredoc, assigne `g_signal = SIGINT` et renvoie `-2` pour stopper la commande.

---

### 5. Builtins & Statuts d'erreur
* **`builtins/builtins1.c`** :
  - `int cd(t_tree *cmd, char **env)` : prend `char **env` en paramètre. Si `cmd->args[1] == NULL`, utilise `find_envvar(env, "HOME")`. Si `$HOME` est absent, affiche `minishell: cd: HOME not set` et renvoie `1`.
* **`builtins/builtins2.c`** :
  - `int ft_exit(t_tree *cmd, t_shell *shell)` : prend `t_shell *shell`. Si plus d'un argument numérique est passé, affiche `minishell: exit: too many arguments` et renvoie `1` sans quitter le shell. Sans argument, quitte avec `shell->status`.
* **`exec_dir/exec_core.c`** :
  - `run_builtin(t_tree *cmd, t_shell *shell)` : met à jour `shell->status` avec le retour de `cd()` et de `ft_exit()`.
  - `execute_command()` : traitement isolé pour `exit` (n'appelle plus `dup()` sur STDIN/STDOUT avant de quitter, éliminant 2 descripteurs résiduels à la fermeture).

---

### 6. Pipeline & Nettoyage des Descripteurs (FDs)
* **`exec_dir/exec_redir.c`** :
  - `exec_left_child()` : ferme les FDs de la branche droite (`close_tree_fds(node->r_child)`) et ferme ses propres FDs avant d'appeler `clean_child_exit()`.
  - `exec_right_child()` : ferme les FDs de la branche gauche (`close_tree_fds(node->l_child)`) et ferme ses propres FDs avant d'appeler `clean_child_exit()`.
  - `execute_pipe()` : attend les deux PIDs et affecte à `shell->status` le statut du child droit (`pid_right`).
* **`exec_dir/exec_tools.c`** :
  - `clean_child_exit()` : libère l'environnement copié et vide l'arène `del_all_rumba()` avant d'appeler `exit()` dans chaque sous-processus fork.