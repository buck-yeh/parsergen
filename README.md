# Brief
- `parsergen`/`scannergen` combo generates source code files of LR1/GLR parser & scanner from a set of annotated production rules, aka grammar.
- Both `parsergen` & `scannergen` use the combo to regenerate their own parser & scanner, respectively, to evolve themselves.
- The generated code will contain C++20 keywords, e.g. **constinit**. IOW, building the code with `-std=c++2a` is required.
- You might not always need both:
  - Sometimes you might reuse an existing scanner with another parser. *(to be explained)*
  - Sometimes all you need is just scanner. (see [`CBrackets`](https://github.com/buck-yeh/CBrackets)) 

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
5. But is it possible to just type `grammarstrip` `parsergen` `scannergen` to run them? 🤔 Append the following lines to `~/.bashrc`:
   ~~~bash
   PSGEN_DIR="/full/path/to/current/dir"
   alias grammarstrip="$PSGEN_DIR/ParserGen/grammarstrip"
   alias parsergen="$PSGEN_DIR/ParserGen/parsergen"
   alias scannergen="$PSGEN_DIR/ScannerGen/scannergen"
   ~~~ 
   And type this in a console window:
   ~~~bash
   . ~/.bashrc
   ~~~ 
   There you go! It will take effect in subsequently opened console windows and will last after reboot.
