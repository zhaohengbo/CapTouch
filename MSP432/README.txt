------------Capacitive Touch Library for MSP432 and Getting Started Guide------------------
September 2015
Texas Instruments, Inc.
-------------------------------------------------------------------------------------------
CONTENTS

I. Library
II. Firmware (Examples)
III. Manifest

I. Library
-----------------------------------
The following files are found in the Library directory:

    CTS_HAL.c
    CTS_HAL.h
    CTS_Layer.c
    CTS_Layer.h

II. Firmware (Source for Examples)
-----------------------------------  
The following example directories incude a main application file and configuration
files structure.c/.h. These files are used with the files found in the source 
directory. The element definitions found in the configuration file structure.c uses a 
designated initializer list. This allows members to be initialized in any order and 
also enhances the readability of the element being initialized.  This feature 
requires the GCC language extension found in Code Composer Studio (CCS).  
C99 is the default dialect found in IAR and therefore the default settings can be 
used.

	430BOOST-SENSE1_CapTouch_MSP432P401R
	430BOOST-SHARP96_CapTouch_MSP432P401R
	TouchProTool_Demo_UART_MSP432

III. Documentation
-----------------------------------
The CTS for MSP432 Programmer's Guide explains the key components of and how to use
the Capacitive Touch Software Library for MSP432. 

For more information on the example firmware, please refer to the 
MSP-EXP432P401R and Touch Pro GUI User Guides.