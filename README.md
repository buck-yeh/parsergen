# Brief
- The `parsergen`/`scannergen` combo generates source code files of LR1/GLR parser & scanner from a set of annotated production rules, aka grammar.
- Both `parsergen` & `scannergen` use the same combo to re-generate their own parser & scanner, respectively, to evolve themselves.
- Building the generated code with [`-std=c++2a`](https://gcc.gnu.org/projects/cxx-status.html#cxx20) is required.
- 🧘 Most often you need *the combo*, but not always:
  - Sometimes reusing an existing scanner with another parser is feasible and cheaper. *(to be explained)*
  - Sometimes a standalone scanner suffices. (see [`CBrackets`](https://github.com/buck-yeh/CBrackets)) 

# Table of Contents
   * [Installation](#installation)
      * [in ArchLinux](#in-archlinux)
      * [from github in any of Linux distros](#from-github-in-any-of-linux-distros)
   * [A quick guide to parsergen/scannergen combo](#a-quick-guide-to-parsergenscannergen-combo)
      * [Write grammar](#write-grammar)
      * [Generate C++ code of parser &amp; scanner](#generate-c-code-of-parser--scanner)
         * [When package parsergen is installed in ArchLinux](#when-package-parsergen-is-installed-in-archlinux)
         * [When parsergen is built from github](#when-parsergen-is-built-from-github)
         * [If target source files already exist](#if-target-source-files-already-exist)
      * [Use the generated](#use-the-generated)
         * [Includes](#includes)
         * [Scanner|screener|parser piped to parse](#scannerscreenerparser-piped-to-parse)

   *(Created by [gh-md-toc](https://github.com/ekalinin/github-markdown-toc))*

# Installation
## in [ArchLinux](https://archlinux.org/)
1. Make sure you have installed [`yay`](https://aur.archlinux.org/packages/yay/) or any other [pacman wrapper](https://wiki.archlinux.org/index.php/AUR_helpers).
2. `yay -S parsergen` to install.
3. `yay -Ql parsergen` to see the installed files:
   ~~~txt
   parsergen /usr/
   parsergen /usr/bin/
   parsergen /usr/bin/grammarstrip
   parsergen /usr/bin/parsergen
   parsergen /usr/bin/scannergen
   parsergen /usr/share/
   parsergen /usr/share/licenses/
   parsergen /usr/share/licenses/parsergen/
   parsergen /usr/share/licenses/parsergen/LICENSE
   parsergen /usr/share/parsergen/
   parsergen /usr/share/parsergen/RE_Suite.txt
   ~~~
4. Three commands `grammarstrip` `parsergen` `scannergen` at your disposal.

## from github in any of [Linux distros](https://distrowatch.com/)
1. Make sure you have installed `cmake` `make` `gcc` `git` `fmt`
2. ~~~bash
   git clone -b main --single-branch https://github.com/buck-yeh/parsergen.git .
   cmake .
   make -j
   PSGEN_DIR="/full/path/to/current/dir"
   ~~~
   *p.s.* You can install a tagged version by replacing `main` with [tag name](https://github.com/buck-yeh/parsergen/tags).
4. Three commands at your disposal:
   - `$PSGEN_DIR/ParserGen/grammarstrip` 
   - `$PSGEN_DIR/ParserGen/parsergen` 
   - `$PSGEN_DIR/ScannerGen/scannergen`
5. 🤔 But is it possible to just type `grammarstrip` `parsergen` `scannergen` to run them?  
   💡 Append the following lines to `~/.bashrc`:
   ~~~bash
   PSGEN_DIR="/full/path/to/parsergen/dir"
   alias grammarstrip="$PSGEN_DIR/ParserGen/grammarstrip"
   alias parsergen="$PSGEN_DIR/ParserGen/parsergen"
   alias scannergen="$PSGEN_DIR/ScannerGen/scannergen"
   ~~~ 
   And type this in a console window:
   ~~~bash
   . ~/.bashrc
   ~~~ 
   There you go! It will also take effect in subsequently opened console windows and will last after reboot.

# A quick guide to `parsergen`/`scannergen` combo
When you need to quickly implement a parser for an improvised or deliberately designed [DSL](https://en.wikipedia.org/wiki/Domain-specific_language), prepare a grammar file in simple [BNF](https://en.wikipedia.org/wiki/Backus%E2%80%93Naur_form#Example) rules with semantic annotations and then let the combo generate C++ code of parser & scanner. 

## Write grammar
[`example/CalcInt/grammar.txt`](example/CalcInt/grammar.txt) defines a calculator for basic arithmetics `+ - * / %` of integral constants in *decimal*, *octal*, or *hexadecimal*.
~~~php
lexid   Spaces

//
//      Output Options
//
%CONTEXT [[std::ostream &]]

%ON_ERROR [[
    $c <<"COL#" <<$pos.m_Col <<": " <<$message <<'\n';
]]

%EXTRA_TOKENS   [[dec_num|oct_num|hex_num|spaces]]
//%SHOW_UNDEFINED

//
//      Operator Precedency
//
left   + -
left   * / %
right  ( )

//
//      Grammar with Reduction Code
//
<@> ::= <Expr>  [[
    $r = $1;
]]

<Expr> ::= <Expr> + <Expr>  [[
    bux::unlex<int>($1) += bux::unlex<int>($3);
    $r = $1;
]]
<Expr> ::= <Expr> - <Expr>  [[
    bux::unlex<int>($1) -= bux::unlex<int>($3);
    $r = $1;
]]
<Expr> ::= <Expr> * <Expr>  [[
    bux::unlex<int>($1) *= bux::unlex<int>($3);
    $r = $1;
]]
<Expr> ::= <Expr> / <Expr>  [[
    bux::unlex<int>($1) /= bux::unlex<int>($3);
    $r = $1;
]]
<Expr> ::= <Expr> % <Expr>  [[
    bux::unlex<int>($1) %= bux::unlex<int>($3);
    $r = $1;
]]
<Expr> ::= ( <Expr> )       [[
    $r = $2;
]]
<Expr> ::= $Num             [[
    $r = bux::createLex(dynamic_cast<bux::C_IntegerLex&>(*$1).value<int>());
]]
~~~ 

## Generate C++ code of parser & scanner
### When package `parsergen` is installed [in ArchLinux](#in-archlinux)
~~~bash
parsergen grammar.txt Parser tokens.txt && \
scannergen Scanner /usr/share/parsergen/RE_Suite.txt tokens.txt
~~~
### When `parsergen` is [built from github](#from-github-in-any-of-linux-distros)
~~~bash
parsergen grammar.txt Parser tokens.txt && \
scannergen Scanner "$PSGEN_DIR/ScannerGen/RE_Suite.txt" tokens.txt
~~~

where
| Parameter | Description
|:----------:|----------- 
| `grammar.txt` | Annotated BNF rules and other types of options.
| `Parser` | Output file base - generating __Parser__*.cpp* __Parser__*.h* __Parser__*IdDef.h*
| `Scanner` | Output file base - generating __Scanner__*.cpp* __Scanner__*.h* 
| `tokens.txt` | Output of `parsergen` & input of `scannergen`
| [`RE_Suite.txt`](ScannerGen/RE_Suite.txt) | Recurring token definitions provided with `scannergen` and used by `tokens.txt`

### If target source files already exist
💡 Put the commands in a script called [`reparse`](test/archlinux/aur_poc/reparse) for recurring uses.

ℹ️ `parsergen` will prompt `(y/n)` questions three times and `scannergen` will ask twice.
~~~bash
> ./reparse 
About to parse 'grammar.txt' ...
Total 1 lex-symbols 1 nonterms 9 literals
states = 30	shifts = 106
Spent 0.005232879"
38 out of 106 goto keys erased for redundancy.
ParserIdDef.h already exists. Overwrite it ?(y/n)y
Parser.h already exists. Overwrite it ?(y/n)y
Parser.cpp already exists. Overwrite it ?(y/n)y
Parser created
#pos_args = 4
About to parse '/usr/share/parsergen/RE_Suite.txt' ...
About to parse 'tokens.txt' ...
Scanner.h already exists. Overwrite it ?(y/n)y
Scanner.cpp already exists. Overwrite it ?(y/n)y
> _ 
~~~

## Use the generated
ℹ️ from [`example/CalcInt/main.cpp`](example/CalcInt/main.cpp)
### Includes
~~~c++
#include "Parser.h"         // C_Parser
#include "ParserIdDef.h"    // TID_LEX_Spaces
#include "Scanner.h"        // C_Scanner
~~~
💡 Including `ParserIdDef.h` may not be necessary when spaces can't be ignored. 

### Scanner|screener|parser piped to parse
~~~c++
C_Parser                            parser{/*args of context ctor*/};
bux::C_ScreenerNo<TID_LEX_Spaces>   screener{parser}; // (1)
C_Scanner                           scanner{screener};
bux::C_IMemStream                   in{line}; // or other std::istream derived
bux::scanFile(">", in, scanner);

// Check if parsing is ok
// ... (2)

// Acceptance
if (!parser.accepted())
{
   std::cerr <<"Incomplete expression!\n";
   continue; // or break or return
}

// Apply the result 
// parser.getFinalLex() ... (3)
~~~
* *(1)* Screener is filter of scanner and can filter out, change, aggregate selected tokens. Don't use it if you don't need it:
   ~~~c++
   C_Parser                            parser{/*args of context ctor*/};
   C_Scanner                           scanner{parser};
   bux::C_IMemStream                   in{line}; // or other std::istream derived
   bux::scanFile(">", in, scanner);
   ~~~
* *(2)* Time to check integrity of your context status.
* *(3)* `parser.getFinalLex()` returns reference to the merged result of type `bux::LR1::C_LexInfo`, defined in [`LR1.h`](https://github.com/buck-yeh/bux/blob/main/include/bux/LR1.h). In this example, the expected result is integral value of type `int` and can be conveniently obtained by expression 
   ~~~c++
   bux::unlex<int>(parser.getFinalLex())
   ~~~
   An alternative way is to reap the result from the user context instance instead of calling `parser.getFinalLex()`. The difference is the former resides on stack whereas the latter resides on heap. 
