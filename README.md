# RLe (Run Length Encoding)
Run Length is a lossless compression algorithm.  
The goal of this project is to implement it and some other compression algorithms.  
The purpose is to discover compression in general.

Let's have some fun !


## How to compile
```sh
make
```


## Usage
Encode / Compress
```sh
./RLe [-e] [-i input_file] [-o output_file]
```

Decode / Decompress
```sh
./RLe -d [-i input_file] [-o output_file]
```


### Options
The options can be used in any order :

`-e` encoding mode : Is enabled by default (it can be ommited when encoding)  
`-d` decoding mode  
`-i` input file : If not specified, stdin is used  
`-o` output file : Will be created. If not specified, stdout is used  


### Example
```sh
./RLe -e -i input.txt -o output.txt
```

Is equivalent to :

```sh
cat input.txt | ./RLe -e > output.txt
```


### To Do
- [x] Makefile
- [x] Organize source code
- [x] Add options in main
- [x] Implement default Run Length encoder
- [x] Implement default Run Length decoder
- [ ] Implement Run Length escape encoder
- [ ] Implement Run Length escape decoder
- [ ] Optimization using Burrows–Wheeler Transform (BWT) algorithm on binary data
- [x] Try with stream fonctions (fopen, fread, fwrite...)
- [ ] Find another (harder) algorithm to implement
