#include <TXLib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INPUT_FILE_NAME  "oldOnegin.txt"
#define OUTPUT_FILE_NAME "newOnegin.txt"
#define POISON_VALUE -13
#define ANSWER_LENGTH 4

struct textInfo
{
    char *fileName;
    int  symbolCount;
    int  linesCount;
    char *buffer;
};

int   makeIndexArr                   (const struct textInfo *const info, char *index[]);
int   CompareStringFuncAlfaFirst     (const void *firstString, const void *secondString);
int   CompareStringFuncAlfaLast      (const void *firstString, const void *secondString);
int   CompareStringByPointer         (const void *firstString,const void *secondString);
int   ThreeWayComparison             (const char *arg1, const char *arg2);
int   FileWrite                      (const char fileName[], const char *const index[], const int linesCount, const char printReason[]);
int   getLinesCount                  (char text[],const int symbolCount);
int   getText                        (struct textInfo *info, const char fileName[]);
void  swap                           (char  **string1, char **string2);
void  readFromFile                   (struct textInfo *info, FILE* file);
char* getLine                        ();




int main(void)
{
    struct textInfo info = {};

    printf("Write file's name for writing sorted text or 0 to use standard file name 'newOnegin.txt'\n");
    char* fileName = getLine();

    if (fileName != NULL)
    {
        if(strcmp(fileName, "0") != 0)
        {
            printf("Your files's name for writing: %s\n", fileName);
            int checker = getText(&info, INPUT_FILE_NAME);

            char **const index = (char**) malloc(info.linesCount * sizeof(char*));
            if (index == NULL)
            {
                printf("Cannot open file");
                return POISON_VALUE;
            }
            makeIndexArr(&info, index);

            qsort(index, info.linesCount, sizeof(*index), CompareStringFuncAlfaFirst);
            FileWrite(fileName, index, info.linesCount, "ALPHABETICAL SORTING (FIRST CHARACTER)");

            qsort(index, info.linesCount, sizeof(*index), CompareStringFuncAlfaLast);
            FileWrite(fileName, index, info.linesCount, "ALPHABETICAL SORTING (LAST CHARACTER)");

            qsort(index, info.linesCount, sizeof(*index), CompareStringByPointer);
            FileWrite(fileName, index, info.linesCount, "NORMAL SORTING");
        }
        else
        {
            int checker = getText(&info, INPUT_FILE_NAME);

            char **const index = (char**) malloc(info.linesCount * sizeof(char*));
            if (index == NULL)
            {
                printf("Cannot open file");
                return POISON_VALUE;
            }
            makeIndexArr(&info, index);


            qsort(index, info.linesCount, sizeof(*index), CompareStringFuncAlfaFirst);
            FileWrite(OUTPUT_FILE_NAME, index, info.linesCount, "ALPHABETICAL SORTING (FIRST CHARACTER)");

            qsort(index, info.linesCount, sizeof(*index), CompareStringFuncAlfaLast);
            FileWrite(OUTPUT_FILE_NAME, index, info.linesCount, "ALPHABETICAL SORTING (LAST CHARACTER)");

            qsort(index, info.linesCount, sizeof(*index), CompareStringByPointer);
            FileWrite(OUTPUT_FILE_NAME, index, info.linesCount, "NORMAL SORTING");
        }
    }
    /*
    char* text = getLine();
    if (text)
    {
        printf("%s\n", text);
    }
    */
    return 0;
}




int makeIndexArr(const struct textInfo *const info, char *index[])
{
    if (info == NULL || info->buffer == NULL)
    {
        printf("Cannot open file");
        return POISON_VALUE;
    }
    int nLines = 1;
    index[0] = &(info->buffer[0]);
    for (int i = 1; i < info->symbolCount; i++)
    {
        if (info->buffer[i] == '\r')
        {
            info->buffer[i] = '\0';
        }
        else if (info->buffer[i] == '\n')
        {
            info->buffer[i] = '\0';
            index[nLines] = &(info->buffer[i + 1]);
            nLines++;
        }
    }

    return 1;
}






int CompareStringFuncAlfaFirst(const void *const firstString, const void *const secondString)
{
    const char* strLine1 = *((const char*const *) firstString);
    const char* strLine2 = *((const char*const *) secondString);

    int symbolCount1 = 0;
    int symbolCount2 = 0;

    while((strLine1[symbolCount1] != '\0') && (!isalpha(strLine1[symbolCount1])))
        symbolCount1++;
    while((strLine2[symbolCount2] != '\0') && (!isalpha(strLine2[symbolCount2])))
        symbolCount2++;
    while (true)
    {
        if (strLine1[symbolCount1] == '\0' || strLine2[symbolCount2] == '\0')
            return strLine1[symbolCount1] - strLine2[symbolCount2];

        if (isalpha(strLine1[symbolCount1]) && isalpha(strLine2[symbolCount2]))
        {
            char symb1 = (char) tolower(strLine1[symbolCount1]);
            char symb2 = (char) tolower(strLine2[symbolCount2]);
            if (symb1 != symb2)
                return symb1 - symb2;
        }

        symbolCount1++;
        while((strLine1[symbolCount1] != '\0') && (!isalpha(strLine1[symbolCount1])))
            symbolCount1++;

        symbolCount2++;
        while((strLine2[symbolCount2] != '\0') && (!isalpha(strLine2[symbolCount2])))
            symbolCount2++;
    }
}


int CompareStringFuncAlfaLast(const void *const firstString, const void *const secondString)
{
    const char* strLine1 = *((const char*const *) firstString);
    const char* strLine2 = *((const char*const *) secondString);


    size_t countSymb1 = strlen(strLine1) - 1;
    size_t countSymb2 = strlen(strLine2) - 1;

    size_t counter1 = countSymb1;
    size_t counter2 = countSymb2;

    while (counter1 > 0 && !isalpha(strLine1[counter1]))
        counter1--;

    while (counter2 > 0 && !isalpha(strLine2[counter2]))
        counter2--;

    while (true)
    {
        char symb1 = (char) tolower(strLine1[counter1]);
        char symb2 = (char) tolower(strLine2[counter2]);
        if (symb1 != symb2)
            return symb1 - symb2;

        if (counter1 == 0 || counter2 == 0)
            return countSymb1 > countSymb2 ? 1 : (countSymb1 == countSymb2 ? 0 : -1);

        counter1--;
        while (counter1 > 0 && !isalpha(strLine1[counter1]))
            counter1--;

        counter2--;
        while (counter2 > 0 && !isalpha(strLine2[counter2]))
            counter2--;
    }


}



int CompareStringByPointer(const void *const firstString, const void *const secondString)
{
    const char *firstAddress = *((const char *const *) firstString);
    const char *secondAddress = *((const char *const *) secondString);

    return ThreeWayComparison(firstAddress, secondAddress);
}


int ThreeWayComparison(const char *arg1, const char *arg2)
{
    return (arg1 > arg2 ? 1 : (arg1 == arg2 ? 0 : -1));
}

int FileWrite(const char fileName[], const char *const index[], const int linesCount, const char printReason[])
{
    FILE *file = 0;
    if ((file = fopen(fileName, "ab")) == NULL)
    {
        printf("Cannot open file");
        return POISON_VALUE;
    }
    fprintf(file, "%s\n\n", printReason);
    for(int i = 0; i < linesCount; i++)
    {
        fprintf(file, "%s\n", index[i]);
    }
    fprintf(file, "\n\n\n\n\n");
    fclose(file);
    printf("Success\n");
    return 0;
}

int getLinesCount(char text[], const int symbolCount)
{
    int lineCount = 0;
    for(int i = 0; i < symbolCount; i++)
    {
        if(text[i] == '\r')
            text[i] = '\0';
        if(text[i] == '\n')
            lineCount++;

    }
    return lineCount;
}


int getText(struct textInfo *const info, const char fileName[])
{
    FILE *file = nullptr;
    if ((file = fopen(fileName, "rb")) != NULL)
    {
        struct stat buff = {};
        stat(fileName, &buff);

        info->symbolCount = buff.st_size;

        readFromFile(info, file);


        info->linesCount = getLinesCount(info->buffer, info->symbolCount);
        fclose(file);
        return 1;
    }
    else
        printf("Cannot open file");

    return POISON_VALUE;
}

void swap(char **const string1, char **const string2)
{
    char* temp = *string2;
    *string2 = *string1;
    *string1 = temp;
}

void readFromFile(struct textInfo *const info, FILE* const file)
{
    info->buffer = (char*) malloc(info->symbolCount + 1);
    if(info->buffer == NULL)
    {
        printf("Problems with memory");
        return;
    }
    fread(info->buffer, 1, info->symbolCount, file);
    info->buffer[info->symbolCount] = '\0';
}


char* getLine()
{
    int size = 10;
    int len = 0;
    char* text = (char*) malloc(size);
    if (text == NULL)
    {
        printf("Memory error");
        return NULL;
    }
    int c = 0;
    while((c = getchar()) != '\n')
    {
        if (len + 1 < size)
            text[len] = (char)c;
        else
        {
            size*=2;
            text = (char*) realloc(text, size );
            if (text == NULL)
            {
                printf("Memory error");
                free(text);
                return NULL;
            }
            text[len] = (char)c;
        }
        len++;
    }
    text = (char*) realloc(text, len + 1);
    text[len] = '\0';
    return text;
}
//TODO: arguments of cmd
