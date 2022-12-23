# TODO
while True:
    try:
        height = int(input("Height: "))
        if height < 9 and height > 0:
            break
    except ValueError:
        None

def print_squares(squares):
    for times in range(1, squares + 1):

        squares = squares - 1
        print(" "*squares + "#"*times, end="")
        print("  " + "#"*times)

print_squares(height)
