
#ifndef BITMAINUPLATION_H_
#define BITMAINUPLATION_H_


#define  SET_BIT(var, bitNum)    ( var |=  (1u<<(bitNum)) )
#define  CLR_BIT(var, bitNum)    ( var &= ~(1u<<(bitNum)) )
#define  TGL_BIT(var, bitNum)    ( var ^=  (1u<<(bitNum)) )
#define  GET_BIT(var, bitNum)    ( (var>>bitNum) & 1 )


#endif /* BITMAINUPLATION_H_ */
