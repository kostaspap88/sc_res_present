An AVR PRESENT cipher implementation for ATtiny45 that will be extended to ATmega.
Instead of the original PRESENT Sboxes it uses new 4-bit Sboxes with better transparency order, aiming at better DPA-resistance

New Sboxes:

0 -> 6
1 -> 5
2 -> 9
3 -> d
4 -> c
5 -> 0
6 -> 7
7 -> f
8 -> a
9 -> b
a -> 2
b -> 4
c -> e
d -> 3
e -> 1
f -> 8

Full paper details to be updated.

Files: 
PRESENT_with_ORIGINAL_Sboxes : AVR implementation with the standard S-boxes defined in the PRESENT paper 
[PRESENT: An Ultra-Lightweight Block Cipher, A. Bogdanov, L.R. Knudsen, G. Leander, C. Paar, A. Poschmann,
M.J.B. Robshaw, Y. Seurin, and C. Vikkelsoe]

PRESENT_with_UPDATED_Sboxes : AVR implementation with the new S-boxes that present better transparency order
[paper to be updated]

bochum_PRESENT_8bit_ORIGINAL_Sboxes : debug C code for standard Sboxes [http://www.lightweightcrypto.org/]

bochum_PRESENT_8bit_UPDATED_Sboxes : debug C code for new Sboxes [http://www.lightweightcrypto.org/] 

