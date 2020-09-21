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
cmd /K Application\Project\GCC\devenv.cmd
