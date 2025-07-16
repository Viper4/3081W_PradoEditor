## README.md for images folder

Folder contains two csv files, one called `smallerpaintings.csv` which is a small csv containing 50 rows of data sourced from 
https://www.kaggle.com/datasets/maparla/prado-museum-pictures. It is split into columns containing useful data to assign the attributes 
that we need for our Artwork structs found inside the artwork.h file under the include folder.

The other csv file is called `clean_smallerpaintings.csv`. It is a cleaned version of smallerpaintings.csv that is easier to parse and is cleaned
by the `csv_fixer.py` file.
Overall, it has some columns in the smallerpaintings.csv removed to make the storage more efficient, and faster to parse.


USED FOR TESTING PURPOSES. Database and url query will be implemented to allow for larger amounts of artworks for the user.
