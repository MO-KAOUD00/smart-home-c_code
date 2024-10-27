/****************************************************/
/********** SWC : DIO *******************************/
/********** Date : 13 August 2024 *******************/
/********** Author : Yousef Ahmad *******************/
/********** Version :1.0          *******************/


#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "EXTI_interface.h"
#include "EXTI_config.h"
#include "EXTI_private.h"

static void (*GlobalPtrTofunc[3])(void)={NULL};






void EXIT_voidEnableDisable(Int_num_e Copy_e_Int_Num,Int_Mode_e Copy_e_Mode)
{
	switch(Copy_e_Int_Num)
	{
	case INT_0:
		switch(Copy_e_Mode)
		{
		case Enabled:SET_BIT(GICR,GICR_INT0);
		break;
		case Disabled:CLR_BIT(GICR,GICR_INT0);
		break;
		}
		;break;
		case INT_1: switch(Copy_e_Mode)
		{
		case Enabled:SET_BIT(GICR,GICR_INT1);
		break;
		case Disabled:CLR_BIT(GICR,GICR_INT1);
		break;
		};break;
		case INT_2:      switch(Copy_e_Mode)
		{
		case Enabled:SET_BIT(GICR,GICR_INT2);
		break;
		case Disabled:CLR_BIT(GICR,GICR_INT2);
		break;
		};break;
	}
}
void EXIT_voidSetSenseControl(Int_num_e Copy_e_Int_Num,Int_SenseCtrl_e Copy_e_SenseCtrl)
{
switch(Copy_e_Int_Num)
{
case INT_0:
	switch(Copy_e_SenseCtrl)
	{
	case low_level :
		CLR_BIT(MCUCR,MCUCR_ISC00);
		CLR_BIT(MCUCR,MCUCR_ISC01);
		break;
	case logical_change :
		SET_BIT(MCUCR,MCUCR_ISC00);
		CLR_BIT(MCUCR,MCUCR_ISC01);
		break;
	case falling_age :
		CLR_BIT(MCUCR,MCUCR_ISC00);
		SET_BIT(MCUCR,MCUCR_ISC01);
		break;
	case rising_age :
		SET_BIT(MCUCR,MCUCR_ISC00);
		SET_BIT(MCUCR,MCUCR_ISC01);
		break;
	}
	break;
case INT_1:
	switch(Copy_e_SenseCtrl)
		{
		case low_level :
			CLR_BIT(MCUCR,MCUCR_ISC10);
			CLR_BIT(MCUCR,MCUCR_ISC11);
			break;
		case logical_change :
			SET_BIT(MCUCR,MCUCR_ISC10);
			CLR_BIT(MCUCR,MCUCR_ISC11);
			break;
		case falling_age :
			CLR_BIT(MCUCR,MCUCR_ISC10);
			SET_BIT(MCUCR,MCUCR_ISC11);
			break;
		case rising_age :
			SET_BIT(MCUCR,MCUCR_ISC10);
			SET_BIT(MCUCR,MCUCR_ISC11);
			break;
		}
	break;
case INT_2:
	switch(Copy_e_SenseCtrl)
		{

		case falling_age :
			CLR_BIT(MCUCSR,MCUCSR_ISC2);

			break;
		case rising_age :
			SET_BIT(MCUCSR,MCUCSR_ISC2);
			break;
		}
	break;
}

}

Std_err_e EXTI_voidSetCallBack(Int_num_e Copy_e_IntNum,void (*PvoidFunc)(void))
{
	Std_err_e Local_eState=OK;
	switch(Copy_e_IntNum)
	{
	case INT_0:
		if(PvoidFunc !=NULL)
		{
			GlobalPtrTofunc[0]=PvoidFunc;
		}
		else
		{
			Local_eState=NOK;
		}
		break;

	case INT_1:
		if(PvoidFunc !=NULL)
		{
			GlobalPtrTofunc[1]=PvoidFunc;
		}
		else
		{
			Local_eState=NOK;
		}
		break;
	case INT_2:
		if(PvoidFunc !=NULL)
		{
			GlobalPtrTofunc[2]=PvoidFunc;
		}
		else
		{
			Local_eState=NOK;
		}
		break;
	}

	return Local_eState;
}


void __vector_1(void)  __attribute__((signal));
void __vector_1(void)
{
   //INVOKE FUNCTION TO BE RUN INSIDE INT0
	GlobalPtrTofunc[0]();
}

void __vector_2(void)  __attribute__((signal));
void __vector_2(void)
{
	GlobalPtrTofunc[1]();
}

void __vector_3(void)  __attribute__((signal));
void __vector_3(void)
{
	GlobalPtrTofunc[2]();
}
