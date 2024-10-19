
#include"util.h"
// mman library to be used for hugepage allocations (e.g. mmap or posix_memalign only)
#include <sys/mman.h>
#include <string.h>
#include <math.h> 

// [Bonus] TODO: define your own buffer size
//#define BUFF_SIZE (1<<21)
#define BUFF_SIZE 256*1024

int extract_numerical_value(const char *char_array) {
    int length = strlen(char_array);

    // Ensure the character array is not empty
    if (length == 0) {
        printf("Error: Empty character array.\n");
        return -1;
    }

    // Check if the first character is a digit
    if (char_array[0] < '0' || char_array[0] > '9') {
        printf("Error: Invalid character at the beginning.\n");
        return -1;
    }

    // Convert the numerical part of the string to an integer
    int value = atoi(char_array);

    // Check if the value is within the valid range (0 to 255)
    if (value < 0 || value > 255) {
        printf("Error: Value out of range (0-255).\n");
        return -1;
    }

    return value;
}

int main(int argc, char **argv)
{
    // Allocate a buffer using huge page
    // See the handout for details about hugepage management
    void *buf= mmap(NULL, BUFF_SIZE, PROT_READ | PROT_WRITE, MAP_POPULATE |
                    MAP_ANONYMOUS | MAP_PRIVATE | MAP_HUGETLB, -1, 0);
    
    if (buf == (void*) - 1) {
        perror("mmap() error\n");
        exit(EXIT_FAILURE);
    }
    // The first access to a page triggers overhead associated with
    // page allocation, TLB insertion, etc.
    // Thus, we use a dummy write here to trigger page allocation
    // so later access will not suffer from such overhead.
    *((char *)buf) = 1; // dummy write to trigger page allocation
    
    
    
    volatile uint8_t *eviction_buffer = (uint8_t *)malloc(BUFF_SIZE);
    //uint64_t latency[256*4] = {0};  
    int one_bit = 0; 
    int tmp;
    int set_no=0;  
    int index = 0; 
    //uint64_t binary[8] = {0};
    int len = 128; 
    char *binary = malloc(len * 8 + 1);
    // [Bonus] TODO:
    // Put your covert channel setup code here

    printf("Please type a message.\n");
	char text_buf[128];
        fgets(text_buf, sizeof(text_buf), stdin);
        set_no = extract_numerical_value(text_buf);

	
    bool sending = true;
    while (sending) {
        //char text_buf[128];
        //fgets(text_buf, sizeof(text_buf), stdin);
	//one_bit = 0; 
        // [Bonus] TODO:
        // Put your covert channel code here
//	*binary = string_to_binary(text_buf);
//       printf("\nbinary:%d\n",*binary);

/*       for(int i =0; i<8;i++){
       		set_no = set_no + binary[i]*(pow(2,i)); //map the 8 bit number from one of set 0 to 255 
       }
*/
	//set_no = extract_numerical_value(text_buf);	
	//printf("\n%d\n",set_no);
        for (int i = 0; i<4; i++){ // filling all 4 ways of that set
	       for (int j = 0; j<4; j++) {
		eviction_buffer[((set_no+256*i)*64) + 1024*64*j] = 1; // since there are 1024 sets in L2, fill total 4 lines of  4 sets each that correspond to set no. or set_no + multiple of 256
       }
       }

	    /*
	    for (int i =0; i< 4; i++) {
		eviction_buffer[(set_no*64) + 1024*64*i]; 
	    }
*/

       /* one_bit = strcmp(text_buf, "0\n");
	if(one_bit == 0){
		printf("IT IS 0");
		// evict entire L2
		for (int k = 0; k < 50; k++) {
		for(int i = 0; i < BUFF_SIZE*1.9/64; i++){
			
			tmp = eviction_buffer[i*64]; 		
		}
		}
	}
	else{
		//evict only 25% of L2 
		for (int i =0; i < BUFF_SIZE/(64*4); i++){
			tmp = eviction_buffer[i*64]; 
		}
	}
	sending = false;
       */ 	
       //sending = false; 
    }

    printf("Sender finished.\n");
    return 0;
}


