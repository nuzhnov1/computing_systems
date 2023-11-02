#!/usr/bin/env python

import os
import subprocess


if __name__ == '__main__':
    os.chdir(os.path.dirname(__file__))

    subprocess.call(['cmake', 'build'])
    subprocess.call(['cmake', '--build', './build'])

    for size in (1, 5, 10, 50, 100, 500, 1000, 2000, 3000, 4000, 5000):
        subprocess.call(['./build/lab1', str(size), '4'])

    for thread in (2, 4, 8, 16, 32):
        subprocess.call(['./build/lab1', '2000', str(thread)])
