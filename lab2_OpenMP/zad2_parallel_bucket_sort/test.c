#include <limits.h>
#include <stdio.h>

int main(void) {
    printf("short is %d bits\n",     CHAR_BIT * sizeof( short )   );
    printf("int is %d bits\n",       CHAR_BIT * sizeof( int  )    );
    printf("long is %d bits\n",      CHAR_BIT * sizeof( long )    );
    printf("long long is %d bits\n", CHAR_BIT * sizeof(long long) );
    return 0;
}
