
#include"util.h"
// mman library to be used for hugepage allocations (e.g. mmap or posix_memalign only)
#include <sys/mman.h>
#include <string.h>

// [Bonus] TODO: define your own buffer size
#define BUFF_SIZE (1<<21)
//#define BUFF_SIZE 256*1024*2

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
    // [Bonus] TODO:
    // Put your covert channel setup code here

    printf("Please type a message.\n");

    bool sending = true;
    while (sending) {
        char text_buf[128];
        fgets(text_buf, sizeof(text_buf), stdin);
	one_bit = 0; 
        // [Bonus] TODO:
        // Put your covert channel code here
        one_bit = strcmp(text_buf, "0\n");
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
    }

    printf("Sender finished.\n");
    return 0;
}


