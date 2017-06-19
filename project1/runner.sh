gcc main.c -o main -lpthread
sleep 1
for ((n=0;n<10;n++)); 
do 
./main
sleep 1 
done