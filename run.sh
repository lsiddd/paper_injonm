echo -n "how many times do you wish to run? "
read n

echo -n "whats the algorithm used? "
read alg

for i in $(seq 1 $n)
do
    time ./waf --run "scratch/v2x_3gpp_small --handoverAlg=$alg --seedValue=$i" > run-$alg-$i 2>&1
    mkdir $alg
    mkdir $alg/simul$i
    mv rd* sd* $alg/simul$i
    rm -rf rnti qo*Torre* Dl* Ul*
    python ../../finish.pyo $alg $i
done

exit
