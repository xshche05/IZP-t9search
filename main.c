#include <stdio.h>

// Basic settings
#define MAX_LENGTH                  100                 // Maximalni pocet znaku pro vstup a pro radku seznamu
#define CONTACT_LIST_MAX_LENGTH     50                  // Maximalni pocet kontaktu ve seznamu

// Kostanty
#define CASE_CHANGE_NUM             32                  // Konstanta pro zmenu velikosti pismen
#define LETTER_TO_NUM               97                  // Konstanta pro prevod pismen na jich poradi v abecede

// ADV. Settings
#define NUMBER_OF_ERRORS_IN_INPUT   1               // Pocet chybnych mezer mezi dvema spravnymi znaky

// Konsolove barvy
#define ERROR_MESSAGE_COLOR     "\x1b[31m"               // Barva chyboveho hlaseni
#define COLOR_RESET             "\x1b[0m"                // Reset barvy

char const transformer[27] = "22233344455566677778889999";    // Transformacni pole
// "22233344455566677778889999"
// "abcdefghijklmnopqrstuvwxyz"

/**
 * @brief Struktura pro uchovani kontaktu
 *
 * @var filled - Zda je kontakt vyplnen, a nebo uz overen a musi byt vypsan
 * @var name - Jmeno kontaktu
 * @var phoneNumber - Telefonni cislo kontaktu
 */
struct contact {
    int filled;
    char name[MAX_LENGTH + 1];
    char phoneNumber[MAX_LENGTH + 1];
};


int error(int code, char *desc)
{
    fprintf(stderr, "%s(%d) Error: %s!%s\n", ERROR_MESSAGE_COLOR, code, desc, COLOR_RESET);
    return code;
}

// check if str contains only numbers
int checkContainsOnlyNumbers(char const *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            return -12;
        }
    }
    return 0;
}

// check if amount of arguments is correct
int checkInputArgumentsAmount(int argc)
{
    if (argc > 2)                                   // Overime pocet argumentu
    {
        return -11;       // Pokud je vice nez 2, tak je to spatny pocet argumentu
    }
    return 0;
}

// copy str from source to destination
void copyStrToStr(const char *from, char *to)
{
    int i = 0;
    while (from[i] != '\0' && from[i] != '\n')
    {
        to[i] = from[i];
        i++;
    }
    to[i] = '\0';
}

// convert uppercase letters to lowercase letters
void toLowerCase(char const *input, char *output)
{
    int i = 0;
    while (input[i] != '\0')
    {
        if (input[i] >= 'A' && input[i] <= 'Z')
        {
            output[i] = input[i] + CASE_CHANGE_NUM;
        }
        else
        {
            output[i] = input[i];
        }
        i++;
    }
    output[i] = '\0';
}

// str has char
int strHasChar(char const *str, char const c, int max_length)
{
    for (int i = 0; i < max_length; i++)
    {
        if (str[i] == c)
        {
            return 1;
        }
    }
    return 0;
}


int checkContactContainsInput(char const *str1, char const *str2)     // Dela tez same co i checkContactPriorityOne ale str1 musi jen obsahovat str2 (obsahuje 3. funkcionalitu)
{
    int found;                          // Flag jestli bylo najdeno str2 ve str1, 1 - str2 je ve str1, 0 - ne
    for (int k = 0; str1[k] != '\0'; k++)
    {
        found = 0;                      // Resetujeme flag
        int i = k;                      // Index pro prochazeni str1
        int j = 0;                      // Index pro prochazeni str2
        int errors = 0;                 // Pocet chbnych useku
        int errorsStack = 0;            // Pocet znaku mezi dvema spravni znaky
        int prev_j = -1;                // Index znáku po 1. chybne mezere
        int correctInLine = 0;          // Pocet spravnych znaku v radku pred chybou
        int was_broke;                  // Flag který se ukazuje jestli cyklus byl zrušen, -1 - ne byl zrušen, 1 - byl zrušen
        while (str1[i] != '\0' && str2[j] != '\0')
        {
            was_broke = -1;             // Resetujeme flag
            if (str1[i] != str2[j])
            {
                if (i == k)                                     // jestli prvni znaky str1 a str2 se nerovna
                {
                    was_broke = 1;
                    break;
                }
                if (errors < NUMBER_OF_ERRORS_IN_INPUT)         // Pocitame cislo chyb v useku
                {
                    errorsStack++;
                    correctInLine = 0;
                    j--;
                }
                else if (errors == NUMBER_OF_ERRORS_IN_INPUT)   // Jestli mame uz jednu chybnu mezeru musime znova zacat hledat znak, ktery byl po prve chybne mezere
                {
                    errorsStack++;
                    j = prev_j - 1;                             // Index znaky po chybne mezere
                    i = i - correctInLine + 1;                  // Index se ktere zacneme hledat znak v str1
                    errors = 0;                                 // Reset pocetu chybnych mezer
                }
                else                                            // Jeslti mame jine problemy
                {
                    was_broke = 1;
                    break;
                }
            }
            else
            {
                correctInLine++;                                // Pocitame spravne znaky v radku
                if (errorsStack > 0)
                {
                    errors++;                                   // Pocitame chybne useky
                    prev_j = j;                                 // Index znaku po skonceni chybne mezery
                    errorsStack = 0;                            // Reset pocetu chyb v mezere
                }
            }
            i++;
            j++;
        }
        if (errorsStack != 0)                                   // Jeslti mame chyby po skonceni cyklusu neni str2 ve str1
        {
            found = 0;                                          // 0 - neni str2 ve str1
        }
        else if (was_broke != 1 && str2[j] == '\0')             // Jeslti nemame chyb, nebyl cyklus zrusen a dosli jsme do posledniho znaku str2
        {
            found = 1;                                          // 1 - je str2 ve str1
            break;
        }
    }
    return found;
}

// read contact list from stdin
int readContactList(struct contact *contactList)    // Nacteme seznam kontaktu
{
    char buffer[MAX_LENGTH + 1];        // Buffer pro nacitani znaku
    int flag = 1;                       // Flag pro overeni jestli je nacteny radek jmene nebo telefonni cislo
    int i = 0;                          // Pocitadlo pro pocet nactenych kontaktu
    while (fgets(buffer, MAX_LENGTH + 2, stdin) != NULL && i < CONTACT_LIST_MAX_LENGTH)
    {
        if (strHasChar(buffer, '\0', MAX_LENGTH + 1) != 1)            // Overeni jestli je nacteny radek delsi nez MAX_LENGTH
        {
            return -21;     // Radek je delsi nez MAX_LENGTH
        }
        if (buffer[0] == '\n')
        {
            return -24;
        }
        if (flag == 1)
        {                                                            // Nacteny radek je jmeno
            flag = 2;
            copyStrToStr(buffer, contactList[i].name);
        }
        else
        {                                                                      // Nacteny radek je telefonni cislo
            flag = 1;
            copyStrToStr(buffer, contactList[i].phoneNumber);
            contactList[i].filled = 1;                                              // Nastavime ze je kontakt plny
            i++;
        }
    }
    if (fgets(buffer, MAX_LENGTH, stdin) != NULL)                  // Overime jestli neni jeste neco v stdin
    {
        return -22;        // Pokud ano, tak je to spatny pocet kontaktu v filu
    }
    if (flag == 2)                                                                  // Jestli flag je 2 tak nacteni bylo ukonceno nactenim jmena
    {
        return -23;        // Pokud ano, tak je to spatny pocet radku v filu, musi byt sude
    }
    return i;                                                                       // Vratime pocet nactenych kontaktu
}

// convert letters to t9 type numbers
int transformElement(char const *inputStr, char *transformedStr)     // Funkce pro prevod jmena na format pro hledani
{
    char lowerCaseStr[MAX_LENGTH + 1];
    toLowerCase(inputStr, lowerCaseStr);                                   // Prevod jmena na mala pismena
    int i = 0;
    while (lowerCaseStr[i] != '\0')
    {
        if (lowerCaseStr[i] >= 'a' && lowerCaseStr[i] <= 'z')       // Pokud je znak pismeno
        {
            transformedStr[i] = transformer[lowerCaseStr[i] - LETTER_TO_NUM];            // Prevod pismena na cislo ('a' - LETTER_TO_NUM = 0; transformedStr[0] = '2')
        }
        else if (lowerCaseStr[i] < 0)
        {
            return -25;
        }
        else                                                                    // Pokud neni znak pismeno
        {
            if (lowerCaseStr[i] == '+')                                                 // Pokud je znak +
            {
                transformedStr[i] = '0';                                       // Nastavime cislo 0
            }
            else                                                                // Pokud neni znak +
            {
                transformedStr[i] = lowerCaseStr[i];                                   // Nastavime znak
            }
        }
        i++;
    }
    transformedStr[i] = '\0';                                                  // Ukonceni retezce
    return 0;
}

// converting list to search form
int transformContactList(struct contact *input, struct contact *output, int contactListLen)    // Funkce pro prevod seznamu kontaktu na format pro hledani
{
    int checkflag = 0;
    for (int i = 0; i < contactListLen; i++)
    {
        checkflag = transformElement(input[i].name, output[i].name);        // Prevod jmena
        if (checkflag < 0) return checkflag;
        checkflag = transformElement(input[i].phoneNumber, output[i].phoneNumber);             // Prevod tel cisla
        if (checkflag < 0) return checkflag;
        output[i].filled = input[i].filled;                                         // Kopirovani flagu
    }
    return checkflag;
}

// get all contacts that contains Input.
void checkAllIfContainsInput(struct contact *contactList, char *inputNum, int *output, int *found, int contactListLen)    // Hledame vsechny kontakty ktere obsahuji zadane cislo
{
    for(int i = 0; i < contactListLen; i++)
    {
        if (contactList[i].filled == 1 && (checkContactContainsInput(contactList[i].phoneNumber, inputNum) == 1
                                           || checkContactContainsInput(contactList[i].name, inputNum) == 1))
        {
            output[*found] = i;             // Ulozime index kontaktu
            (*found)++;                     // Zvysime pocet nalezenych kontaktu
            contactList[i].filled = 2;      // Nastavime flag na 2, kontakt uz byl nalezen
        }
    }
}

// print all found contacts
void printFoundContacts(struct contact *contactList, const int *outList, int found)
{
    if (found == 0)                        // Pokud nebyl nalezen zadny kontakt
    {
        printf("Not found\n");               // Vypiseme, ze nebyl nalezen zadny kontakt
    }
    else                                            // Pokud byl nalezen alespon jeden kontakt
    {
        for (int i = 0; i < found; i++)             // Vypiseme vsechny nalezeny kontakty
        {
            printf("%s, %s\n", contactList[outList[i]].name, contactList[outList[i]].phoneNumber);
        }
    }
}


int main(int argc, char *argv[]) {
    int checkCode = checkInputArgumentsAmount(argc);
    if (checkCode < 0) return error(checkCode, "Input has too many arguments");
    char userInput[MAX_LENGTH + 1];                 // Uzivatelsky vstup
    struct contact contactList[CONTACT_LIST_MAX_LENGTH];        // Seznam kontaktu
    int contactListLen = readContactList(contactList);         // Nacteme seznam kontaktu a ulozime pocet nactenych kontaktu
    checkCode = contactListLen;
    if (checkCode < 0) return error(checkCode, "Seznam file has a problem");
    if (argc < 2)                                   // Pocet argumentu je mensi nez 2
    {
        for (int i = 0; i < contactListLen; i++)    // Nic ne hledame, vypiseme vsechne kontakty
        {
            printf("%s, %s\n", contactList[i].name, contactList[i].phoneNumber);
        }
        return 0;
    }
    checkCode = checkContainsOnlyNumbers(argv[1]);        // Overime, ze uzivatelsky vstup obsahuje pouze cislice
    if (checkCode < 0) return error(checkCode, "Input has to contain only numbers");
    copyStrToStr(argv[1], userInput);               // Zkopirujeme argument do promenne pro uzivatelsky vstup
    struct contact contactListTransformed[contactListLen];    // Seznam kontaktu v transformovanem formatu
    int outList[contactListLen];           // Seznam indexu nalezenych kontaktu
    int found = 0;                                  // Pocet nalezenych kontaktu
    checkCode = transformContactList(contactList, contactListTransformed, contactListLen);     // Prevod seznamu kontaktu na format pro hledani
    if (checkCode < 0) return error(checkCode, "Data in seznam file has to contain only ASCII characters");
    checkAllIfContainsInput(contactListTransformed, userInput, outList, &found, contactListLen);      // Hledame vsechny kontakty ktere obsahuji zadane cislo
    printFoundContacts(contactList, outList, found);    // Vypiseme vsechno co jsme nasli
    return 0;
}
