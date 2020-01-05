cd build
if [ -e "OKGen" ]; then
    ./OKGen ../"$1" "${@:2}"
elif [ -e "OKGen.exe" ]; then
    ./OKGen.exe ../"$1" "${@:2}"
fi