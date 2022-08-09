#!/bin/bash

FILENAME=N25_01
M=_4

echo "Criando diretório . . ."
mkdir ./solution/$FILENAME$M

for i in $(seq 1 30); do
    echo "Executando $i"
    ./main ./data/sfmrflp_instances/table_10/$FILENAME.txt ./solution/$FILENAME$M/exec$i.txt $(($i*4576 + 15753))
done

echo "Passando para CSV . . ."
python3 ./tocsv.py solution/$FILENAME$M/
echo "Done"