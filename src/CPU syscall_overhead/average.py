from subprocess import PIPE, run

sys_lst = []
pro_list = []

for i in range(100):
  command = ['./bin.out']
  result = run(command, stdout=PIPE, stderr=PIPE, universal_newlines=True)
  parsed = result.stdout.replace('\n','').split(',')
  syscall_cycles = int(parsed[0].split(':')[1])
  sys_lst.append(syscall_cycles)
  procedure_cycles = int(parsed[1].split(':')[1])
  pro_list.append(procedure_cycles)
  print("syscall cycles:", syscall_cycles, ", procedure cycles:", procedure_cycles)

print("\nsyscall average cycles:", int(sum(sys_lst) / len(sys_lst)), "\nprocedure average cycles:", int(sum(pro_list) / len(sys_lst)), "\n")