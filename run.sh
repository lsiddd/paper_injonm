echo -n "how many times do you wish to run? "
read times

echo -n "whats the algorithm used? "
read alg

for i in $(seq 1 $times)
do
    ./waf --run "scratch/v2x_3gpp_small --handoverAlg=$alg --seedValue=$i"
    mkdir simul$i
    mv rd* sd* simul$i
    rm -rf rnti qo* Dl* Ul*
done

exit
