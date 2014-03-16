// -----------------------------------------------------------------------
//
//
//	Tomas Petras Rupšys ir Dominykas Šiožinis
//	Operacinių Sistemų II-ojo atsiskaitymo realizacija
//
//
// -----------------------------------------------------------------------
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// C boolean apibrėžimas
#define TRUE 1
#define FALSE 0

// Darbo pabaigos tekstas
#define END_OF_WORK "\n\nDarbas baigtas..."

// Klaidų tekstai
#define FILE_FORMAT_ERROR_LENGTH "Klaida! Failo ilgis neatitinka reikalavimu!"
#define FILE_FORMAT_ERROR_BEGINNING "Klaida! Failo pradzios zyme - neteisinga!"
#define FILE_FORMAT_ERROR_ENDING "Klaida! Failo pabaigos zyme - neteisinga!"
#define FILE_FORMAT_ERROR_INCORRECT_LINES_LENGTH "Klaida! Programos maksimalus spausdinamų eiluciu kiekis - neteisingas!"
#define FILE_FORMAT_ERROR_INCORRECT_NAME "Klaida! Programos pavadinimas nenurodytas!"
#define FILE_FORMAT_WORD_LENGTH_INCORRECT "Klaida! Programoje yra netinkamas kiekis simboliu!"
#define INCORRECT_PARAMETERS "Klaida! Neteisingi parametrai!"
#define FILE_DOES_NOT_EXIST "Klaida! Failas neegzistuoja!"

// Atminties spausdinimo tekstai
#define MEMORY_STATUS_TEXT_FIRST_LINE "Atminties bukle:"
#define MEMORY_STATUS_TEXT_WORD_LINE "-as zodis"

// Programos failų formato reikalavimai
#define FILE_FORMAT_BEGINNING "$BEG"
#define FILE_FORMAT_ENDING "$END"
#define FILE_FORMAT_MAX_LENGTH 444
#define FILE_FORMAT_PRINT_LINES_LENGTH 4
#define FILE_FORMAT_PROGRAM_MAX_LINES_FROM 4
#define FILE_FORMAT_PROGRAM_MAX_LINES_TO 7
#define FILE_FORMAT_PROGRAM_NAME_LENGTH 32
#define FILE_FORMAT_PROGRAM_NAME_END_SYMBOL '$'
#define FILE_FORMAT_PROGRAM_NAME_FROM 8
#define FILE_FORMAT_WORD_LENGTH 4

// OS Atminties specifikacija
#define OS_DESIGN_WORDS_AMOUNT 256
#define OS_DESIGN_BYTES_PER_WORD 4
#define OS_DESIGN_EMPTY_WORD_BYTE_SYMBOL '*'

// Atmintis
char memory[OS_DESIGN_WORDS_AMOUNT][OS_DESIGN_BYTES_PER_WORD];

// Virtualios mašinos registras, rodantis į steko viršūnę
char sp;

// Komandų skaitliukas, rodo į sekančia instrukciją
char pc;

/**
 * Užpildo atminties masyvą simboliais, reiškiančiais tuščią baitą.
 */
void initialize_memory() {  
  for (int i=0 ; i<OS_DESIGN_WORDS_AMOUNT ; i++) {
    for (int j=0 ; j<OS_DESIGN_BYTES_PER_WORD ; j++) {
	  memory[i][j] = OS_DESIGN_EMPTY_WORD_BYTE_SYMBOL;
	}
  }
}

/**
 * Išspausdina atminties būklę
 */
void print_memory_status() {
  printf("\n%s\n", MEMORY_STATUS_TEXT_FIRST_LINE);
  for (int i=0 ; i<OS_DESIGN_WORDS_AMOUNT ; i++) {
	for (int j=0 ; j<OS_DESIGN_BYTES_PER_WORD ; j++) {
	  printf("%c", memory[i][j]);
	}
	printf(": %d%s\n", (i+1), MEMORY_STATUS_TEXT_WORD_LINE);
  }
}

/***
 * Tikrina, ar vartotojo pateikti programos argumentai yra teisingi.
 */
int is_argument_data_correct(int argc, const char* argv[]) {
  return argc == 2 ? TRUE : FALSE;
}

/**
 * Laukia vartotojo įvesties.
 */
void wait_for_user_interaction() {
  printf("%s\n", END_OF_WORK);
  std::getchar();
}

/**
 * Tikrina, ar failas atitinka reikalaujamą formatą.
 */
int is_file_of_required_format(char *fileByteArray) {
  // Tikriname masyvo ilgį
  if (sizeof(fileByteArray) > FILE_FORMAT_MAX_LENGTH) {
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

/**
 * Nuskaito failą ir gražina simbolių masyvą. 
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

/**
 * Gražina tekstinę eilutę nurodytuose rėžiuose.
 */
char *substring_from_to(const char *dataLine, const int from, const int to) {
  char *arrayToReturn = (char *)calloc(to - from + 2, sizeof(char));

  for (int i=from,j=0 ; i<=to ; i++,j++) {
	arrayToReturn[j] = dataLine[i];
	
	if (i == to) {
	  arrayToReturn[j+1] = '\0';
	}
  }

  return arrayToReturn;
}

/**
 * Gražina tekstinę eilutę nuo nurodyto rėžio iki nurodyto simbolio.
 */
char *substring_from_until_symbol(const char *dataLine, const int from, const char endSymbol, const int maxLength) {
  char *arrayToReturn = (char *)calloc(maxLength, sizeof(char));
  int dataArrayIteratorIndex = from;
  int newArrayIteratorIndex = 0;
  
  do {
    char symbol = dataLine[dataArrayIteratorIndex];
	if (symbol == endSymbol) {
	  break;
	}
	
	arrayToReturn[newArrayIteratorIndex++] = symbol;
	dataArrayIteratorIndex++;
	
  } while (dataArrayIteratorIndex > 0 && dataArrayIteratorIndex <= maxLength);
  arrayToReturn[newArrayIteratorIndex] = '\0';
  
  arrayToReturn = (char *)realloc(arrayToReturn, newArrayIteratorIndex);
  return arrayToReturn;
}

/**
 * Pagal įvesties duomenis, vykdo programą operacinėjė sistemoje.
 */
int initialize_given_program(const char *fileByteArray) {
  // Išgauname maksimalų spausdinamų eilučių kiekį
  const int maximumLinesToPrint = atoi(substring_from_to(fileByteArray, FILE_FORMAT_PROGRAM_MAX_LINES_FROM, 
    FILE_FORMAT_PROGRAM_MAX_LINES_TO));
  if (maximumLinesToPrint == 0) {
    printf("%s\n", FILE_FORMAT_ERROR_INCORRECT_LINES_LENGTH);
    return FALSE;
  }
	
  // Išgauname programos pavadinimą
  const char *programName = substring_from_until_symbol(fileByteArray, FILE_FORMAT_PROGRAM_NAME_FROM, 
    FILE_FORMAT_PROGRAM_NAME_END_SYMBOL, FILE_FORMAT_PROGRAM_NAME_LENGTH);

  if (programName == NULL) {
    printf("%s\n", FILE_FORMAT_ERROR_INCORRECT_NAME);
    return FALSE;
  }
  
  // Išgauname pačią programą į simbolių masyvą
  int programCodeLength = strlen(fileByteArray) - strlen(FILE_FORMAT_BEGINNING) - 
    strlen(FILE_FORMAT_ENDING) - FILE_FORMAT_PROGRAM_MAX_LINES_FROM - strlen(programName);
  
  if (programCodeLength % FILE_FORMAT_WORD_LENGTH != 0) {
	printf("%s\n", FILE_FORMAT_WORD_LENGTH_INCORRECT);
	return FALSE;
  }
  
  int programCodeStartIndex = strlen(FILE_FORMAT_BEGINNING) + 
    (FILE_FORMAT_PROGRAM_MAX_LINES_TO - FILE_FORMAT_PROGRAM_MAX_LINES_FROM + 1) +
	strlen(programName);

  const char* programCode = substring_from_to(fileByteArray, programCodeStartIndex, 
    programCodeStartIndex + programCodeLength - 1);

  // Iš programos simbolių masyvo, sukuriame programos žodžių 2d masyvą
  const int wordsAmount = programCodeLength / FILE_FORMAT_WORD_LENGTH;
  char programCodeLines[wordsAmount][FILE_FORMAT_WORD_LENGTH + 1];
  
  int lineNo = 0;
  for (int i=0 ; i<strlen(programCode) ; i=i+FILE_FORMAT_WORD_LENGTH) {
    for (int j=0 ; j<FILE_FORMAT_WORD_LENGTH ; j++) {
	  programCodeLines[lineNo][j] = programCode[i+j];
	}

	programCodeLines[lineNo][FILE_FORMAT_WORD_LENGTH] = '\0';
	lineNo++;
  }
  
  // Iteruojame per kiekvieną programos eilutę
  for (int i=0 ; i<lineNo ; i++) {
    printf("%s\n", programCodeLines[i]);
    //   komandos dekodavimas (atmintis per puslapiavima)
    //   komandos vykdymas (tik CPU, atmintis)
    //   kanalu irenginys
    //   timer
    //   pertraukimu tikrinimas (supervizoriaus rezimas)
  }

  // Jeigu neįvyko problemų - gražiname sėkmės reikšmę
  return TRUE;
}

/**
 * Pradeda darbą. 
 */
int main(int argc, const char *argv[]) {
  // Tikriname vartotojo įvestus parametrus
  if (is_argument_data_correct(argc, argv) == FALSE) {
    printf("%s\n", INCORRECT_PARAMETERS);
	wait_for_user_interaction();
	return EXIT_FAILURE;
  }
  
  // Nuskaitome failo duomenis į baitų masyvą
  const char *fileByteArray = init_char_array_from_file(argv[1]);
  if (fileByteArray == NULL) {
    wait_for_user_interaction();
	return EXIT_FAILURE;
  }
	
  // Įdiegiame operacinės sistemos atmintį
  initialize_memory();
  
  // Spausdiname atminties būklę
  print_memory_status();
	
  // Vykdome programą
  int flowSuccess = initialize_given_program(fileByteArray);
  
  // Užbaigiame darbą
  wait_for_user_interaction();
  return flowSuccess == TRUE ? EXIT_SUCCESS : EXIT_FAILURE;
}