# Simulate a sports tournament

import csv
import sys
import random

# Number of simluations to run
N = 1000


def main():

    # Ensure correct usage
    if len(sys.argv) != 2:
        sys.exit("Usage: python tournament.py FILENAME")

    teams = []
    # TODO: Read teams into memory from file
    csv_file = open(sys.argv[1], newline='')

    # Return a csv object using the opened csv file.
    csv_reader = csv.reader(csv_file, delimiter=';')

    # Iterate through csv object where each current_line is an array containing one string: country,rating
    for current_line in csv_reader:
        if current_line[0] == 'team,rating':
            continue

        # Split the string country,rating into an array [country, rating]
        current_team = current_line[0].split(',')
        # Just append the data to the proposed format.
        teams.append({"team": current_team[0], "rating": int(current_team[1])})

    counts = {}
    # TODO: Simulate N tournaments and keep track of win counts
    for _ in range(N):
        current_round_winner = simulate_tournament(teams)
        if current_round_winner not in counts:
            counts[current_round_winner] = 1
            continue

        counts[current_round_winner] = counts[current_round_winner] + 1

    # Print each team's chances of winning, according to simulation
    for team in sorted(counts, key=lambda team: counts[team], reverse=True):
        print(f"{team}: {counts[team] * 100 / N:.1f}% chance of winning")


def simulate_game(team1, team2):
    """Simulate a game. Return True if team1 wins, False otherwise."""
    rating1 = team1["rating"]
    rating2 = team2["rating"]
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))
    return random.random() < probability


def simulate_round(teams):
    """Simulate a round. Return a list of winning teams."""
    winners = []

    # Simulate games for all pairs of teams
    for i in range(0, len(teams), 2):
        if simulate_game(teams[i], teams[i + 1]):
            winners.append(teams[i])
        else:
            winners.append(teams[i + 1])

    return winners


def simulate_tournament(teams):
    """Simulate a tournament. Return name of winning team."""
    # TODO
    if len(teams) > 1:
        current_teams = simulate_round(teams)
        return simulate_tournament(current_teams)

    winner_team = teams[0]["team"]
    return winner_team


if __name__ == "__main__":
    main()

