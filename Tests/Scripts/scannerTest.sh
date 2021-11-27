#! /bin/sh

numberOfFiles=$(ls Tests/Inputs/ | wc -l);

echo "TESTS FOR SCANNER:"
echo

for((i=1;i<$numberOfFiles+1;i++))
do
  echo "---Test" $i "---";
  echo
    $PWD/Tests/test.o  < $PWD/Tests/Inputs/testInput_$i.txt > $PWD/Tests/Outputs/testOutput_$i.txt
    diff -us  $PWD/Tests/Outputs/testOutput_$i.txt $PWD/Tests/Referals/referOutput_$i.txt
  echo
done