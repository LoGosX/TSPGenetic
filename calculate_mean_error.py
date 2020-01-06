
def load_last_n_results(instance_name):
    file_name = instance_name + '.results'
    file_path = 'datasets/' + file_name
    with open(file_path, 'r') as file:
        distances = []
        for line in file:
            if line.startswith('Distance'):
                distances.append(float(line.split(' ')[-1]))