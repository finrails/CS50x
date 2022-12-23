#include <cs50.h>
#include <stdio.h>

int get_cents(void);
int calculate_quarters(int cents);
int calculate_dimes(int cents);
int calculate_nickels(int cents);
int calculate_pennies(int cents);

int main(void)
{
    // Ask how many cents the customer is owed
    int cents = get_cents();

    // Calculate the number of quarters to give the customer
    int quarters = calculate_quarters(cents);
    cents = cents - quarters * 25;

    // Calculate the number of dimes to give the customer
    int dimes = calculate_dimes(cents);
    cents = cents - dimes * 10;

    // Calculate the number of nickels to give the customer
    int nickels = calculate_nickels(cents);
    cents = cents - nickels * 5;

    // Calculate the number of pennies to give the customer
    int pennies = calculate_pennies(cents);
    cents = cents - pennies * 1;

    // Sum coins
    int coins = quarters + dimes + nickels + pennies;

    // Print total number of coins to give the customer
    printf("%i\n", coins);
}

int get_cents(void)
{
    int number_of_cents;
    do
    {
        number_of_cents = get_int("How many cents?: ");
    }
    while (number_of_cents < 0);

    return number_of_cents;
}

int calculate_quarters(int cents)
{
    float result = cents / 25;
    if((int)result > 0)
    {
        return (int)result;
    }
    return 0;
}

int calculate_dimes(int cents)
{
    float result = cents / 10;
    if((int)result > 0) {
        return (int)result;
    }
    return 0;
}

int calculate_nickels(int cents)
{
    float result = cents / 5;
    if((int)result > 0) {
        return (int)result;
    }
    return 0;
}

int calculate_pennies(int cents)
{
    if(cents < 5)
    {
        return cents;
    }

    return 0;
}

