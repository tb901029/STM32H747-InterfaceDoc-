@echo off

rem *************************************************************************************************
rem Please set the ABSOLUTE installation path of the ST-LINK utility
rem *************************************************************************************************

::set ST-LINK_Utility_Path=C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility
set STM32CubeProgrammer_Path=C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer

rem *************************************************************************************************
rem Please do NOT change the following settings
rem *************************************************************************************************
set PlatformName=STM32H747-Discovery
set ExternalLoader=MT25TL01G_STM32H747I-DISCO.stldr
set BuildEnvVersion=V9.30
set GettingStartedLink=getting-started-stm32h747-discovery
cd ..
call .\Application\Project\GCC\devenv.cmd

echo.
echo *******************************************************************************
echo *
echo * Flashing Embedded Wizard Master Demo for target %PlatformName%
echo *
echo *******************************************************************************
echo.

set APP_FILE=..\..\..\MasterDemo\EmbeddedWizard-%PlatformName%

"%STM32CubeProgrammer_Path%/bin/STM32_Programmer_CLI" -c port=SWD -w %APP_FILE%.hex -el "%STM32CUBEPROG_EXT_FLASH_LOADER%" -g 0x8000000
