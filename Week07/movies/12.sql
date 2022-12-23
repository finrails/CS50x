SELECT DISTINCT title FROM movies
JOIN stars ON movies.id = stars.movie_id
WHERE stars.person_id IN (SELECT id FROM people WHERE name = 'Johnny Depp' OR name = 'Helena Bonham Carter')
AND movies.id IN (SELECT movie_id from stars WHERE person_id = (SELECT id FROM people WHERE name = 'Johnny Depp'))
AND movies.id IN (SELECT movie_id from stars WHERE person_id = (SELECT id FROM people WHERE name = 'Helena Bonham Carter'));
