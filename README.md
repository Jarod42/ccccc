![Ubuntu badge](https://github.com/Jarod42/CCCCC/actions/workflows/ubuntu.yml/badge.svg)

----------

CCCCC stands for C and C++ Code Counter using Clang.

Similar to [CCCC](http://sourceforge.net/projects/cccc/),
CCCCC is a tool which analyzes C/C++ files and generates a report on various metrics of the code.
Metrics supported include lines of code, [McCabe's Cyclomatic Complexity](http://en.wikipedia.org/wiki/Cyclomatic_complexity), [Halstead metrics](http://en.wikipedia.org/wiki/Halstead_complexity_measures).

Those metrics can help to identify complex code which might need refactoring.

----------

Basic Usage
----

ccccc srcFiles > output.html

It will create a report in output.html with each function and associated counters.

It currently may output in stderr some errors about header file not found.
You may ignore them (some prototype may be wrong in the report for unknown type)
or fix include path with -I option (like with clang/gcc).

Command arguments
----

`ccccc [-D define] [-I include-path] [-pch pch-file] [-e extra_options] [-t template-file] files...`


`-D`, `--define=name`          Predefine name as a macro, with definition.

`-I`, `--include-dir=dir`      Add the directory dir to the list of directories to be searched for header files.

`-pch`                         Compiled header path

`-e`, `--extra-option=option`  Extra option directly given to the clang parser (as `-e "-std=c++17"`).

`-t`, `--template-file=file`   Template file to use for report. (default is template/html/template.tpl)

`files...`                     List of files to examine


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
- ctemplate (zip included)
- doctest (for unit tests) (included (header-only))
