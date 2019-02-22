# BSP Note: For TI EK-TM4C1294XL Tiva C Series Connected LancuhPad	(REV D)

import os

# toolchains options
ARCH       = 'arm'
CPU        = 'cortex-m7'
CROSS_TOOL = 'gcc'

if os.getenv('RTT_CC'):
	CROSS_TOOL = os.getenv('RTT_CC')
	
#device options

# cross_tool provides the cross compiler
# EXEC_PATH is the compiler execute path, for example, CodeSourcery, Keil MDK, IAR
if  CROSS_TOOL  == 'gcc':
	PLATFORM 	 = 'gcc'
	EXEC_PATH 	 = 'D:/ArdaArmTools/GNUARM_4.9_2015q1/bin'
elif CROSS_TOOL == 'keil':
	PLATFORM 	 = 'armcc'
	EXEC_PATH 	 = 'C:/Keil_v5'
elif CROSS_TOOL == 'iar':
	PLATFORM 	 = 'iar'
	EXEC_PATH 	 = 'C:/Program Files (x86)/IAR Systems/Embedded Workbench 7.2'

if os.getenv('RTT_EXEC_PATH'):
	EXEC_PATH = os.getenv('RTT_EXEC_PATH')

BUILD = 'debug' #release

# os.system("python rt-thread/tools/mkromfs.py board/romfs/ board/drives/romfs/drv_romfs.c")

if PLATFORM == 'gcc':
    # tool-chains
    PREFIX = 'arm-none-eabi-'
    CC = PREFIX + 'gcc'
    AS = PREFIX + 'gcc'
    AR = PREFIX + 'ar'
    LINK = PREFIX + 'gcc'
    TARGET_EXT = 'elf'
    SIZE = PREFIX + 'size'
    OBJDUMP = PREFIX + 'objdump'
    OBJCPY = PREFIX + 'objcopy'
	
    DEVICE = ' -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections'
    CFLAGS = DEVICE + ' -std=c99'
    AFLAGS = ' -c' + DEVICE + ' -x assembler-with-cpp -Wa,-mimplicit-it=thumb '
    LFLAGS = DEVICE + ' -Wl,--gc-sections,-Map=rtthread.map,-cref,-u,Reset_Handler -T board/linker_scripts/link.lds'

    CPATH = ''
    LPATH = ''

    if BUILD == 'debug':
        CFLAGS += ' -O0 -gdwarf-2 -g'
        AFLAGS += ' -gdwarf-2'
    else:
        CFLAGS += ' -O2'

    POST_ACTION = OBJCPY + ' -O binary $TARGET build/rtthread_gcc.bin\n' + SIZE + ' $TARGET \n' + 'cmd.exe /k "call scripts\clear.bat"\n'

elif PLATFORM == 'armcc':
    # toolchains
    CC = 'armcc'
    AS = 'armasm'
    AR = 'armar'
    LINK = 'armlink'
    TARGET_EXT = 'axf'

    DEVICE = ' --cpu Cortex-M4.fp '
    CFLAGS = '-c ' + DEVICE + ' --apcs=interwork --c99' # -D' + PART_TYPE
    AFLAGS = DEVICE + ' --apcs=interwork '
    LFLAGS = DEVICE + ' --scatter "board\linker_scripts\link.sct" --info sizes --info totals --info unused --info veneers --list rtthread.map --strict'

    CFLAGS += ' -I' + EXEC_PATH + '/ARM/ARMCC/INC'
    LFLAGS += ' --libpath ' + EXEC_PATH + '/ARM/ARMCC/LIB'
	
    CFLAGS += ' -D__MICROLIB '
    AFLAGS += ' --pd "__MICROLIB SETA 1" '
    LFLAGS += ' --library_type=microlib '
    EXEC_PATH += '/arm/armcc/bin/'

    if BUILD == 'debug':
        CFLAGS += ' -g -O0'
        AFLAGS += ' -g'
    else:
        CFLAGS += ' -O2'

    POST_ACTION = 'fromelf --bin $TARGET --output build/rtthread_mdk.bin \nfromelf -z $TARGET \n' + 'cmd.exe /k "call scripts\clear.bat"\n'
