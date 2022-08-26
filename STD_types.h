
#ifndef STD_TYPES_H_
#define STD_TYPES_H_

#include <stm32f10x.h>

typedef  float                   f32;
typedef  double                  f64;


#define NULL ((void *)0)


typedef  enum
{
	FALSE = 0,
	TRUE
}Boolean_t;


typedef   FunctionalState  STD_State_t;


#define  OK        1
#define  NOT_OK    0
#define  ERROR     NOT_OK




#endif /* STD_TYPES_H_ */
