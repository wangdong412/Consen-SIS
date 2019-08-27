/* 
   Copyright (C) 1995 Free Software Foundation

 Linear congruential.  */

#include <libc/rand.h>

//void printf(char *s,...);

#define	TYPE_0		0
#define	BREAK_0		8
#define	DEG_0		0
#define	SEP_0		0

/* x**7 + x**3 + 1.  */
#define	TYPE_1		1
#define	BREAK_1		32
#define	DEG_1		7
#define	SEP_1		3

/* x**15 + x + 1.  */
#define	TYPE_2		2
#define	BREAK_2		64
#define	DEG_2		15
#define	SEP_2		1

/* x**31 + x**3 + 1.  */
#define	TYPE_3		3
#define	BREAK_3		128
#define	DEG_3		31
#define	SEP_3		3

/* x**63 + x + 1.  */
#define	TYPE_4		4
#define	BREAK_4		256
#define	DEG_4		63
#define	SEP_4		1

/* Array versions of the above information to make code run faster.
   Relies on fact that TYPE_i == i.  */

#define	MAX_TYPES	5	/* Max number of types above.  */

//void printf(char *f ,...);
    int
__random_r (buf, result)
    struct random_data *buf;
    int32_t *result;
{
    int32_t *state;

        if (buf == NULL || result == NULL)
        {
           // printf("there is a arg error:%s\n",__FUNCTION__);
            return -1;
        }

    state = buf->state;

    if (buf->rand_type == TYPE_0)
    {
        int32_t val = state[0];
        val = ((state[0] * 1103515245) + 12345) & 0x7fffffff;
        state[0] = val;
        *result = val;
    }
    else
    {
        int32_t *fptr = buf->fptr;
        int32_t *rptr = buf->rptr;
        int32_t *end_ptr = buf->end_ptr;
        int32_t val;

        val = *fptr += *rptr;
        /* Chucking least random bit.  */
        *result = (val >> 1) & 0x7fffffff;
        ++fptr;
        if (fptr >= end_ptr)
        {
            fptr = state;
            ++rptr;
        }
        else
        {
            ++rptr;
            if (rptr >= end_ptr)
                rptr = state;
        }
        buf->fptr = fptr;
        buf->rptr = rptr;
    }
    return 0;

    //fail:
    //__set_errno (EINVAL);
    //return -1;
}


