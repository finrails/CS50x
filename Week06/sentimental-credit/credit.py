# TODO
VALID_DIGITS = "0123456789"

def valid_card(card):
    for card_digit in card:
        if card_digit not in VALID_DIGITS:
            return False
    return True

def check_sum(card):
    card_length = len(card)
    even_digits = 0
    odd_digits = 0
    card = card[::-1]
    for digit_position in range(card_length):
        if int(digit_position) % 2 != 0:
            multiplied_digit = int(card[digit_position]) * 2

            if multiplied_digit > 9:
                multiplied_digit = multiplied_digit % 10 + int(multiplied_digit / 10)

            odd_digits += multiplied_digit
            continue

        even_digits += int(card[digit_position])

    valid_card = (even_digits + odd_digits) % 10 == 0
    card = card[::-1]

    if card[:2] in ['34', '37'] and valid_card and len(card) == 15:
        print("AMEX")
    elif card[:2] in ['51', '52', '53', '54', '55'] and valid_card and len(card) == 16:
        print("MASTERCARD")
    elif card[0] == "4" and valid_card and (len(card) == 13 or len(card) == 16):
        print("VISA")
    else:
        print("INVALID")


def get_card():
    user_input = input("Credit Card: ")
    while not valid_card(user_input):
        user_input = input("Credit Card: ")
    return user_input

user_input = get_card()
check_sum(user_input)
