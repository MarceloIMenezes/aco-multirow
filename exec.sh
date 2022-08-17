#!/bin/bash

auto_exec() {

    FILENAME=$1
    M=$2
    TABLE=$3

    echo "----- INICIANDO $FILENAME -----"
    echo "Criando diretório . . ."
    mkdir ./solution/$FILENAME$M

    for i in $(seq 1 30); do
        echo "Executando $i"
        ./main ./data/sfmrflp_instances/table_$TABLE/$FILENAME.txt ./solution/$FILENAME$M/exec$i.txt $(($i*4576 + 15753))
    done

    echo "Passando para CSV . . ."
    python3 ./tocsv.py solution/$FILENAME$M/
    echo "Done"

}

make

auto_exec AKV_60_05 _5 13
auto_exec AKV_70_05 _5 13
auto_exec CAP_n60_p30_s30_1 _5 14
auto_exec CAP_n60_p90_s60_1 _5 14