/*
 ============================================================================
	Tomas Petras Rupšys ir Dominykas Šiožinis
	Operacinių Sistemų atsiskaitymo užduotis
 ============================================================================
 */
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>

// Deklaruojame konstantas --------------------------------------------------
#define TRUE 1
#define FALSE 0
#define INCORRECT_PARAMETERS "Klaida! Neteisingi parametrai!"
#define FILE_DOES_NOT_EXIST "Klaida! Failas neegzistuoja!"
#define END_OF_WORK "\n\nDarbas baigtas..."
// --------------------------------------------------------------------------

/** Tikrina, ar vartotojo pateikti programos argumentai yra teisingi  */
int isArgumentDataCorrect(int argc, const char* argv[]) {
  return argc == 2 ? TRUE : FALSE;
}

/** Laukia vartotojo įvesties */
void waitForUserInteraction() {
  printf(END_OF_WORK);
  std::getchar();
}

/** Suvaldo situaciją, kai nurodytas failas neegzistuoja */
void promptFileDoesNotExist() {

}

/** Nuskaito failą ir gražina simbolių masyvą */
char *initCharArrayFromFile(const char *fileName) {
  // Atidarome failą
  FILE *osFile;
  osFile = fopen(fileName, "r");
  if (osFile == NULL) {
    return NULL;
  }
  
  // Einame į failo pabaigą, paskaičiuojame ilgį
  fseek(osFile, 0L, SEEK_END);
  int elements = ftell(osFile);
  
  // Sukuriame char masyvą tolygų failo dydžiui
  char *dataToReturn = (char *)calloc(elements, sizeof(char));
  
  // Grįžtame į failo pradžią
  fseek(osFile, 0L, SEEK_SET);

  // Nuskaitome visus char'us iš failo į masyvą
  int charCounter = 0;
  char readChar;
  while ((readChar = getc(osFile)) != EOF) {
    dataToReturn[charCounter++] = readChar;
  }  
  
  // Uždarome failą
  fclose(osFile);
  
  // Gražiname rezultatą
  return dataToReturn;
}

/** Pradeda darbą */
int main(int argc, const char *argv[]) {
  // Tikriname parametrus
  if (isArgumentDataCorrect(argc, argv) == FALSE) {
    printf(INCORRECT_PARAMETERS);
	waitForUserInteraction();
	return EXIT_FAILURE;
  }
  
  // Nuskaitome failo duomenis į baitų masyvą
  char *fileByteArray = initCharArrayFromFile(argv[1]);
  if (fileByteArray == NULL) {
    printf(FILE_DOES_NOT_EXIST);
    waitForUserInteraction();
	return EXIT_FAILURE;
  }
	
  // Pagal destytojo ant lentos brezta konspekta:
  // Loader() -> byte[] is failo
  // while (true) {
  //   komandos dekodavimas (atmintis per puslapiavima)
  //   komandos vykdymas (tik CPU, atmintis)
  //   kanalu irenginys
  //   timer
  //   pertraukimu tikrinimas (supervizoriaus rezimas)
  // }

  waitForUserInteraction();
  return EXIT_SUCCESS;
}