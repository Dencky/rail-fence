## Description
Simple implementation of a [rail-fence](https://en.wikipedia.org/wiki/Rail_fence_cipher) encryption and decryption written in C.

## Usage:
Program has these options:

**-e** [encrypt] 
Used to specify you want to encrypt
Example: 

    $ rail-fence -e "Hi how are you doing?"

**-d**	[decrypt] Example:
Used to specify you want to decrypt
Example: 

    $ rail-fence -d "H oniwayuig or  o?hed"

**-f** [file] : 
Used to specify the input file
Example:

    $ rail-fence -e -f input.txt

**-o** [output] :
Used to specity the output file
Example:

    $ rail-fence -e "Hi how are you doing?" -o output.txt

**-r** [rails] :
Used to specify the number of rails (4 default)
Example:

    $ rail-fence -e "Hi how are you doing?" -r 3

## Installation
Simply clone the git repository:

    $ git clone https://github.com/Dencky/rail-fence.git
   
   And then use your favourite C compiler to compile, for example:
   

    $ gcc rail-fence.c -o rail-fence
