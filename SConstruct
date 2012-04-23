import glob
import os
import os.path

sources = {}
environments = []

bin_dir = 'bin'

# node-code selection
node = BUILD_TARGETS[0].split(os.sep)[-1]
node_parts = node.split('.')
if len(node_parts) >= 2:
  node = node_parts[-2]

print "Building node '{0}'".format(node)

node_dir = os.path.join('nodes', node)
sources['node'] = Glob(os.path.join(node_dir, '*.c'))

# lib sources
sources['lib'] = Glob(os.path.join('lib', '*.c'))

backend_root_dir = 'backend'
backend_dirs = os.listdir(backend_root_dir)

# create backend specific environments
for backend_dir in backend_dirs:

  env = Environment()
  env.Default(None)
  env.Append(CCFLAGS = ['-g', '-Wall'])
  env.AppendUnique(CPPPATH=['#lib'])#, '#' + node_dir])

  env.Export(env = env, sources = sources, node = node)
  backend_path = os.path.join(backend_root_dir, backend_dir)
  variant_path = os.path.join(bin_dir, backend_dir)
  sconscript_path = os.path.join(backend_path, 'SConscript')

  env.SConscript(sconscript_path, variant_dir= variant_path, duplicate=0)
  environments.append(env)
