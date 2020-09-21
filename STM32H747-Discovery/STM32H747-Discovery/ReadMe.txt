/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This software is delivered "as is" and shows the usage of other software
* components. It is provided as an example software which is intended to be
* modified and extended according to particular requirements.
*
* TARA Systems hereby disclaims all warranties and conditions with regard to the
* software, including all implied warranties and conditions of merchantability
* and non-infringement of any third party IPR or other rights which may result
* from the use or the inability to use the software.
*
********************************************************************************
*
* DESCRIPTION:
*   This package contains everything needed to build and run an Embedded Wizard
*   generated UI application on a STM32 target.
*   This Build Environment for Embedded Wizard generated UI applications was
*   tested by using the following components:
*   - Embedded Wizard Studio V9.30
*   - Embedded Wizard Platform Package for STM32 V9.30
*   - STM32H747 Discovery Build Environment V9.30
*   - STM32H747 Discovery board
*   - STM32CubeProgrammer V2.4.0
*   - STM32CubeH7 Firmware Package V1.7.0
*   - GCC ARM Embedded Toolchain 8-2019-q3-update
*   - IAR Embedded Workbench 8.50.1, IAR C/C++ Compiler V8.50.1.245/W32 for ARM
*   - Keil MDK-ARM Professional Version 5.29.0.0, ARM Compiler 6.13.1 (armclang)
*   - Atollic TrueSTUDIO 9.3.0
*
*******************************************************************************/

Getting started with STM32H747 Discovery board:
-----------------------------------------------
  In order to get your first Embedded Wizard generated UI application up and
  running on your STM32 target, we have prepared a detailed article, which
  covers all necessary steps.
  We highly recommend to study the following document:

  https://doc.embedded-wizard.de/getting-started-stm32h747-discovery


Getting started with Embedded Wizard Studio:
--------------------------------------------
  In order to get familiar with Embedded Wizard Studio and the UI development
  work-flow, we highly recommend to study our online documentation:

  https://doc.embedded-wizard.de

  Furthermore, we have collected many 'Questions and Answers' covering
  typical Embedded Wizard programming aspects. Please visit our community:

  https://ask.embedded-wizard.de

  Please use this platform to drop your questions, answers and ideas.


/*******************************************************************************
* Important note:
* This Build Environment is intended to be used as template for Embedded Wizard
* GUI applications on the development board or your customer specific hardware.
* Please take care to adjust all timings and hardware configurations (e.g. system
* clock configurations, memory timings, MPU and cache settings) according to your
* needs and hardware capabilities in order to ensure a stable system.
* Please also take the hardware manufacturer's corresponding specifications,
* application notes and erratas into account.
*******************************************************************************/


Build Environment specific changes:
-----------------------------------
* Version 9.30.01
  - Adding SmartThermostat example. MasterDemo enhanced.
  - DeviceDriver.c template improved to handle display update request properly.
  - Memory pool defines renamed and typos in comments fixed.
  - Changed to STM32CubeH7 Firmware Package V1.7.0
  - Test with Embedded Wizard V9.30 and STM32 Platform Package V9.30

* Version 9.30
  - Support of STM32H747 Discovery board (STM32H747I-DISCO with MB1166 Display).
  - The TLSF memroy manager has been removed: The Runtime Environment
    contains now an own optimized memory manager that is tailored for GUI
    applications on resource constraint embedded systems.
    The Embedded Wizard Heap Manager is now used instead of TLSF.
  - The file ewmain.c is adapted to use the new memory manager.
    For debugging purposes the new function EwVerifyHeap() is called
    before the Garbage Collector is started, to check the memory integrity.
  - The project configuration is now done within the new file ewconfig.h.
    The file ewmain.c does no more contain configuration settings.
    The makefile/project files contain only those settings which have an
    impact to the project structure or the selection of files and libraries.
  - The interface between Embedded Wizard and the underlying hardware specific
    Board Support Package has been redesigned and aligned with all other
    platforms. Please have a look to the header files within the folder
    /TargetSpecific for more details.
  - Support of multi-touch: The touch driver ew_bsp_touch.c provides now
    multi-touch events. The main loop in ewmain.c has been adapted to
    process multi-touch events.
  - The files ew_bsp_serial.c/h are renamed to ew_bsp_console.c/h
  - Changed to GCC ARM Embedded Toolchain 8-2019-q3-update
  - Test with Embedded Wizard V9.30 and STM32 Platform Package V9.30


3rdParty-Components
-------------------
The following 3rdParty-Components are used:

1. STM Source code

* Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

2. Gnu Make - GNU General Public License v2 or later

3. Gnu GCC - GNU General Public License v2 or later
