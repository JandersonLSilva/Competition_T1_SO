import re
def get_runtimes(file):
    runtimes = []
    with open(file, 'r') as file:
        text = file.read()
        runtimes.extend(re.findall(r"Tempo de execução: (\d+(\.\d+)?e-\d+)", text))
    return runtimes

for runtime in get_runtimes('./logs/log.txt'):
    print(f"Runtime: {runtime}")
    print('-'*80)
