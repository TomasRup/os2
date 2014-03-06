/*
 ============================================================================
	Tomas Petras Rup�ys ir Dominykas �io�inis
	Operacini� Sistem� atsiskaitymo u�duotis
 ============================================================================
 */
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>

/** The beginning */
int main(void) {
	puts("OS2 Deadline: kovo 28!");
	
	// Pagal destytojo ant lentos brezta konspekta:
	// Loader() -> byte[] is failo
	// while (true) {
	//   komandos dekodavimas (atmintis per puslapiavima)
	//   komandos vykdymas (tik CPU, atmintis)
	//   kanalu irenginys
	//   timer
	//   pertraukimu tikrinimas (supervizoriaus rezimas)
	// }
	
	std::getchar();
	return EXIT_SUCCESS;
}