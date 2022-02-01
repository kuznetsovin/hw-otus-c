# HW03 

## Description

Create [Rarjpeg](https://lurkmore.to/Rarjpeg) file detector without external dependencies. If checking file is _rarjpeg_, then program must print all filenames from the archive to display.

Instead of `rar` archive will be used `zip` archive in test images. 

## Build

For build program run command:

```
make
```

If you want run test only use command:

```
make test
```

## Usage

For using program run command:

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
