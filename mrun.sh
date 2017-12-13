count=$1
var='0'
while [ $var  -ne $2 ]; do
    # bash mrun.sh NumeroSeedInicial NumeroDeRuns NomeDoAlgoritmo
    mkdir simul$count
    ./waf --run "scratch/v2x_3gpp_small --handoverAlg='$3' --seedValue=$count" > run$count 2>&1
    mv rd* sd* run* qo* simul$count
    let 'count++'
    let 'var++'
done
