#!/usr/bin/env python3
import sys

if len(sys.argv) != 2:
	print('Incorrect usage!', file=sys.stderr)
	print('Use', sys.argv[0], '<size>', file=sys.stderr)
	sys.exit(1)

n = int(sys.argv[1])
print(' '.join(list(map(str, [i for i in range(n, 0, -1)]))))
