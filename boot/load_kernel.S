//author: night
//date: 2020/08/25

/*
I'm going to use the "load.c" to link with "boot1.S" to gen "bootblock",
but it seem too big to put them in 512Byte space.

So i just use the "boot1.S" to gen "bootblock",and jump to this file.
then in this file we load_kernel :-)
*/

//The "boot1.S" already set up the env and we are now in the 
//protected mode, the ds=es=0x1000(the first segement seclctor)
//gs=fs=0 not used

.code32

