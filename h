[1mdiff --git a/.settings/language.settings.xml b/.settings/language.settings.xml[m
[1mindex a8f90c9..c9452bc 100644[m
[1m--- a/.settings/language.settings.xml[m
[1m+++ b/.settings/language.settings.xml[m
[36m@@ -6,7 +6,7 @@[m
 				<language-scope id="org.eclipse.cdt.core.gcc"/>[m
 				<language-scope id="org.eclipse.cdt.core.g++"/>[m
 			</provider>[m
[31m-			<provider copy-of="extension" id="altera.tool.Nios2GCCBuildCommandParser"/>[m
[32m+[m			[32m<provider class="org.eclipse.cdt.managedbuilder.language.settings.providers.GCCBuildCommandParser" id="altera.tool.Nios2GCCBuildCommandParser" keep-relative-paths="false" name="Nios II GCC Build Output Parser" parameter="(nios2-elf-gcc)|(nios2-elf-g\+\+)" prefer-non-shared="true"/>[m
 			<provider copy-of="extension" id="org.eclipse.cdt.ui.UserLanguageSettingsProvider"/>[m
 			<provider-reference id="org.eclipse.cdt.core.ReferencedProjectsLanguageSettingsProvider" ref="shared-provider"/>[m
 			<provider-reference id="org.eclipse.cdt.managedbuilder.core.MBSLanguageSettingsProvider" ref="shared-provider"/>[m
[1mdiff --git a/HexDisplay.c b/HexDisplay.c[m
[1mindex 1bd6811..b526fa5 100644[m
[1m--- a/HexDisplay.c[m
[1m+++ b/HexDisplay.c[m
[36m@@ -6,14 +6,15 @@[m
  */[m
 #include "seven_seg_hex.h"[m
 #include "seven_seg_bcd.h"[m
[32m+[m[32m#include "alt_types.h"[m
 [m
 extern volatile alt_u8 leading_zeros;[m
 extern volatile alt_u8 display_as_bcd;[m
 [m
 void HexDisplay(alt_u32 * base, alt_u16 num) {[m
[31m-	if (display_as_bcd) {[m
[32m+[m[32m/*	if (display_as_bcd) {[m
 		seven_seg_bcd(base, num, leading_zeros);[m
[31m-	} else {[m
[31m-		seven_seg_hex(base, num, leading_zeros);[m
[31m-	}[m
[32m+[m	[32m} else {*/[m
[32m+[m		[32mseven_seg_hex(base, num, 0); // leading_zeros);[m
[32m+[m	[32m//}[m
 }[m
[1mdiff --git a/InterruptDemo.c b/InterruptDemo.c[m
[1mindex 222106c..58a66c8 100644[m
[1m--- a/InterruptDemo.c[m
[1m+++ b/InterruptDemo.c[m
[36m@@ -1,7 +1,7 @@[m
[31m-[m
 #include "io.h"[m
 #include "system.h"[m
 #include "alt_types.h"[m
[32m+[m[32m#include "timerISR.h"[m
 [m
 /*******************************************************************************[m
  * int main()                                                                  *[m
[36m@@ -10,14 +10,18 @@[m
 [m
 int main(void)[m
 { [m
[31m-    /* Setup */[m
[32m+[m[32m    alt_u32* pCtr;[m
[32m+[m[32m    static alt_u32 count = 0;[m
[32m+[m[32m    pCtr = &count;[m
 [m
     /* Initialize Variables */[m
 [m
     /* Register ISRs */[m
[31m-[m
[32m+[m[32m    alt_irq_register(INTERVAL_TIMER_IRQ, (void*)pCtr, timerISR);[m
     /* Initialize Timer */[m
[31m-[m
[32m+[m[32m    IOWR(INTERVAL_TIMER_BASE, 2, 0x0000FFFF&INTERVAL_TIMER_FREQ);[m
[32m+[m[32m    IOWR(INTERVAL_TIMER_BASE, 3, 0xFFFF&(INTERVAL_TIMER_FREQ >> 16));[m
[32m+[m[32m    IOWR(INTERVAL_TIMER_BASE, 1, 0x7); // STOP=0, START=1, CONT=1, ITO=1 => 0111b = 0x7[m
     /* Loop while processing interrupts */[m
     while( 1 ) [m
     {[m
[1mdiff --git a/Makefile b/Makefile[m
[1mindex 8d6aa65..d9b6c8f 100644[m
[1m--- a/Makefile[m
[1m+++ b/Makefile[m
[36m@@ -139,7 +139,7 @@[m [mACDS_VERSION := 16.0[m
 # Define path to the application ELF. [m
 # It may be used by the makefile fragments so is defined before including them. [m
 # [m
[31m-ELF := EECE321F16_InterruptDemo.elf[m
[32m+[m[32mELF := InterruptsNIOS.elf[m
 [m
 # Paths to C, C++, and assembly source files.[m
 C_SRCS += HexDisplay.c[m
[36m@@ -183,7 +183,7 @@[m [mCRT0 :=[m
 SYS_LIB :=[m
 [m
 # Define path to the root of the BSP.[m
[31m-BSP_ROOT_DIR := C:/Users/brent.horine/Documents/FPGA/EECE321F16_InterruptDemo_bsp/[m
[32m+[m[32mBSP_ROOT_DIR := C:/Users/psmith04/Documents/FPGA/InterruptsNIOS_bsp/[m
 [m
 # List of application specific include directories, library directories and library names[m
 APP_INCLUDE_DIRS :=[m
[36m@@ -194,7 +194,7 @@[m [mAPP_LIBRARY_NAMES :=[m
 BUILD_PRE_PROCESS :=[m
 BUILD_POST_PROCESS :=[m
 [m
[31m-QUARTUS_PROJECT_DIR := C:/Users/brent.horine/Documents/FPGA/DE2-115_Media_ComputerV16/verilog/[m
[32m+[m[32mQUARTUS_PROJECT_DIR := C:/altera/16.0/University_Program/Computer_Systems/DE2-115/DE2-115_Media_Computer/verilog/[m
 [m
 [m
 #END GENERATED[m
[1mdiff --git a/buttonISR.c b/buttonISR.c[m
[1mindex 1ad0f4f..ab91f49 100644[m
[1m--- a/buttonISR.c[m
[1m+++ b/buttonISR.c[m
[36m@@ -6,7 +6,9 @@[m
  */[m
 [m
 #include "buttonISR.h"[m
[31m-#include "altera_avalon_pio_regs.h"[m
[32m+[m[32m#include "io.h"[m
[32m+[m[32m#include "system.h"[m
[32m+[m[32m#include "sys\alt_irq.h"[m
 [m
 #ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT[m
 void buttonISR(void* context)[m
[36m@@ -21,7 +23,7 @@[m [mvoid buttonISR(void* context, alt_u32 id)[m
     /* Store the value in the PUSHBUTTONS's edge capture register in *context. */[m
 [m
     /* Reset the PUSHBUTTONS's edge capture register. */[m
[31m-    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PUSHBUTTONS_BASE, 0);[m
[32m+[m[32m    //IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PUSHBUTTONS_BASE, 0);[m
 [m
     /* Act upon the interrupt */[m
 [m
[36m@@ -30,5 +32,5 @@[m [mvoid buttonISR(void* context, alt_u32 id)[m
      * interrupt in systems with high processor -> pio latency and fast[m
      * interrupts.[m
      */[m
[31m-    IORD_ALTERA_AVALON_PIO_EDGE_CAP(PUSHBUTTONS_BASE);[m
[32m+[m[32m    //IORD_ALTERA_AVALON_PIO_EDGE_CAP(PUSHBUTTONS_BASE);[m
 }[m
[1mdiff --git a/timerISR.c b/timerISR.c[m
[1mindex dbfee94..bcd488d 100644[m
[1m--- a/timerISR.c[m
[1m+++ b/timerISR.c[m
[36m@@ -4,7 +4,8 @@[m
  *  Created on: [m
  *      Author: [m
  */[m
[31m-#include "System.h"[m
[32m+[m[32m#include "system.h"[m
[32m+[m[32m#include "io.h"[m
 #include "alt_types.h"[m
 #include "HexDisplay.h"[m
 [m
[36m@@ -22,16 +23,16 @@[m [mvoid timerISR(void * context, alt_u32 id)[m
 	/* Cast context. It is important that this be declared[m
 	 * volatile to avoid unwanted compiler optimization.[m
 	 */[m
[31m-[m
[32m+[m	[32malt_u32* pCount = (alt_u32*)context;[m
 	/* clear the interrupt */[m
[31m-[m
[32m+[m[32m    IOWR(INTERVAL_TIMER_BASE, 0, 1);[m
 	/* Act upon the interrupt */[m
[31m-[m
[32m+[m[32m    *pCount = *pCount + 1;[m
 	/* Display count value [m
 	 * (Is this the best place to do this? [m
 	 * Why or why not?) [m
 	 */[m
[31m-	HexDisplay((alt_u32*)HEX3_HEX0_BASE, *count_ptr);[m
[32m+[m	[32mHexDisplay((alt_u32*)HEX3_HEX0_BASE, *pCount);[m
 [m
 	return;[m
 }[m
warning: LF will be replaced by CRLF in Makefile.
The file will have its original line endings in your working directory.
