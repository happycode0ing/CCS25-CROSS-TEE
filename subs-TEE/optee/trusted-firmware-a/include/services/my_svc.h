/* Custom Runtime Service Header File */

#ifndef MY_SVC_H
#define MY_SVC_H

#include <smccc_helpers.h>

#define OEN_KEY_START (OEN_RSV_START+1)
#define OEN_KEY_END (OEN_RSV_START+1)
#define OEN_ECSC_START (OEN_RSV_START+2)
#define OEN_ECSC_END (OEN_RSV_START+2)
#define OEN_CP_START (OEN_RSV_START+3)
#define OEN_CP_END (OEN_RSV_START+3)

/* SMC function IDs for Test Service queries */

/* function id rule:
 TYPE SMC_64/32   OEN     RESERVED  SVC_HINT 	NUM 
    1      1      001000     0000000     0	  1111 1111 0000 0000
*/
#define TEST_SVC_ADD	0xc800ff00 /* TEST OEN=8 */
#define TEST_SVC_MUL	0xc800ff01
#define KEY_SVC_GET	0xc900ff00 /* KEY OEN=9 */
#define ECSC_SVC_EXT	0xca00ff00 /* ECSC OEN=10 */
#define CP_SVC_INVOKE	0xcb00ff00 /* CP OEN=11 */

/* Level2-TEE Definition */
#define SUBF_TEE	0
#define SUBS_TEE	1
#define SUBT_TEE	2

#endif /* MY_SVC_H */
