#!/usr/bin/env python

import os
import subprocess


if __name__ == '__main__':
    os.chdir(os.path.dirname(__file__))

    subprocess.call(['cmake', 'build'])
    subprocess.call(['cmake', '--build', './build'])

    for thread in (2, 4, 8, 16, 32):
        subprocess.call(['./build/lab1', '2000', str(thread)])
