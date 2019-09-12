#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "currenttime.h"

void printUsage()
{
    printf("Usage:\n\n");
    printf("vbuild <options>\n");
    printf("\t-incfile [incremental version file]\n");
    printf("\t-template [template version source code file]\n");
    printf("\t-out [version source code output]\n");
    printf("\t-major [major version]\n");
    printf("\t-minor [minor version]\n\n");
}

int main(int argc, char * argv[])
{
    FILE *		fptrRead;
    FILE *      fptrWrite;
    char		szIncrementalVersion[8];
    char        szVersionStr[64];
    char *      pszTimestamp;
    char *      pszIncrementalVersionFileName;
    char *      pszVersionCodeFileName;
    char *      pszTemplateFileName;
    char *      pszTemplateBuffer;
    int			i = 0;
    int         majorVersion = 0;
    int         minorVersion = 0;
    int         incrementalVersion;
    int         filelength;

    if (argc > 5) {
		for (i = 1;i < argc;i++) {
			if (argv[i][0] == '-') {
				if (strncmp(&argv[i][1], "incfile", 7) == 0) {
                    pszIncrementalVersionFileName = strdup(&argv[++i][0]);
				}
				else if (strncmp(&argv[i][1], "out", 3) == 0) {
                    pszVersionCodeFileName = strdup(&argv[++i][0]);
				}
                else if (strncmp(&argv[i][1], "major", 5) == 0) {
                    majorVersion = atoi(&argv[++i][0]);
                }
                else if (strncmp(&argv[i][1], "minor", 5) == 0) {
                    minorVersion = atoi(&argv[++i][0]);
                }
                else if (strncmp(&argv[i][1], "template", 8) == 0) {
                    pszTemplateFileName = strdup(&argv[++i][0]);
                }
                else {
                    printf("Unknown command line option %s\n\n", &argv[++i][0]);
                    printUsage();
                    return -1;
                }
			}
		}
	}
	else {
        printUsage();
		return -1;
	}

    fptrRead = fopen(pszIncrementalVersionFileName, "rt");

    if (fptrRead == NULL) {
        printf("Failed to open incremental version file %s for reading\n\n", pszIncrementalVersionFileName);
        return -1;
    }

    i = 0;

    while (!feof(fptrRead)) {
        szIncrementalVersion[i++] = fgetc(fptrRead);
    }
    szIncrementalVersion[i] = 0;

    fclose(fptrRead);

    fptrWrite = fopen(pszIncrementalVersionFileName, "wt");

    if (fptrWrite == NULL) {
        printf("Failed to open incremental version file %s for writing\n\n", pszIncrementalVersionFileName);
        return -1;
    }

    incrementalVersion = atoi(szIncrementalVersion);
    incrementalVersion++;

    fprintf(fptrWrite, "%03d", incrementalVersion);
    fclose(fptrWrite);

    sprintf(
        szVersionStr, 
        "%d.%d.%03d", 
        majorVersion, 
        minorVersion, 
        incrementalVersion);

    fptrRead = fopen(pszTemplateFileName, "rt");

    if (fptrRead == NULL) {
        printf("Failed to open template code file %s\n\n", pszTemplateFileName);
        return -1;
    }

    fseek(fptrRead, 0L, SEEK_END);
    filelength = ftell(fptrRead);
    rewind(fptrRead);

    pszTemplateBuffer = (char *)malloc(filelength + 1);

    if (pszTemplateBuffer == NULL) {
        printf("Failed to allocate memory for template file %s\n", pszTemplateFileName);
        return -1;
    }

    fread(pszTemplateBuffer, 1, filelength, fptrRead);

    fclose(fptrRead);

    fptrWrite = fopen(pszVersionCodeFileName, "wt");

    if (fptrWrite == NULL) {
        printf("Failed to open code file %s\n\n", pszVersionCodeFileName);
        return -1;
    }

    CurrentTime time;
    pszTimestamp = time.getTimeStamp();

    i = 0;

    while (pszTemplateBuffer[i] != 0) {
        if (strncmp(&pszTemplateBuffer[i], "<BUILD_DATE>", 12) == 0) {
            fwrite(pszTimestamp, 1, strlen(pszTimestamp), fptrWrite);
            i += 12;
        }
        else if (strncmp(&pszTemplateBuffer[i], "<BUILD_VERSION>", 15) == 0) {
            fwrite(szVersionStr, 1, strlen(szVersionStr), fptrWrite);
            i += 15;
        }
        else {
            fputc((int)pszTemplateBuffer[i], fptrWrite);
            i++;
        }
    }

    fclose(fptrWrite);

    free(pszTemplateBuffer);

    return 0;
}