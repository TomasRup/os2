//
//
//	Tomas Petras Rupšys ir Dominykas Šiožinis
//	Operacinių Sistemų II-ojo atsiskaitymo realizacija
//
//
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Konstantos
#define TRUE 1
#define FALSE 0
#define INCORRECT_PARAMETERS "Klaida! Neteisingi parametrai!"
#define FILE_DOES_NOT_EXIST "Klaida! Failas neegzistuoja!"
#define END_OF_WORK "\n\nDarbas baigtas..."

#define FILE_FORMAT_ERROR_LENGTH "Klaida! Failo ilgis neatitinka reikalavimų!"
#define FILE_FORMAT_ERROR_BEGINNING "Klaida! Failo pradžios žymė - neteisinga!"
#define FILE_FORMAT_ERROR_ENDING "Klaida! Failo pabaigos žymė - neteisinga!"
#define FILE_FORMAT_BEGINNING "$BEG"
#define FILE_FORMAT_ENDING "$END"
#define FILE_FORMAT_LENGTH 444

/** 
 * Tikrina, ar vartotojo pateikti programos argumentai yra teisingi  
 */
int is_argument_data_correct(int argc, const char* argv[]) {
  return argc == 2 ? TRUE : FALSE;
}

/*
 * Laukia vartotojo įvesties 
 */
void wait_for_user_interaction() {
  printf("%s\n", END_OF_WORK);
  std::getchar();
}

/*
 * Tikrina, ar failas atitinka reikalaujamą formatą
 */
int is_file_of_required_format(char *fileByteArray) {
  // Tikriname masyvo ilgį
  if (sizeof(fileByteArray) > FILE_FORMAT_LENGTH) {
	printf("%s\n", FILE_FORMAT_ERROR_LENGTH);
	return FALSE;
  }
  
  // Tikriname pradžios žymę
  int actualFormatBeginningLength = strlen(FILE_FORMAT_BEGINNING);
  if (strncmp(fileByteArray, FILE_FORMAT_BEGINNING, actualFormatBeginningLength) != 0) {
    printf("%s\n", FILE_FORMAT_ERROR_BEGINNING);
	return FALSE;
  }
  
  // Tikriname pabaigos žymę
  int actualFormatEndingLength = strlen(FILE_FORMAT_ENDING);
  char *lastSymbols = (char *)calloc(actualFormatEndingLength, sizeof(char));
  for (int i=0,j=(strlen(fileByteArray) - actualFormatEndingLength) ; i<actualFormatEndingLength ; i++,j++) {
	lastSymbols[i] = fileByteArray[j];
  }
  
  if (strcmp(lastSymbols, FILE_FORMAT_ENDING) != 0) {
	printf("%s\n", FILE_FORMAT_ERROR_ENDING);
	free(lastSymbols);
	return FALSE;
  }
  free(lastSymbols);
  
  // Jeigu viskas gerai, gražiname TRUE
  return TRUE;
}

/*
 * Nuskaito failą ir gražina simbolių masyvą 
 */
char *init_char_array_from_file(const char *fileName) {
  // Atidarome failą
  FILE *osFile;
  osFile = fopen(fileName, "r");
  if (osFile == NULL) {
	printf("%s\n", FILE_DOES_NOT_EXIST);
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
  
  // Tikriname, ar failas atitinka formatą
  if (is_file_of_required_format(dataToReturn) == FALSE) {
	return NULL;
  }
  
  // Gražiname rezultatą
  return dataToReturn;
}

/*
 * Pradeda darbą 
 */
int main(int argc, const char *argv[]) {
  // Tikriname parametrus
  if (is_argument_data_correct(argc, argv) == FALSE) {
    printf("%s\n", INCORRECT_PARAMETERS);
	wait_for_user_interaction();
	return EXIT_FAILURE;
  }
  
  // Nuskaitome failo duomenis į baitų masyvą
  char *fileByteArray = init_char_array_from_file(argv[1]);
  if (fileByteArray == NULL) {
    wait_for_user_interaction();
	return EXIT_FAILURE;
  }
	

	
  // while (true) {
  //   komandos dekodavimas (atmintis per puslapiavima)
  //   komandos vykdymas (tik CPU, atmintis)
  //   kanalu irenginys
  //   timer
  //   pertraukimu tikrinimas (supervizoriaus rezimas)
  // }
  
  

  // Užbaigiame darbą
  wait_for_user_interaction();
  return EXIT_SUCCESS;
}