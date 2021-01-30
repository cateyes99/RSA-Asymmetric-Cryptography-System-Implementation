The improvement of this version, compared to the old one, is the divide1( ) func in Lib.c. The new divide1( ) func can now return the correct remainder. Furthermore, all functions in Lib.c relevant to the divide1( ) have been changed respectively.

As a result, the authentication speed is now twice as quick as the old version (experiments done by encrypting and decrypting a file with the size of about 2.28 KBytes).


------------------------------------------------------------------------
1. For generating all the keys
 	(1.99+0.84+1.16+0.55+1.87+.86)/6 = 1.212 seconds

2. For encryption and decrytion a file with the size of 2028 bytes
The time to encrypt it:
 	(2.96+4.34+3.64+2.56+2.3)/5 = 3.16 seconds

The time to decrypt it:
	(1.560+2.29+1.71+1.35+1.54)/5 = 1.69 seconds

3. For authentication
New Version:  [divide1( ) func is changed, so it can return correct remainder]
time r_a:
real    0m7.341s
user    0m6.800s
sys     0m0.020s

time r_b:
real    0m5.964s
user    0m5.350s
sys     0m0.010s


Old Version:
time r_a:
real    0m14.369s
user    0m13.120s
sys     0m0.010s

time r_b:
real    0m10.692s
user    0m10.270s
sys     0m0.010s
