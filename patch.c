// readelf -x .text p
// objdump -S p -Mintel
// hexdump -e '16/1 "%02x " "\n"' -v p

// c7 45 fc 00

#include <stdio.h>
#include <stdlib.h>

int ab_strcmp(const char* a, const char* b) {
	while (*b) if (*a++ != *b++) return 0;
	return 1;
}

int main(int argc, char** argv) {
	printf("memory patcher v1.0 by 1eht~rampnup-mirpec\n");
	printf("filename: ");
	char s_filename[8] = {};
	scanf("%s", s_filename);
	printf("reading..");
	char *Result = 0;
    FILE *File = fopen(s_filename, "rb");
    size_t FileSize = 0;
    if (File) {
        fseek(File, 0, SEEK_END);
        FileSize = ftell(File);
        fseek(File, 0, SEEK_SET);

        Result = (char *)malloc(FileSize + 1);
        fread(Result, FileSize, 1, File);
        Result[FileSize] = 0;
        fclose(File);
    }
    printf("done.\n");
    int s_inst, s_index, s_value;
    printf("query(hex): ");
    scanf("%x", &s_inst);

	int IsElf = ab_strcmp(Result + 1, "ELF");
	if (!IsElf) { printf("Invalid ELF file.\n"); return 0; }
	int i;
	for (i = 0; i < FileSize - 3; i++) {
		if (((unsigned int)*(Result + i + 0) & 0xff) == s_inst) {
			int start = i;
			printf("(%04d) ", start);
			for (; i < start + 8; i++) {
				printf("%02x ", *(Result + i) & 0xff);
			}
			printf("\n");
		}
		/*if (*(Result + i + 0) == '\xc7' && *(Result + i + 1) == '\x45' && *(Result + i + 2) == '\xfc') {
			printf("Found memory at %d!\n", i);
			printf("4th byte: %x\n", *(Result + i + 3));
			*(Result + i + 3) = 0x1;
			// printf("4th byte: %02x\n", *(Result + i + 3) & 0xff);
		}*/
		// printf("%02x ", *(Result + i) & 0xff);
	}
	printf("set index: ");
	scanf("%d", &s_index);
	printf("to ");
	scanf("%d", &s_value);
	*(Result + s_index) = s_value;
	File = fopen("p", "wb");
	fwrite(Result, sizeof(char), FileSize, File);
	fclose(File);
	return 0;
}

/*40052d:	55                   	push   rbp
  40052e:	48 89 e5             	mov    rbp,rsp
  400531:	48 83 ec 20          	sub    rsp,0x20
  400535:	89 7d ec             	mov    DWORD PTR [rbp-0x14],edi
  400538:	48 89 75 e0          	mov    QWORD PTR [rbp-0x20],rsi
  40053c:	c7 45 fc 01 00 00 00 	mov    DWORD PTR [rbp-0x4],0x1
  400543:	83 7d fc 00          	cmp    DWORD PTR [rbp-0x4],0x0
  400547:	74 0a                	je     400553 <main+0x26>
  400549:	bf e4 05 40 00       	mov    edi,0x4005e4
  40054e:	e8 bd fe ff ff       	call   400410 <puts@plt>
  400553:	c9                   	leave  
  400554:	c3                   	ret    
  400555:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
  40055c:	00 00 00 
  40055f:	90                   	nop*/