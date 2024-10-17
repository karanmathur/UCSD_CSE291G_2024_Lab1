
#include"util.h"
// mman library to be used for hugepage allocations (e.g. mmap or posix_memalign only)
#include <sys/mman.h>
#define BUFF_SIZE (256*1024 + 32*1024)
int main(int argc, char **argv)
{
    // [Bonus] TODO: Put your covert channel setup code here
    volatile uint8_t *eviction_buffer = (uint8_t *)malloc(BUFF_SIZE);
    int tmp;
    uint64_t latency[BUFF_SIZE/64] = {0}; 
    uint64_t latency_sum = 0; 
    printf("Please press enter.\n");

    char text_buf[2];
    fgets(text_buf, sizeof(text_buf), stdin);

    printf("Receiver now listening.\n");
    for(int k =0 ; k < 50; k++){
    for (int i = 0; i < BUFF_SIZE/64; i ++) {
                 eviction_buffer[i*64]=1;    
        }       
    }
    bool listening = true;
    while (listening) {
	latency_sum = 0; 
        // [Bonus] TODO: Put your covert channel code here
/*	for (int i = 0; i < BUFF_SIZE/64; i ++) {
		tmp = eviction_buffer[i*64]; 	
	}	*/
	for (int i = 0; i < BUFF_SIZE/64; i ++) {
                latency[i] = measure_one_block_access_time((uint64_t)(eviction_buffer + i*64));
		latency_sum = latency_sum + latency[i]; 
        }
	//printf("\nlatency sum:%d\n",latency_sum); 
	if((latency_sum/(BUFF_SIZE/64)) > 50) printf("\nlatency sum:%d\n",(latency_sum/(BUFF_SIZE/64)));
    }

    printf("Receiver finished.\n");

    return 0;
}


