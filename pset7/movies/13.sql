SELECT distinct(name) FROM people JOIN stars ON people.id = stars.person_id WHERE
stars.movie_id IN (SELECT stars.movie_id FROM stars JOIN people ON stars.person_id = people.id
WHERE name = "Kevin Bacon" AND birth = 1958)
AND name != "Kevin Bacon"