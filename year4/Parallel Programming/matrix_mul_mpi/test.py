import subprocess

threads = [1, 2, 4, 6, 12]
sizes = [24, 240, 1200, 2400]
progs = ['row', 'col', 'blk']


for prog in progs:
    f = open(f'{prog}_stat.csv', 'w')
    f.write(f'threads,{",".join(map(str, sizes))}\n')
    for j in threads:
        f.write(f'{j},')
        for size in sizes:
            res  = subprocess.run(f'mpiexec --use-hwthread-cpus -n {j} mmul_{prog} {size} {size} {size} {size}'.split(), capture_output=True)
            text = res.stdout.decode('utf-8')
            time = text.split()[-1]
            print(f'threads: {j}, size: {size}, time: {time}')
            f.write(f'{time},')
        f.write('\n')

