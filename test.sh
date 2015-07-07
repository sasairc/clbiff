#!/bin/bash
#
# 簡単なテスト　
#

for ((i = 0; i < 16; i++)); do
	test `expr $i % 2` -eq 0 && \
		echo "touch test.txt" &&
		touch test.txt;

	test `expr $i % 2` -ne 0 && \
		echo "remove test.txt" && \
		rm test.txt;

	sleep 2;
done

