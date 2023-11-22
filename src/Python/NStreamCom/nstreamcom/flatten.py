'''Function to flatten sequences in a sequence'''

def flatten(seq):
    '''Flatten a sequence of other sequences'''

    return [item for sublist in seq for item in sublist]
