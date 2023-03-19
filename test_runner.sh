# get argument form command line
# $1: number of program to test

program_number=$1

for i in {1..10}
do
    ./${program_number}-score/bin/myprogram ./Test/test${i}.txt ./${program_number}-score/test_results/test${i}.txt;
done 