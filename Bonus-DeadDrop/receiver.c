
#include"util.h"
// mman library to be used for hugepage allocations (e.g. mmap or posix_memalign only)
#include <sys/mman.h>
#define BUFF_SIZE (256*1024)
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



    // [Bonus] TODO: Put your covert channel setup code here
    volatile uint8_t *eviction_buffer = (uint8_t *)malloc(BUFF_SIZE);
    int tmp;
    int total = 0; 
    uint64_t latency = 0;
   // uint64_t latency_freq[BUFF_SIZE/64] = {0}; 
    uint64_t latency_sum = 0; 
    printf("Please press enter.\n");

    char text_buf[2];
    fgets(text_buf, sizeof(text_buf), stdin);

    printf("Receiver now listening.\n");
    for(int k =0 ; k < 1; k++){
    for (int i = 0; i < BUFF_SIZE/64; i++) {
                 eviction_buffer[i*64]=1;    
        }       
    }
    bool listening = true;
    int iter = 1; 
    while (listening) {
	//latency_sum = 0; 
        // [Bonus] TODO: Put your covert channel code here
	//uint64_t latency_freq[BUFF_SIZE/64] = {0};

    	iter = ~iter;
	
	if(iter == 1){
	for (int i = 0; i < BUFF_SIZE/64; i++){ // BUFF_SIZE/64 = 4096 lines, also probing in opposite direction 
		latency = measure_one_block_access_time((uint64_t)(eviction_buffer + i*64));
		if(latency > 46) eviction_buffer[i*64]=1;
	       	else eviction_buffer[i*64] = 0; 	
	}
	}
	else {
	for (int i = (BUFF_SIZE/64)-1; i >=0; i--){ // BUFF_SIZE/64 = 4096 lines, also probing in opposite direction 
                latency = measure_one_block_access_time((uint64_t)(eviction_buffer + i*64));
                if(latency > 46) eviction_buffer[i*64]=1;
                else eviction_buffer[i*64] = 0;
        }
	}


	int jval = 0; 
	for (int i = 0; i < BUFF_SIZE/1024; i++){ // need to check 4 lines in 4 sets 
		total = 0;
	        for(int j= 0; j <4; j++){	
		if(eviction_buffer[(i + 1024*j)*64 ] == 1 ) total++; 
		if(eviction_buffer[(i + 256 + 1024*j)*64] == 1) total++; 
		if(eviction_buffer[(i + 512 + 1024*j)*64] == 1) total++; 
		if(eviction_buffer[(i + 768 + 1024*j)*64] == 1) total++; 
		//if(total >=3) printf("\n%d\n",i); 
		jval =j; 
	}
		if(total >= 9) printf("\n%d,%d\n",i,jval);
	}


	/*	for (int i = 0; i < BUFF_SIZE/64; i ++) {
		tmp = eviction_buffer[i*64]; 	
	}	*/
/*	for (int i = 0; i < BUFF_SIZE/64; i ++) {
                latency[i] = measure_one_block_access_time((uint64_t)(eviction_buffer + i*64));
		latency_sum = latency_sum + latency[i]; 
        }
	//printf("\nlatency sum:%d\n",latency_sum); 
	if((latency_sum/(BUFF_SIZE/64)) > 50) printf("\nlatency sum:%d\n",(latency_sum/(BUFF_SIZE/64)));
*/  
      	}

    printf("Receiver finished.\n");

    return 0;
}


