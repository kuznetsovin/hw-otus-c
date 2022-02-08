# HW 02

## Description

Create utf-8 encoder. Program must convert encoding (cp-1251, koi8-r, iso-8859-5) to utf-8. 

## Build

To build the program run command:

```
make
```

To run only the test, use command::

```
make test
```

## Usage

To use the program run command:

```
./bin/iconv [encoding] [INPUT_FILE] [OUTPUT_FILE]
```

- `[encoding]` - input file encoding (cp-1251, koi8-r, iso-8859-5)
- `[INPUT_FILE]` - path to source file
- `[OUTPUT_FILE]` - path to output file


## Useful links 

1. [Абсолютный минимум который каждый разработчик должен знать о Unicode и кодировках](https://www.cyberforum.ru/blogs/33029/blog5139.html)
2. [Unicode mapping](https://unicode.org/Public/MAPPINGS/)
