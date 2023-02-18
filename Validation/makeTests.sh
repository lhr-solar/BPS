errors=0
for x in Tests/Test_*; do
    make clean
    x="${x%.*}"; x="${x#*_}"
    sim='stm32f413'
    if [[ "$x" == *"Simulator"* ]]; then
        sim='simulator'
    fi
    if ! make "$sim" TEST="$x"; then # Different command for STM
        echo make "$sim" TEST="$x"
        errors=$((errors+1))
    fi
done

if ((errors > 0)); then
    echo "Errors occurred during testing"
    exit 1
fi