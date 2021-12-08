#! /bin/sh

numberOfFiles=$(ls Tests/Inputs/ | wc -l);
echo
echo "TESTS FOR SCANNER:"
echo

for((i=1;i<$numberOfFiles+1;i++))
do
  	echo "---Test" $i "---";
  	echo
	#valgrind --leak-check=full --track-origins=yes $PWD/ifj21  < $PWD/Tests/Inputs/testInput_$i.txt 2> $PWD/Tests/Outputs/testOutput_$i.txt
	$PWD/ifj21  < $PWD/Tests/Inputs/testInput_$i.txt 2> $PWD/Tests/Outputs/testOutput_$i.txt
    diff -us  $PWD/Tests/Outputs/testOutput_$i.txt $PWD/Tests/Referals/referOutput_$i.txt
  	echo
done
