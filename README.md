![Ubuntu badge](https://github.com/Jarod42/CCCCC/actions/workflows/ubuntu.yml/badge.svg)
![Msys2 badge](https://github.com/Jarod42/CCCCC/actions/workflows/msys2.yml/badge.svg)

----------

CCCCC stands for C and C++ Code Counter using Clang.

Similar to [CCCC](http://sourceforge.net/projects/cccc/),
CCCCC is a tool which analyzes C/C++ files and generates a report on various metrics of the code.
Metrics supported include lines of code, [McCabe's Cyclomatic Complexity](http://en.wikipedia.org/wiki/Cyclomatic_complexity), [Halstead metrics](http://en.wikipedia.org/wiki/Halstead_complexity_measures).

Those metrics can help to identify complex code which might need refactoring.

----------

Basic Usage
----

`ccccc myProject/compile_commands.json > output.html`
`ccccc -I IncludeDir srcFiles > output.html`

It will create a report in output.html with each function and associated statistics.

It outputs in stderr some extra information about progress or parsing issues (most of them might be ignored, unknown types (due to missing include path) would be replaced by `int`)

Command arguments
----

`ccccc [-D define] [-I include-path] [-pch pch-file] [-e extra_options] [-t template-file] files...`


- `-D`, `--define=name`          Predefine name as a macro, with definition.
- `-I`, `--include-dir=dir`      Add the directory dir to the list of directories to be searched for header files.
- `-pch`                         Compiled header path
- `-e`, `--extra-option=option`  Extra option directly given to the clang parser (as `-e "-std=c++17"`).
- `-t`, `--template-file=file`   Template file to use for report. (default is template/html/template.tpl)
- `files...`                     List of source files to examine or directly compile_commands.json (all files in that file would be examined)

CCCCC supports response file, i.e. `@my_response_file` in command line. Content of the response file is the command line arguments optionally split by line.

----------

Example
----

That is the result applied on ccccc src files:

[CCCCC report](https://jarod42.github.io/ccccc/)

----------

Build Dependencies
----

- LLVM/Clang
- mstch (included)
- doctest (for unit tests) (included (header-only))
