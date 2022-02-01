# HW01 

## Description

Create a program to determine if a file is a so-called [Rarjpeg](https://lurkmore.to/Rarjpeg) (an image file with an embedded archive) and to print out a list of filenames in the archive in case the file is actually a Rarjpeg. 

For simplicity a `zip` archive will be used instead of `rar`.

## Build

To build the program run command:

```
make
```

To run only the test, use command:

```
make test
```

## Usage

To use the program run command:

```
./bin/zipjpeg [FILE]
```

`[FILE]` - path to checking file


## Useful links 
1. [The structure of a PKZip file](https://users.cs.jmu.edu/buchhofp/forensics/formats/pkzip.html)
2. [Описание формата ZIP файла](https://blog2k.ru/archives/3391)
3. [Zip-файлы: история, объяснение и реализация](https://habr.com/ru/company/mailru/blog/490790/#17)
4. [Zip – как не нужно создавать формат файлов](https://habr.com/ru/company/ruvds/blog/569464/)
5. [Методы обнаружения "склееных" файлов](https://habr.com/ru/company/infowatch/blog/337084/)
