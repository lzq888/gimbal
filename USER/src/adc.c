
#include <stdlib.h>
#include "adc.h"

void Configure_ADC1(void)
{
  
  ADC_InitTypeDef ADC_InitStructure;


  //72/8 =9MHz
  RCC_ADCCLKConfig(RCC_PCLK2_Div8);

  /*Reset ADC*/
  ADC_DeInit(ADC1);
  /*Configure ADC1*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 4;			                                   // how many channel you have
  ADC_Init(ADC1, &ADC_InitStructure);
  //ADC1 regular channels configuration 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_239Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_239Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_239Cycles5);  
  //   (12.5+239.5)/9*10^-6s=28us   35.7khz

  //Enable ADC1 DMA
  ADC_DMACmd(ADC1, ENABLE);
  
  //Enable ADC1
  ADC_Cmd(ADC1, ENABLE);  

  //Enable ADC1 reset calibaration register 
  ADC_ResetCalibration(ADC1);
  //Check the end of ADC1 reset calibration register 
  while(ADC_GetResetCalibrationStatus(ADC1));

  //Start ADC1 calibaration
  ADC_StartCalibration(ADC1);
  //Check the end of ADC1 calibration 
  while(ADC_GetCalibrationStatus(ADC1));
  
  //Start ADC1 Software Conversion 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

