-- Keep a log of any SQL queries you execute as you solve the mystery.

-- 1. Search for Fiftyville: Find out about the bakery.
SELECT description FROM crime_scene_reports
WHERE year = 2021
  AND day = 28
  AND month = 7
  AND street = 'Humphrey Street';

-- 2. Search for interviews on date:
  -- Ruth saw the thief left the Bakery 10 minutes after the crime
  -- Egune said that saw the thief at the ATM on Legget Street, withdrawing some money.
  -- Raymond said the as the thief was leaving the Bakery he was calling to someone, the thief said they were planning to take the
  --   earliest flight ou Fiftyville tomorrow (29), the thief asked the person on the line to buy the ticket.
SELECT name, transcript FROM interviews
WHERE year = 2021
  AND day = 28
  AND month = 7;

-- 3. Search for people's name who have leaving the Bakery within 10 minutes.
  -- Here we got some Names: Vanessa, Barry, Iman, Sofia, Luca, Diana, Kelsey, Bruce.
SELECT name FROM people
WHERE license_plate
  IN (SELECT license_plate FROM bakery_security_logs
        WHERE year = 2021
        AND activity = 'exit'
        AND day = 28 AND month = 7
        AND hour = 10 AND minute > 15 AND minute <= 25);

-- 4. Let's check for ATMs now.
  -- Now we know all the bank accounts of the people who withdrew that day (28) 10 AM.
SELECT * FROM atm_transactions
WHERE year = 2021
  AND month = 7
  AND day = 28
  AND atm_location = 'Leggett Street'
  AND transaction_type = 'withdraw';


-- 5. Let's check the bank accounts then.
  -- Now we have all the IDS of persons who withdrew some money (28) 10 AM
SELECT * FROM bank_accounts WHERE account_number
IN (SELECT account_number FROM atm_transactions
    WHERE year = 2021
      AND month = 7
      AND day = 28
      AND atm_location = 'Leggett Street'
      AND transaction_type = 'withdraw');

-- 6. Now let's check for the people who withdrew that day.
  -- Iman(829) 555-5269 | 7049073643, Luca(389) 555-5198 | 8496433585, Diana(770) 555-1861 | 3592750733, Bruce(367) 555-5533 | 5773159633.
  -- We also know the passport.
SELECT * FROM people WHERE id
IN (SELECT person_id FROM bank_accounts WHERE account_number IN
        (SELECT account_number FROM atm_transactions WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw'));

-- 7. Now we have the passports and phone numbers...
 -- With this query we can get the all flights which of the four people;
SELECT * FROM passengers
WHERE passport_number
    IN (SELECT passport_number FROM people
        WHERE id
        IN (SELECT person_id FROM bank_accounts
            WHERE account_number
            IN (SELECT account_number FROM atm_transactions
                WHERE year = 2021
                AND month = 7
                AND day = 28
                AND atm_location = 'Leggett Street'
                AND transaction_type = 'withdraw')));

-- 8. So let's query for the flights.
  -- Ok, now we have information about all flights which the four passengers took.
  -- It includes origin_airport_id year, day, hour, minute, so we can filter it yet more.
  -- Nice, two flight, the things are getting better. (Day 29 because the flight was took 'tomorrow')

SELECT * FROM flights
WHERE id
    IN (SELECT flight_id FROM passengers
        WHERE passport_number
        IN (SELECT passport_number FROM people
            WHERE id
            IN (SELECT person_id FROM bank_accounts
                WHERE account_number
                IN (SELECT account_number FROM atm_transactions
                    WHERE year = 2021
                    AND month = 7
                    AND day = 28
                    AND atm_location = 'Leggett Street'
                    AND transaction_type = 'withdraw'))))
  AND year = 2021
  AND month = 7
  AND day = 29;

-- 9. Now, let's find the Fifyville airport id.
  -- It has the ID 8, it's a good sign, cause the two flights has the origin airport 8.
SELECT id FROM airports
WHERE city = 'Fiftyville';

-- 10. Bruce(5773159633), Diana(3592750733), Luca(8496433585) Took at the Fiftyville airport.
  -- Now we know theirs IDS, we just need to check who they called, and if has relation between one of them, the chances of one be the thief are high.

-- 11. Let's check for the calls.
  -- Good, just some calls, and one is a familiar number.
  -- So the envidences that Bruce is the thief are VERY high.
  -- So let's get the accomplice.

SELECT caller, receiver FROM phone_calls
WHERE day = 28
AND year = 2021
AND month = 7
AND duration < 60;

-- 12. We know the bruce number, so lets go.
  -- Bruce and Robin.
SELECT name FROM people
WHERE phone_number = '(375) 555-8161';

-- 13. The city is simple, just see the earliest flight took.

-- Just in case i wrote it again to confirm that bruce took the flight 36.
SELECT passport_number FROM people WHERE name = 'Bruce';

SELECT * FROM passengers WHERE passport_number = 5773159633;

-- How we know that the flight 36 is the correct one? It's because the transcript told us that the thief was going to take the earliest flight.

SELECT city FROM airports WHERE id = 4;

-- New York City.
