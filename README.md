![Ubuntu badge](https://github.com/Jarod42/CCCCC/workflows/Ubuntu/badge.svg)

CCCCC
====

CCCCC (C and C++ Code Counter using Clang) is a tool which reports some metrics like LOC (line of code), Mc Cabe Cyclomatic Number or Halstead metrics.


Those metrics can help to identify complex code which might need refactoring.

----------

Command arguments
----

`ccccc [-D define] [-I include-path] [-pch pch-file] [-e extra_options] [-t template-file] files...`


`-D`,`-define`
  Define to pass to the preprocessor.

`-I`, `-include-dir`
  Include paths to use.

`-pch`
  Pre-compiled header to use.

`-e`, `-extra-option`
  Extra option directly given to the clang parser.

`-t`, `-template-file`
  template file to use for the report (default is `template/html/template.tpl`).

`files...`
  List of files to examine




Build Dependencies
----

- LLVM/Clang
- ctemplate
- unittest-cpp (for unit tests)
