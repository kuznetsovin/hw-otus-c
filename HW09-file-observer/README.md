# HW09

## Description

Create the observing file size application, which receive data via socket and using `inotify`    

## Build

To build the program run command:

```
cmake
make file-observer <observing file> <bus socket>
```

## Usage

To use the program run command:

```
./file-observer [-d] <observing file> <socket file>
```

`-d` - demonize
`<observing file>` - observing file
`<socket file>` - input city

