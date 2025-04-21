#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include <string>

const int CYPHERLENGTH = 71;
const char *PLAIN =  "abcdefghijklmnopq rstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_();:.=-";
const char *CYPHER = "(1cUMvBu;C 5o0NmY=bRJ_xq2ThtEIlgO4r.Xj:yH7aWfPDdAke)nG3pF6QzKVw98SiLsZ-";

/**
 * Takes an array of four bytes and interprets it as a big-endian integer.
 * 
 * @param bytes The byte array.
 * @return      The number that the array represents.
 */
unsigned int bytes_to_size(unsigned char *bytes) {
    return ((bytes[3]<<24) | (bytes[2]<<16) | (bytes[1]<<8) | bytes[0]);
}

/**
 * Decyphers a string of characters.
 * 
 * @param   cyphered        The string of characters to decypher.
 * @param   stringLength    The length of the string.
 * @return                  The string, decyphered.
 */
unsigned char *decypher(unsigned char *cyphered, int stringLength) {
    unsigned char *decyphered = new unsigned char[stringLength];
    for (int i = 0; i < stringLength; i++) {
        for (int j = 0; j < CYPHERLENGTH; j++) {
            if (cyphered[i] == CYPHER[j]) {
                decyphered[i] = PLAIN[j];
            }
        }
    }
    
    return(decyphered);
}

/**
 * Cyphers a string of characters.
 * 
 * @param   string        The string of characters to cypher.
 * @param   stringLength    The length of the string.
 * @return                  The string, cyphered. Again.
 */
unsigned char *recypher(unsigned char *string, int stringLength) {
    unsigned char *cyphered = new unsigned char[stringLength];
    for (int i = 0; i < stringLength; i++) {
        for (int j = 0; j < CYPHERLENGTH; j++) {
            if (string[i] == PLAIN[j]) {
                cyphered[i] = CYPHER[j];
            }
        }
    }
    
    return(cyphered);
}

/**
 * FMWI data file decypher-er and recypher-er.
 * 
 * @param   The first parameter should be the FMWI data file to decypher.
 *          The second parameter should be either c or d.
 *          c means cypher, d means decypher.
 * @return  0 on success, otherwise failure.
 * 
 * @author  Roonespism
 * @since   Jan 2016
 */
int main(int argc, char **argv) {   
    if (argc < 3) {
        printf("Usage: ./FMWICypher originalFile c/d");
        return(-9);
    }
    
    FILE *originalFile = fopen(argv[1], "rb");
    bool decyphering = true;
    char *outFileName = "decyphered.dat";
    if (strcmp(argv[2], "c") == 0) {
        decyphering = false;
        outFileName = "cyphered.dat";
    }
    
    FILE *testNewFile = fopen(outFileName, "r");
    if (testNewFile != NULL) {       
        printf("WARNING: Output file already exists! Overwrite? [y/N] ");
        char ovr = getchar();
        if (ovr != 'y' && ovr != 'Y') {
            return(0);
        }
    }
    
    FILE *newFile = fopen(outFileName, "wb");
    
    //First, check how many filenames we're decyphering.
    unsigned char *numFilesString = new unsigned char[4];
    fread(numFilesString, 4, 1, originalFile);
    unsigned int numFiles = bytes_to_size(numFilesString);
    //printf("%u\n", numFiles);
    
    //Move the original file along to the file list pointer, too.
    fseek(originalFile, 4, SEEK_SET);
    
    //Next is the file list pointer, go to where it's pointing.
    unsigned char *fileListPos = new unsigned char[8];
    fread(fileListPos, 8, 1, originalFile);
    
    double *doublePos = reinterpret_cast<double*>(fileListPos);
    unsigned int intPos = (unsigned int) *doublePos;
    
    //printf("%u\n", intPos);
    
    //Everything before the file list should be left alone, so let's quickly
    //put all that in to the output file.
    fseek(originalFile, 0, SEEK_SET);
    unsigned char *fileData = new unsigned char[intPos];
    fread(fileData, intPos, 1, originalFile);
    fwrite(fileData, intPos, 1, newFile);

    
    //Write file list in to new file, decyphering anything cyphered as we go.
    for (int i = 0; i < numFiles; i++) {
        //Read in file name size, write it.
        unsigned char *fileNameSizeString = new unsigned char[4];
        fread(fileNameSizeString, 4, 1, originalFile);
        unsigned int fileNameSize = bytes_to_size(fileNameSizeString);
        fwrite(fileNameSizeString, 4, 1, newFile);
        
        //Read in file name, cypher or decypher or whatever, write it.
        unsigned char *fileNameCypher = new unsigned char[fileNameSize];
        fread(fileNameCypher, fileNameSize, 1, originalFile);
        unsigned char *fileName;
        if (decyphering) {
            fileName = decypher(fileNameCypher, fileNameSize);
        } else {
            fileName = recypher(fileNameCypher, fileNameSize);
        }
        fwrite(fileName, fileNameSize, 1, newFile);
        
        //Read in internal name size, write it.
        fread(fileNameSizeString, 4, 1, originalFile);
        fileNameSize = bytes_to_size(fileNameSizeString);
        fwrite(fileNameSizeString, 4, 1, newFile);
        
        //Read in internal name (cyphered) and file position pointer.
        //Write both, don't bother decyphering internal name.
        //(Because we don't need it and we'll just end up recyphering it later anyway)
        fileNameCypher = new unsigned char[fileNameSize + 8];
        fread(fileNameCypher, fileNameSize + 8, 1, originalFile);
        fwrite(fileNameCypher, fileNameSize + 8, 1, newFile);
    }
    
    fclose(originalFile);
    fclose(newFile);
}