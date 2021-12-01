#! /bin/sh

numberOfFiles=$(ls Tests/Inputs/ | wc -l);
echo
echo "TESTS FOR SCANNER:"
echo

for((i=1;i<$numberOfFiles+1;i++))
do
  if [ $i == 51 ]
  then
    echo
    echo "TESTS FOR PARSER:"
    echo
  fi

  echo "---Test" $i "---";
  echo

    if [ $i -lt 0 ]
    then
      $PWD/Tests/test.o  < $PWD/Tests/Inputs/testInput_$i.txt > $PWD/Tests/Outputs/testOutput_$i.txt
    else
      $PWD/ifj21  < $PWD/Tests/Inputs/testInput_$i.txt > $PWD/Tests/Outputs/testOutput_$i.txt
    fi
    diff -us  $PWD/Tests/Outputs/testOutput_$i.txt $PWD/Tests/Referals/referOutput_$i.txt
  echo
done
