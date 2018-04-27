sudo apt-get install python-pip python-dev
sudo pip2.7 install numpy scipy scikit-learn==0.17
./waf configure --enable-examples
./waf build