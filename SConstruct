import glob

# compile simulator using specific node code
default_node =  'hellocluster'
node = ARGUMENTS.get('node', default_node)
node_dir = 'nodes/' + node
node_sources = Glob(node_dir + '/*.c')

# create build environment
env = Environment()

# determine compiler and linker flags for SDL
env.ParseConfig('sdl-config --cflags')
env.ParseConfig('sdl-config --libs')

# gather a list of source files
target_sources = ['simulator.c']#Glob('*.c')
lib_sources = Glob('lib/*.c')

# add additional compiler flags
env.Append(CCFLAGS = ['-g', '-Wall'])
# add additional libraries to link against
env.Append(LIBS = ['SDL_image'])
# add fifo header and lib
env.AppendUnique(CPPPATH=['lib', node_dir])
# add static simulation flag as there is no other
# hardware-implementation at the moment
env.AppendUnique(CPPDEFINES=['_SIM_'])

# build target
# output executable will be "game"
env.Program(target = 'simulator', source = target_sources + lib_sources + node_sources)
