#include <xen/percpu.h>
#include <xen/random.h>
#include <xen/time.h>
#include <asm/random.h>

static DEFINE_PER_CPU(unsigned int, seed);

unsigned int get_random(void)
{
    unsigned int next = this_cpu(seed), val = arch_get_random();

    if ( unlikely(!next) )
        next = val ?: NOW();

    if ( !val )
    {
        unsigned int i;

        for ( i = 0; i < sizeof(val) * 8; i += 11 )
        {
            next = next * 1103515245 + 12345;
            val |= ((next >> 16) & 0x7ff) << i;
        }
    }

    this_cpu(seed) = next;

    return val;
}
