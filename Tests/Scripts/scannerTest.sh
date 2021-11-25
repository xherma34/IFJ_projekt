#! /bin/sh

numberOfFiles=$(ls Tests/Inputs/ | wc -l);

echo "TESTS FOR SCANNER:"
echo

for((i=1;i<$numberOfFiles+1;i++))
do
  echo "---Test" $i "---";
  echo
  make test n=$i --no-print-directory;
  echo
done