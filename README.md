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
./RLe <input_file> <output_file>
```

Decode / Decompress
```sh
./RLe -d <input_file>
```

### To Do
- [x] Makefile
- [ ] Organize source code
- [ ] Add options in main
- [x] Implement default Run Length encoder
- [x] Implement default Run Length decoder
- [ ] Implement Run Length escape encoder
- [ ] Implement Run Length escape decoder
- [ ] Optimization using Burrows–Wheeler Transform (BWT) algorithm on binary data
- [ ] Try with stream fonctions (fopen, fread, fwrite...)
- [ ] Find another (harder) algorithm to implement
