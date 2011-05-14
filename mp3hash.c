/* mp3hash 
 *
 * Quick ugly hack - feel free to improve.
 * FORK ON GITHUB
 */ 
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "md5/md5.h"

#define BUFFERSIZE 1048576

int hash(char* filename);

int main(int argc,char** argv) {
	if(argc!=2) {
		fprintf(stderr,"\
Usage: %s <filename>|@\n\
<filename> = read input from file named <filename>\n\
@          = read filename(s) from STDIN\n\
\n\
Outputs:\n\
mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm lllllllll f\n\
m = md5sum of mpeg data without id3 tags\n\
l = length of id3v2 tag\n\
f = filename\n\
\n\
",argv[0]);
		return 1;
	}
	
	if(!strcmp("@",argv[1])) {
		char filename[1024];
		while(fgets(filename,1024,stdin)) {
			if(filename[strlen(filename)-1]=='\n') filename[strlen(filename)-1]=0;
			hash(filename);
		}
		return 0;
	}
	else {
		return hash(argv[1]);
	}
}

int hash(char* filename) {
	FILE* mp3file;
	if(!(mp3file=fopen(filename,"rb"))) {
		return 2;
	} 
	uint8_t buffer[BUFFERSIZE];
	int len=fread(buffer,1,BUFFERSIZE,mp3file);
	if(!len) {
		fprintf(stderr,"Could not read from file %s\n",filename);
		fclose(mp3file);
		return 3;
	}
	unsigned long id3v2len=0;
	if(!strncmp("ID3",buffer,3)) {
		if(buffer[5]&0x10) {
			id3v2len=20;
		}
		else {
			id3v2len=10;
		}
		id3v2len+=((buffer[6]*128+buffer[7])*128+buffer[8])*128+buffer[9];
	}
	
	unsigned long id3v1len=0;
	fseek(mp3file,-128,SEEK_END);
	len=fread(buffer,1,128,mp3file);
	if(!strncmp("TAG",buffer,3)) {
		id3v1len=128;
	}
	
	unsigned long todo=ftell(mp3file)-id3v2len-id3v1len;
	md5_state_t state;
	md5_init(&state);
	fseek(mp3file,id3v2len,SEEK_SET);
	while(todo) {
		len=fread(buffer,1,(todo>=BUFFERSIZE?BUFFERSIZE:todo),mp3file);
		md5_append(&state,buffer,len);
		todo-=len;
	}
	md5_finish(&state,buffer);
	int i;
	for(i=0;i<16;i++) {
		printf("%02x",buffer[i]);
	}
	printf(" %09lu %s\n",id3v2len,filename);
	
	fclose(mp3file);
	return 0;
}