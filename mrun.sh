count=$1
while [ $count -ne $(($2+$1)) ]; do
    # bash mrun.sh NumeroSeedInicial NumeroDeRunsQueQueroRodar NomeDoAlgoritmo
    mkdir simul$count
    ./waf --run "scratch/v2x_3gpp_small --handoverAlg='$3' --seedValue=$count" > run$count 2>&1
    mv rd* sd* run* qo* simul$count
    echo $count
    let 'count++'
done
