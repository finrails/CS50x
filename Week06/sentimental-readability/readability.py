# TODO
from cs50 import get_string
import string

user_input = get_string("Type a text: ")


def print_grade(grade: int) -> None:
    if grade < 1:
        print("Before Grade 1")
    elif grade > 16:
        print("Grade 16+")
    else:
        print(f"Grade {grade}")


def get_text_grade(striped_text: dict) -> int:
    average_words = striped_text["words"] / 100
    average_letters = striped_text["letters"] / average_words
    average_sentences = striped_text["sentences"] / average_words

    grade = 0.0588 * average_letters - 0.296 * average_sentences - 15.8

    # Return the grade rounded to the nearest integer.
    return round(grade)


def strip_word(text: string) -> dict:
    # Capture the number of letters in a text.
    text_specs = {"letters": 0, "words": 0, "sentences": 0}
    splited_text = text.split(' ')

    for letter in text:
        if letter in string.ascii_letters:
            text_specs["letters"] += 1

    # Capture the number of words in a text.
    text_specs["words"] = len(splited_text)

    # Capute the number of sentences in a text.
    for word in splited_text:
        word_len = len(word)
        if word[word_len-1:word_len] in ['.', '!', '?']:
            text_specs["sentences"] += 1
    return text_specs


text_specs = strip_word(user_input)
grade = get_text_grade(text_specs)
print_grade(grade)
