gcc -g -Ofast -march=native -o lab4.exe lab4.c -pthread
for (( i=1; i <= 10; i++ ))
do
    ./lab4.exe 4 $i 20
done
./line.exe
gcc -g -Ofast -march=native -o lab4_b.exe lab4_b.c -pthread
for (( i=1; i <= 10; i++ ))
do
    ./lab4_b.exe 4 $i 20
done
./line.exe
gcc -g -Ofast -march=native -o lab4_.exe lab4_.c -pthread
for (( i=1; i <= 10; i++ ))
do
    ./lab4_.exe 4 $i 20
done
./line.exe
#for (( i=0; i <= 20; i++ ))
#do
#    ./lab4.exe 1 8 1
#done
