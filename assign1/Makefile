HEAD = Prototypes.h Global.h Declare.h

All: RSA_Key RSA_En RSA_De

RSA_Key: RSA_Key.o Lib.o
	gcc -o rsa_key RSA_Key.o Lib.o
	strip rsa_key
RSA_En: RSA_En.o Lib.o
	gcc -o rsa_en RSA_En.o Lib.o
	strip rsa_en
RSA_De: RSA_De.o Lib.o
	gcc -o rsa_de RSA_De.o Lib.o
	strip rsa_de

RSA_Key.o: RSA_Key.c ${HEAD}
	gcc -c -O RSA_Key.c
RSA_En.o: RSA_En.c ${HEAD}
	gcc -c -O RSA_En.c
RSA_De.o: RSA_De.c ${HEAD}
	gcc -c -O RSA_De.c
Lib.o: Lib.c ${HEAD}
	gcc -c -O Lib.c
