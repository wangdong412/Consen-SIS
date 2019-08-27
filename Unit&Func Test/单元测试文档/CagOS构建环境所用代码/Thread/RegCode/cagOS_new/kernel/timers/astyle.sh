#! /bin/bash

for f in $(find . -name '*.c' -or -name '*.h' -type f)
do
   astyle --style=linux $f
done

# after formate the code,we need to rm '*.orig' files
for f in $(find . -name '*.orig' -type f)
do
   rm $f
done
