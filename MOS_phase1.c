//MOS_phase1.c
#include<stdio.h>
#include<string.h>

int IC, C;
char IR[4], R[4];
int SI;
char M[100][4];
char buffer[41];
int i = 0, loc;
int t = 0, a = 0;
int j;

FILE* input, * output;

void EXECUTEUSERPROGRAM();
void LOAD();
void MOS();
void START();
void READ();
void WRITE();
void TERMINATE();
void INIT();
void clearbuff();

int main()
{
    //Open read and write files
    input = fopen("inputfile.txt", "r");
    output = fopen("outputfile.txt", "w");
    LOAD();
    return 0;
}


void EXECUTEUSERPROGRAM()
{
    for (i = 0;i < 4;i++)
    {
        IR[i] = M[IC][i];
    }

    loc = ((int)IR[2] - 48) * 10 + ((int)IR[3] - 48);

    IC++;

    if (IR[0] == 'G' && IR[1] == 'D')
    {
        SI = 1;
        MOS();
    }

    else if (IR[0] == 'P' && IR[1] == 'D')
    {
        SI = 2;
        /*printf("\nIN PD\n");
        for (i = loc;i < loc + 10;i++)
        {
            printf("M[%d] : ", i);
            for (j = 0;j < 4;j++)
            {
                printf("%c", M[i][j]);
            }
            printf("\n");
        }*/
        MOS();
    }

    else if (IR[0] == 'H')
    {
        SI = 3;

        MOS();
    }

    else if (IR[0] == 'L' && IR[1] == 'R')
    {
        for (i = 0;i < 4;i++)
        {
            R[i] = M[loc][i];
        }
        EXECUTEUSERPROGRAM();
    }

    else if (IR[0] == 'S' && IR[1] == 'R')
    {
        printf("\nIN SR\n");
        printf("\nmemory stored with data-->>");
        for (i = 0;i < 4;i++)
        {
            M[loc][i] = R[i];
            printf("%c", M[loc][i]);
        }
        EXECUTEUSERPROGRAM();
    }

    else if (IR[0] == 'C' && IR[1] == 'R')
    {
        for (i = 0;i < 4;i++)
        {
            if (R[i] == M[loc][i])
            {

                C = 1;
            }
            else
            {
                C = 0;
                break;
            }
        }
        EXECUTEUSERPROGRAM();
    }

    else if (IR[0] == 'B' && IR[1] == 'T')
    {
        if (C == 1)
        {
            IC = loc;

        }

        EXECUTEUSERPROGRAM();
    }

}

void INIT()
{
    int i, j;
    for (i = 0;i < 100;i++)
    {
        for (j = 0;j < 4;j++)
        {
            M[i][j] = '\0';
        }
    }

    for (i = 0;i < 4;i++)
    {
        IR[i] = R[i] = '\0';
    }

}

void READ()
{
    int i, j, k;

    if (fgets(buffer, 41, input) != NULL)
    {

        k = 0;
        for (i = loc;i < loc + 10;i++)
        {
            //printf("M[%d] : ", i);
            for (j = 0;j < 4 && buffer[k] != '\0';j++)
            {
                M[i][j] = buffer[k];
                k++;

                printf("%c", M[i][j]);
            }
            printf("\n");
        }

    }
    EXECUTEUSERPROGRAM();
}

void clearbuff()
{
    int v;
    for(v = 0;v < 41;v++)
    {
        buffer[v] = NULL;
    }
}


void WRITE()
{
    int i, j, k;
    IR[3] = '0';
    k = 0;
    printf("\n");
    clearbuff();
    for (i = loc; i < loc + 10; i++)
    {
        for (j = 0; j < 4 && M[i][j] != '$'; j++)
        {
            if (M[i][j] == '\n')
            {
                buffer[k] = '\n';
            }
            else
                buffer[k] = M[i][j];
            printf("%c", buffer[k]);
            k++;
        }
    }
    buffer[k] = '\n';
    //printf("\nString is\n");
    /*int v;
    for (v = 0;v < 41;v++)
    {
        printf("%c", buffer[v]);
    }*/
    fputs(buffer, output);
    EXECUTEUSERPROGRAM();
}

void TERMINATE()
{

    memset(buffer, 0, 41);
    buffer[0] = '\n';
    buffer[1] = '\n';
    fputs(buffer, output);
}

void MOS()
{
    switch (SI)
    {
    case 1:
        READ();
        break;
    case 2:
        WRITE();
        break;
    case 3:
        TERMINATE();
        break;
    default:
        SI = 0;
    }
}

void START()
{
    IC = 0;
    EXECUTEUSERPROGRAM();
}

void LOAD()
{
    int t = 0;
    int i, j, k;
    while (getc(input) != EOF)
    {
        fseek(input, -1, SEEK_CUR);     /*int fseek(FILE *stream, long int offset, int whence)*/
        memset(buffer, 0, 42);          /*void *memset(void *str, int c, size_t n)*/
        fgets(buffer, 42, input);

        if (buffer[0] == '$' && buffer[1] == 'A' && buffer[2] == 'M' && buffer[3] == 'J')
        {
            t = 0;
            INIT();
        }
        else if (buffer[0] == '$' && buffer[1] == 'D' && buffer[2] == 'T' && buffer[3] == 'A')
        {
            START();
        }

        else if (buffer[0] == '$' && buffer[1] == 'E' && buffer[2] == 'N' && buffer[3] == 'D')
            { continue; }

        else
        {
            int k;
            for (k = t;k < t + 10;k++)
            {
                printf("M[%d]:", k);
                int i;
                for (i = 0;i < 4;i++)
                {
                    M[k][i] = buffer[a];
                    a++;
                    printf("%c", M[k][i]);
                }
                printf("\n");
            }
            t = t + 10;
            a = 0;
        }
    }
    fclose(input);
    fclose(output);
}
