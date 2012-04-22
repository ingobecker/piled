# Helper methods
def create_opts_str(opts_array, prefix='-'):
  str = ''
  for o in opts_array:
    str += ' {0}{1}'.format(prefix, o)
  return str

def create_ext_opts_str(opts_dict):
  str = ''
  for o in opts_dict:
    str += " -{0}={1}".format(o, opts_dict[o])
  return str

def change_suffix(obj, suffix):
  return str(obj).split('.', 1)[0] + ".{0}".format(suffix)

def PhonyTargets(env = None, **kw):
  if not env: env = DefaultEnvironment()
  for target,action in kw.items():
    env.AlwaysBuild(env.Alias(target, [], action))

def cmd_list_to_action(str_list):
   return ' '.join(map('{0};'.format, str_list))

def make_phony(env, target, action):
  env.AlwaysBuild(env.Alias(target = target, action = action))

def generate_gdb_options(opt_list):
  option_str = ''
  for opt in opt_list:
    option_str += '-ex "{0}" '.format(opt)
  return option_str
