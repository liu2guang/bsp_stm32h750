import os
import sys
import rtconfig

RTT_ROOT = os.path.normpath(os.getcwd() + '/rt-thread')

sys.path = sys.path + [os.path.join(RTT_ROOT, 'tools')]
try:
    from building import *
except:
    print('Cannot found RT-Thread root directory, please check RTT_ROOT')
    print(RTT_ROOT)
    exit(-1)

TARGET = 'rtthread.' + rtconfig.TARGET_EXT

env = Environment(tools = ['mingw'],
	AS = rtconfig.AS, ASFLAGS = rtconfig.AFLAGS,
	CC = rtconfig.CC, CCFLAGS = rtconfig.CFLAGS,
	AR = rtconfig.AR, ARFLAGS = '-rc',
	LINK = rtconfig.LINK, LINKFLAGS = rtconfig.LFLAGS)
env.PrependENVPath('PATH', rtconfig.EXEC_PATH)

if rtconfig.PLATFORM == 'iar':
	env.Replace(CCCOM = ['$CC $CCFLAGS $CPPFLAGS $_CPPDEFFLAGS $_CPPINCFLAGS -o $TARGET $SOURCES'])
	env.Replace(ARFLAGS = [''])
	env.Replace(LINKCOM = ['$LINK $SOURCES $LINKFLAGS -o $TARGET --map project.map'])

Export('RTT_ROOT')
Export('rtconfig')

# prepare building environment
objs = PrepareBuilding(env, RTT_ROOT, has_libcpu=False)

# make a building
DoBuilding(TARGET, objs)
