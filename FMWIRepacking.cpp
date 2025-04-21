//Tool for repacking extracted files into Gensou Shoujo Taisen You data files
//Written by Yam
//Last Modified December 11th by Roonespism
 
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include <sys/stat.h>
 
//Reads 4 bytes (big endian) and returns an int value
inline unsigned int fread_to_size(FILE *readf);
 
//Reads 8 bytes, converts to double, and returns an unsigned int
unsigned int bytes_to_position (FILE *readf);

//Encrypts a file with FMWI's key.
int encrypt(char *inFile, char *outFile) {
    FILE *originalFile = fopen(inFile, "rb");
    char *key = "bV";
    int keyLen = 2;
   
    FILE *newFile = fopen(outFile, "wb");

    unsigned char *currentByte = new unsigned char[1];
    unsigned char *newByte = new unsigned char[1];

    int eofCheck;

    for (int i = 0; true; i++) {
        eofCheck = fread(currentByte, 1, 1, originalFile);
        if (eofCheck != 1) { 
            break;
        }
        
        if (i == keyLen) {
            i = 0;
        }
        
        newByte[0] = currentByte[0]^key[i];
        
        if (newByte[0] == key[i] || newByte[0] == 0 || newByte[0] == ' ' || currentByte[0] == ' ') {
            newByte[0] = currentByte[0];
        }
        
        fwrite(newByte, 1, 1, newFile);
    }

    fclose(newFile);
    
    return(0);
}

//Converts an unsigned int in to 4 bytes (big endian)
unsigned char *size_to_bytes(unsigned int num) {
    unsigned char *szbytes = new unsigned char[4];
    for (int i=0; i<4; i++) {
        szbytes[i] = num >> i*8;
    }
    
    return szbytes;
}
 
//Struct to hold the five parts of a block of filelist data
struct finfoblock {
        unsigned int efnamesize;
        unsigned char *ename;
        unsigned int ifnamesize;
        unsigned char *iname;
        double filepos;
};
 
int main(int argc, char **argv) {
        FILE * infof = fopen(argv[1], "rb");
        //Check for input
        if (infof == NULL) {
                printf("ERROR: No input file!\n");
                return 0;
        }
 
        //Make sure the file doesn't already exist
        FILE * t_outputf = fopen("repacked.dat", "r");
        if (t_outputf != NULL) {      
                printf("WARNING: repacked.dat already exists! Overwrite? [y/N] ");
                char ovr = getchar();
                if (ovr != 'y' && ovr != 'Y') {
                        fclose(t_outputf);
                        return 0;
                }
        }
 
        //Set up output file
        FILE * outputf = fopen("repacked.dat", "wb");
               
        //Check if the input is valid
        unsigned char *hddata = new unsigned char[4];
        fread(hddata, 4, 1, infof);
        if (hddata[0] != 0x47 || hddata[1] != 0x53 || hddata[2] != 0x54 || hddata[3] != 0x59) {
                printf("ERROR: Not a gsty info file!\n");
                return 0;
        }
        delete hddata;
       
        //Get file offset
        unsigned int ofs_files = fread_to_size(infof);
 
        //Get number of files
        fseek(infof, 0, SEEK_CUR);
        unsigned int num_files = fread_to_size(infof)-ofs_files;
 
        //Create list to store the starting positions of file data
        finfoblock *filelist_data = new finfoblock[num_files];
 
        //Write filelist position placeholder
        unsigned char *flistpos = new unsigned char[12];
        for (int a=0; a<12; a++) {
                flistpos[a] = 0x00;
        }
        fwrite(flistpos, 12, 1, outputf);
 
        //Parse filelist data, write files
        for (int i=0; i<num_files+ofs_files; i++) {
                //Get length of next name
                unsigned int fname_size = fread_to_size(infof);
                filelist_data[i].efnamesize = fname_size;
 
                //Get filename
                filelist_data[i].ename = new unsigned char[fname_size+1];
                fread(filelist_data[i].ename, fname_size, 1, infof);
                filelist_data[i].ename[fname_size] = '\0';
                               
                //If the file is before the offset, skip
                if (i >= ofs_files) {
                        //Store starting position of file
                        filelist_data[i].filepos = ftell(outputf);
                        
                        //Encrypt file before adding it
                        encrypt(reinterpret_cast<char*>(filelist_data[i].ename), "currentFile.enc");
                        
                        //Open file, abort if missing
                        FILE * inputf = fopen("currentFile.enc", "rb");
                        if (inputf == NULL) {
                                printf("ERROR: Missing file %s! Aborting!\n", filelist_data[i].ename);
                                remove("output.dat");
                                return 0;
                        }
                       
                        //Get size of file to be added
                        struct stat st;
                        size_t size;
                        int filed = fileno(inputf);
                        fstat(filed, &st);
                        unsigned int input_size = st.st_size;
 
                        //Write size of file
                        unsigned char *szbytes = new unsigned char[4];
                        for (int j=0; j<4; j++) {
                                szbytes[j] = input_size >> j*8;
                        }
                        fwrite(szbytes, 4, 1, outputf);
                        delete szbytes;
 
                        //Write file data
                        unsigned char *fdata = new unsigned char[input_size];
                        fread(fdata, input_size, 1, inputf);
                        fwrite(fdata, input_size, 1, outputf);
                        delete fdata;
 
                        fclose(inputf);
                } else {
                        filelist_data[i].filepos = 0;
                        fseek(infof, fname_size, SEEK_CUR);
                }
               
                //Get the internal filename
                filelist_data[i].ifnamesize = fread_to_size(infof);
                filelist_data[i].iname = new unsigned char[filelist_data[i].ifnamesize];
                fread(filelist_data[i].iname, filelist_data[i].ifnamesize, 1, infof);
 
                //Offset: If the file isn't included in the archive, use the original position
                if (filelist_data[i].filepos == 0) {
                        filelist_data[i].filepos = bytes_to_position(infof);
                } else {
                        fseek(infof, 8, SEEK_CUR);            
                }
        }
       
        //Get position of file list
        unsigned int flist_position = ftell(outputf);
       
        //Write filelist data
        for (int i=0; i<num_files; i++) {
                //Write external filename data
                unsigned char *ex_namesize_bytes = reinterpret_cast<unsigned char*>(&filelist_data[i].efnamesize);
                fwrite(ex_namesize_bytes, 4, 1, outputf);
                fwrite(filelist_data[i].ename, filelist_data[i].efnamesize, 1, outputf);
 
                //Write internal filename data
                unsigned char *in_namesize_bytes = reinterpret_cast<unsigned char*>(&filelist_data[i].ifnamesize);
                fwrite(in_namesize_bytes, 4, 1, outputf);
                fwrite(filelist_data[i].iname, filelist_data[i].ifnamesize, 1, outputf);
 
                //Write file position
                unsigned char *fposition_bytes = reinterpret_cast<unsigned char*>(&filelist_data[i].filepos);
                fwrite(fposition_bytes, 8, 1, outputf);
        }
       
        //Write in number of files, then file list pointer.
        fseek(outputf, 0, SEEK_SET);
        unsigned char *numFilesBytes = size_to_bytes(num_files);
        fwrite(numFilesBytes, 4, 1, outputf);
       
        double lp = (unsigned int)flist_position;
        unsigned char *list_position_bytes = reinterpret_cast<unsigned char*>(&lp);
        fwrite(list_position_bytes, 8, 1, outputf);
       
        printf("Successfully packed %u files!\n", num_files);
 
        fclose(outputf);      
        fclose(infof);
        return 0;
}
 
inline unsigned int fread_to_size(FILE *readf) {
        unsigned char *bytes = new unsigned char[4];
        fread(bytes, 4, 1, readf);
        return ((bytes[3]<<24) | (bytes[2]<<16) | (bytes[1]<<8) | bytes[0]);
        delete bytes;
}
 
unsigned int bytes_to_position (FILE *readf) {
        unsigned char *bytes = new unsigned char[8];
        fread(bytes, 8, 1, readf);
        double temp;
        memcpy(&temp, bytes, 8);
        return ((unsigned int)temp);
}