count='0'
var='0'
while [ $var  -ne '30' ]; do
    mkdir simul$count
    NS_GLOBAL_VALUE="RngRun=$count" ./waf --run scratch/v2x_3gpp > run$count 2>&1
    mv rd* sd* simul$count
    let 'count++'

    if [ "$(ls -A simul$count)" ]; then
        let 'var++'
    fi
done
