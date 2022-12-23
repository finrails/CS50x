import csv
from sys import argv, exit


def main():

    # TODO: Check for command-line usage

    if len(argv) != 3:
        print("Invalid!")
        exit(1)

    # TODO: Read database file into a variable

    stored_dnas = {}
    patterns = []

    with open(argv[1], "r", newline='') as csv_file:

        csv_reader = csv.reader(csv_file, delimiter=",")

        for line in csv_reader:
            if line[0] == 'name':
                patterns = line[1:len(line)]
                continue

            person_name = line[0]

            index = 1

            stored_dnas[person_name] = {}
            for pattern in patterns:
                stored_dnas[person_name][pattern] = int(line[index])
                index += 1

    # TODO: Read DNA sequence file into a variable

    with open(argv[2], 'r') as dna_sequence:
        sequence = dna_sequence.readline().strip()

    # TODO: Find longest match of each STR in DNA sequence

    sequences = {}
    patterns_length = len(patterns)

    for pattern_index in range(patterns_length):
        current_pattern = patterns[pattern_index]
        sequences[current_pattern] = longest_match(sequence, current_pattern)

    # TODO: Check database for matching profiles

    valid_person = True

    for key, person in stored_dnas.items():
        valid_person = True
        for pattern in patterns:
            if person[pattern] == sequences[pattern]:
                continue
            valid_person = False
            break

        if valid_person:
            print(key)
            break

    if not valid_person:
        print("No match")

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()

