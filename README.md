# gfm2polygon-statement

[WIP] Converts GFM to Limited LaTeX Format for Codeforces Polygon Statement

> [!CAUTION]
> This project is a work in progress.

## Getting Started

This project depends on [cmark-gfm](https://github.com/github/cmark-gfm), But no additional installation is required. 

When running cmake, dependencies will downloaded automatically.

## How to use

Use stdio with pipelining 
```sh
cat [input] | ./md2pst > [output]
```

Serve file name
```sh
./md2pst [input] > [output]
```

Get output to stdout
```sh
./md2pst [input]
```

## Handling Mathematical Expressions

Mathematical expression is not defined in `cmark-gfm`.

Cause this project neither changes GFM format nor adds specific syntax, using mathematical expression and `$` character needs caution.

For additional information about supporting mathematical expression in GFM, [read the discussion at cmark-gfm issue tracker](https://github.com/github/cmark-gfm/issues/6).
