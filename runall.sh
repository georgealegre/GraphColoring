for file in graphs/*; do

    echo "$file" >> benchmarks.txt

    \time -l dirmain/color < "$file" >> benchmarks.txt 2>&1

    echo "-------------------------" >> benchmarks.txt

done
